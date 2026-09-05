# pntr_app on the ESP32

Runs a pntr_app application on an ESP32 with an SPI display, using
[PlatformIO](https://platformio.org/).

The default board profile is the **ESP32-2432S028R "Cheap Yellow Display"** (CYD): a
320x240 ILI9341 panel with an XPT2046 resistive touch panel.

``` bash
pio run -e cyd -t upload -t monitor
```

## Screen size

pntr allocates the screen as a single contiguous RGBA8888 buffer, so a 320x240 screen needs
307,200 bytes. The CYD's ESP32-WROOM-32 has no PSRAM, and its largest contiguous DRAM block
is far below that, so a full resolution screen cannot be allocated.

Instead, pick a screen size that fits and let `pntr_app_esp32` upscale it by an integer
factor onto the panel, centred with a black border:

| Screen | Bytes | Scale on a 320x240 panel |
| ------ | ----- | ------------------------ |
| 320x240 | 307,200 | Will not allocate |
| 160x120 | 76,800 | 2x, fills the panel |
| 106x80 | 33,920 | 3x |
| 80x60 | 19,200 | 4x |

This example uses 160x120. If the screen is too large to allocate, `pntr_app_esp32_setup()`
reports the size it wanted and the largest block actually available, rather than failing
silently.

## Arduino entry point

Arduino-ESP32 defines `app_main()` itself and calls `setup()` and `loop()` from its own
task. Those have C++ linkage, so `src/main.cpp` is a small shim that forwards to
`pntr_app_esp32_setup()` and `pntr_app_esp32_loop()`. Under plain ESP-IDF the header
defines `app_main()` for you and no shim is needed.

## Another board

Set `PNTR_APP_ESP32_BOARD_CUSTOM` and define the pins yourself. The configuration macros are
listed at the top of [`pntr_app_esp32.h`](../../include/pntr_app_esp32.h).

``` ini
build_flags =
    -D PNTR_APP_ESP32
    -D PNTR_APP_ESP32_BOARD_CUSTOM
    -D PNTR_APP_ESP32_LCD_MOSI=13
    -D PNTR_APP_ESP32_LCD_SCLK=14
    ; ...
    -D PNTR_APP_ESP32_NO_TOUCH
```

## SD card

Define `PNTR_APP_ESP32_SD` to mount the microSD card and route `pntr_load_file()` and
`pntr_save_file()` at it. Relative paths resolve against the mount point, so an application
asking for `resources/logo.png` reads `/sdcard/resources/logo.png`, exactly as it would on
the desktop. A missing or unreadable card is not fatal: mounting logs a warning and file
loading returns `NULL`.

### Why the touchscreen is bit-banged

The ESP32 has two general purpose SPI hosts, and the CYD wires three peripherals to three
different sets of pins:

| Peripheral | Pins | Bus |
| ---------- | ---- | --- |
| ILI9341 display | 13 / 14 / 15 / 2 | Hardware SPI2 |
| microSD | 23 / 19 / 18 / 5 | Hardware SPI3 |
| XPT2046 touch | 32 / 39 / 25 / 33 | Software SPI |

Two hosts cannot cover three pin sets, so the touch panel gets software SPI. It costs
almost nothing: the XPT2046 tops out at 2MHz, a full read is four 24-bit transfers, and the
`T_IRQ` line on GPIO 36 means it is only read at all while the panel is actually being
touched. Set `PNTR_APP_ESP32_TOUCH_DELAY` to change the clock rate.

## Images

Decoding costs far more than the finished image. `pntr_stb_image_load_image_from_memory()`
holds stb_image's decoded buffer while `pntr_image_from_pixelformat()` allocates a second
full size copy, so the peak is **twice** the image, plus stb's scratch, plus the file itself
if it came from the card.

That puts the practical ceiling for a PNG loaded off the SD card at around **64x64** on this
board. 96x96 decodes only when the file is already in flash rather than read into the heap,
and 128x128 does not fit either way.

Note that pntr crashes rather than reporting failure when it runs out of room here:
`pntr_image_from_pixelformat()` null checks its allocation in the `PNTR_PIXELFORMAT_GRAYSCALE`
branch but not in the `PNTR_PIXELFORMAT_RGBA8888` one, so an out of memory decode dereferences
NULL. Keep images small enough that it does not come up.

For static artwork, skip decoding entirely. The ESP32 memory-maps flash for reads, so a
`const pntr_color[]` baked in at build time can be drawn straight out of flash with no heap
cost at all:

``` c
static const pntr_color spriteData[] = { /* ... */ };

pntr_image sprite;
sprite.data = (pntr_color*)spriteData;
sprite.width = 32;
sprite.height = 32;
sprite.pitch = 32 * (int)sizeof(pntr_color);
sprite.subimage = false;
sprite.clip = (pntr_rectangle){0, 0, 32, 32};

pntr_draw_image(screen, &sprite, x, y);
```

Such an image is read only. It is fine as a source for `pntr_draw_image()`, but passing it
anywhere that writes, including as a destination, will fault.

## Stack size

`src/main.cpp` calls `SET_LOOP_TASK_STACK_SIZE()`. Arduino gives `loopTask` only 8KB, which
stb_image's PNG decoder overruns, and the resulting crash reports as a stack overflow with a
corrupted backtrace that points nowhere useful.

## Getting files onto the card without a reader

The board can write its own SD card. Stage the file in flash as a C array and write it out
with `pntr_save_file()` from a one-shot firmware, then flash the real application again:

``` c
extern const unsigned char logo_png[];
extern const unsigned int logo_png_size;

pntr_save_file("logo.png", logo_png, logo_png_size);
```

## Not implemented yet

- Audio. The `PNTR_APP_LOAD_SOUND_FROM_MEMORY` and friends hooks are unset, so
  `pntr_load_sound()` returns `NULL` and playback is a no-op.
- Keyboard and gamepad input. Only touch, mapped to mouse events, is wired up.
