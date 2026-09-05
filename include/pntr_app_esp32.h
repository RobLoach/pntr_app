/**********************************************************************************************
*
*   pntr_app_esp32 - ESP32 platform layer for pntr_app.
*
*   Targets microcontrollers running an SPI display. Depends only on ESP-IDF APIs
*   (driver/spi_master, driver/gpio, esp_timer), which Arduino-ESP32 also builds on, so the
*   same header serves both frameworks. Only the Arduino path has an example to build.
*
*   BOARDS:
*       PNTR_APP_ESP32_BOARD_CYD  ESP32-2432S028R "Cheap Yellow Display" (default)
*                                 320x240 ILI9341 + XPT2046 resistive touch + microSD
*
*   SPI BUSES:
*       The ESP32 has two general purpose SPI hosts, and the CYD wants three separate sets
*       of pins. The display takes one host and the SD card takes the other, so the touch
*       panel is bit-banged. The XPT2046 tops out at 2MHz and is only read while its IRQ
*       line is low, so it costs almost nothing to drive in software.
*
*   MEMORY:
*       pntr allocates the screen as one contiguous RGBA8888 buffer, so a 320x240 screen
*       needs 307200 bytes. The ESP32-WROOM-32 on the CYD has no PSRAM and its largest
*       contiguous DRAM block is well under that, so pick a smaller screen in Main() and
*       let this layer scale it up. 160x120 (76800 bytes) upscales 2x to fill the panel.
*
*   CONFIGURATION:
*       PNTR_APP_ESP32_LCD_SPI_HOST, PNTR_APP_ESP32_LCD_MOSI, PNTR_APP_ESP32_LCD_SCLK,
*       PNTR_APP_ESP32_LCD_CS, PNTR_APP_ESP32_LCD_DC, PNTR_APP_ESP32_LCD_RST,
*       PNTR_APP_ESP32_LCD_BACKLIGHT, PNTR_APP_ESP32_LCD_WIDTH, PNTR_APP_ESP32_LCD_HEIGHT,
*       PNTR_APP_ESP32_LCD_SPEED, PNTR_APP_ESP32_LCD_MADCTL,
*       PNTR_APP_ESP32_LCD_BAND_ROWS, PNTR_APP_ESP32_SCALE
*       PNTR_APP_ESP32_TOUCH_MOSI, PNTR_APP_ESP32_TOUCH_MISO, PNTR_APP_ESP32_TOUCH_SCLK,
*       PNTR_APP_ESP32_TOUCH_CS, PNTR_APP_ESP32_TOUCH_IRQ, PNTR_APP_ESP32_TOUCH_DELAY,
*       PNTR_APP_ESP32_TOUCH_MIN_X, PNTR_APP_ESP32_TOUCH_MAX_X,
*       PNTR_APP_ESP32_TOUCH_MIN_Y, PNTR_APP_ESP32_TOUCH_MAX_Y,
*       PNTR_APP_ESP32_TOUCH_SWAP_XY, PNTR_APP_ESP32_TOUCH_INVERT_X,
*       PNTR_APP_ESP32_TOUCH_INVERT_Y, PNTR_APP_ESP32_NO_TOUCH
*       PNTR_APP_ESP32_SD (opt in), PNTR_APP_ESP32_SD_SPI_HOST, PNTR_APP_ESP32_SD_MOSI,
*       PNTR_APP_ESP32_SD_MISO, PNTR_APP_ESP32_SD_SCLK, PNTR_APP_ESP32_SD_CS,
*       PNTR_APP_ESP32_SD_SPEED, PNTR_APP_ESP32_SD_MOUNT_POINT, PNTR_APP_ESP32_SD_MAX_FILES
*
*   LICENSE: zlib/libpng
*
**********************************************************************************************/

#ifdef PNTR_APP_ESP32
#ifndef PNTR_APP_ESP32_H__
#define PNTR_APP_ESP32_H__

#include <stdint.h>

#include "driver/spi_master.h"

#ifdef PNTR_APP_ESP32_SD
#include "sdmmc_cmd.h"
#endif

// Board profile. Default to the Cheap Yellow Display.
#if !defined(PNTR_APP_ESP32_BOARD_CYD) && !defined(PNTR_APP_ESP32_BOARD_CUSTOM)
    #define PNTR_APP_ESP32_BOARD_CYD
#endif

#ifdef PNTR_APP_ESP32_BOARD_CYD
    // ESP32-2432S028R: ILI9341 on HSPI, XPT2046 touch on its own VSPI bus.
    #ifndef PNTR_APP_ESP32_LCD_SPI_HOST
        #define PNTR_APP_ESP32_LCD_SPI_HOST SPI2_HOST
    #endif
    #ifndef PNTR_APP_ESP32_LCD_MOSI
        #define PNTR_APP_ESP32_LCD_MOSI 13
    #endif
    #ifndef PNTR_APP_ESP32_LCD_SCLK
        #define PNTR_APP_ESP32_LCD_SCLK 14
    #endif
    #ifndef PNTR_APP_ESP32_LCD_CS
        #define PNTR_APP_ESP32_LCD_CS 15
    #endif
    #ifndef PNTR_APP_ESP32_LCD_DC
        #define PNTR_APP_ESP32_LCD_DC 2
    #endif
    #ifndef PNTR_APP_ESP32_LCD_RST
        // Tied to the module's EN line, so there is no dedicated reset GPIO.
        #define PNTR_APP_ESP32_LCD_RST -1
    #endif
    #ifndef PNTR_APP_ESP32_LCD_BACKLIGHT
        #define PNTR_APP_ESP32_LCD_BACKLIGHT 21
    #endif
    #ifndef PNTR_APP_ESP32_LCD_WIDTH
        #define PNTR_APP_ESP32_LCD_WIDTH 320
    #endif
    #ifndef PNTR_APP_ESP32_LCD_HEIGHT
        #define PNTR_APP_ESP32_LCD_HEIGHT 240
    #endif
    #ifndef PNTR_APP_ESP32_TOUCH_MOSI
        #define PNTR_APP_ESP32_TOUCH_MOSI 32
    #endif
    #ifndef PNTR_APP_ESP32_TOUCH_MISO
        #define PNTR_APP_ESP32_TOUCH_MISO 39
    #endif
    #ifndef PNTR_APP_ESP32_TOUCH_SCLK
        #define PNTR_APP_ESP32_TOUCH_SCLK 25
    #endif
    #ifndef PNTR_APP_ESP32_TOUCH_CS
        #define PNTR_APP_ESP32_TOUCH_CS 33
    #endif
    #ifndef PNTR_APP_ESP32_TOUCH_IRQ
        #define PNTR_APP_ESP32_TOUCH_IRQ 36
    #endif
    #ifndef PNTR_APP_ESP32_TOUCH_SWAP_XY
        // The panel is wired portrait, the display is driven landscape. Swapping the axes is
        // all it takes; neither axis needs inverting once they line up.
        #define PNTR_APP_ESP32_TOUCH_SWAP_XY 1
    #endif
    // microSD, on the ESP32's default VSPI pins.
    #ifndef PNTR_APP_ESP32_SD_SPI_HOST
        #define PNTR_APP_ESP32_SD_SPI_HOST SPI3_HOST
    #endif
    #ifndef PNTR_APP_ESP32_SD_MOSI
        #define PNTR_APP_ESP32_SD_MOSI 23
    #endif
    #ifndef PNTR_APP_ESP32_SD_MISO
        #define PNTR_APP_ESP32_SD_MISO 19
    #endif
    #ifndef PNTR_APP_ESP32_SD_SCLK
        #define PNTR_APP_ESP32_SD_SCLK 18
    #endif
    #ifndef PNTR_APP_ESP32_SD_CS
        #define PNTR_APP_ESP32_SD_CS 5
    #endif
#endif  // PNTR_APP_ESP32_BOARD_CYD

#ifndef PNTR_APP_ESP32_LCD_SPEED
/**
 * SPI clock for the display, in Hz. 40MHz is safe on the CYD; many panels tolerate 65MHz.
 */
#define PNTR_APP_ESP32_LCD_SPEED (40 * 1000 * 1000)
#endif

#ifndef PNTR_APP_ESP32_LCD_MADCTL
/**
 * MADCTL value written during init, which sets rotation and colour order.
 *
 * 0x28 is landscape (MV | BGR), which suits the BGR-wired ILI9341 on the CYD. If red and
 * blue come out swapped on your panel, drop the BGR bit and use 0x20.
 */
#define PNTR_APP_ESP32_LCD_MADCTL 0x28
#endif

#ifndef PNTR_APP_ESP32_LCD_BAND_ROWS
/**
 * How many destination rows are converted and pushed to the panel per SPI transaction.
 *
 * Two buffers of `PNTR_APP_ESP32_LCD_WIDTH * this * 2` bytes are allocated from DMA-capable
 * memory, so 16 rows on a 320px panel costs 20KB total.
 */
#define PNTR_APP_ESP32_LCD_BAND_ROWS 16
#endif

#ifndef PNTR_APP_ESP32_TOUCH_MIN_X
#define PNTR_APP_ESP32_TOUCH_MIN_X 200
#endif
#ifndef PNTR_APP_ESP32_TOUCH_MAX_X
#define PNTR_APP_ESP32_TOUCH_MAX_X 3700
#endif
#ifndef PNTR_APP_ESP32_TOUCH_MIN_Y
#define PNTR_APP_ESP32_TOUCH_MIN_Y 240
#endif
#ifndef PNTR_APP_ESP32_TOUCH_MAX_Y
#define PNTR_APP_ESP32_TOUCH_MAX_Y 3800
#endif
#ifndef PNTR_APP_ESP32_TOUCH_SWAP_XY
#define PNTR_APP_ESP32_TOUCH_SWAP_XY 0
#endif
#ifndef PNTR_APP_ESP32_TOUCH_INVERT_X
#define PNTR_APP_ESP32_TOUCH_INVERT_X 0
#endif
#ifndef PNTR_APP_ESP32_TOUCH_INVERT_Y
#define PNTR_APP_ESP32_TOUCH_INVERT_Y 0
#endif

#ifndef PNTR_APP_ESP32_TOUCH_DELAY
/**
 * Microseconds each half of the bit-banged touch clock is held for.
 *
 * 1us gives roughly a 500kHz clock, well inside the XPT2046's 2MHz limit. A whole poll is
 * four 24-bit reads, so about 200us, and only while the panel is being touched.
 */
#define PNTR_APP_ESP32_TOUCH_DELAY 1
#endif

#ifdef PNTR_APP_ESP32_SD
#ifndef PNTR_APP_ESP32_SD_SPEED
/**
 * SPI clock for the SD card, in kHz.
 */
#define PNTR_APP_ESP32_SD_SPEED 20000
#endif

#ifndef PNTR_APP_ESP32_SD_MOUNT_POINT
/**
 * Where the card's FAT filesystem is mounted.
 *
 * Relative paths handed to pntr_load_file() are resolved against this, so an application
 * asking for "resources/logo.png" reads "/sdcard/resources/logo.png".
 */
#define PNTR_APP_ESP32_SD_MOUNT_POINT "/sdcard"
#endif

#ifndef PNTR_APP_ESP32_SD_MAX_FILES
/**
 * How many files may be open at once. Each one costs a sector buffer, so keep it small.
 */
#define PNTR_APP_ESP32_SD_MAX_FILES 3
#endif
#endif  // PNTR_APP_ESP32_SD

typedef struct pntr_app_esp32_platform {
    spi_device_handle_t lcd;
    uint16_t* band[2];          // DMA-capable RGB565 scratch buffers.
    int bandRows;               // Destination rows held by one band buffer.
    int bandIndex;              // Which band buffer is next to be filled.
    int bandPending;            // How many band transactions are still in flight.
    spi_transaction_t bandTransaction[2];

    int scale;                  // Integer upscale factor from screen to panel.
    int offsetX;                // Where the scaled screen lands on the panel.
    int offsetY;

    int64_t lastTime;           // esp_timer_get_time() at the last delta time update.

    #ifndef PNTR_APP_ESP32_NO_TOUCH
        // The touch panel is bit-banged, so it needs no driver handle.
        bool touchAvailable;
        bool touchDown;
        float mouseX;
        float mouseY;
    #endif

    #ifdef PNTR_APP_ESP32_SD
        sdmmc_card_t* card;
    #endif
} pntr_app_esp32_platform;

/**
 * Runs the pntr_app entry point and initializes the application.
 *
 * Called for you by app_main() under ESP-IDF, or by setup() under Arduino.
 */
PNTR_APP_API bool pntr_app_esp32_setup(void);

/**
 * Runs a single frame: events, update and render.
 *
 * @return True while the application should keep running.
 */
PNTR_APP_API bool pntr_app_esp32_loop(void);

// pntr_app.h provides int main(), which microcontrollers do not use.
#ifndef PNTR_APP_NO_ENTRY
#define PNTR_APP_NO_ENTRY
#endif

#endif  // PNTR_APP_ESP32_H__

#if defined(PNTR_APP_IMPLEMENTATION) && !defined(PNTR_APP_HEADER_ONLY)
#ifndef PNTR_APP_ESP32_IMPLEMENTATION_ONCE
#define PNTR_APP_ESP32_IMPLEMENTATION_ONCE

#include <stdio.h>
#include <string.h>  // memset

#include "driver/gpio.h"
#include "esp_heap_caps.h"
#ifdef PNTR_APP_ESP32_SD
#include "driver/sdspi_host.h"
#include "esp_vfs_fat.h"
#endif
#include "esp_rom_sys.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The application, kept here because the entry points have nowhere else to hold it.
 */
static pntr_app pntr_app_esp32_app;

// ILI9341 commands used during init and blitting.
#define PNTR_APP_ESP32_CMD_SWRESET 0x01
#define PNTR_APP_ESP32_CMD_SLPOUT  0x11
#define PNTR_APP_ESP32_CMD_INVOFF  0x20
#define PNTR_APP_ESP32_CMD_DISPON  0x29
#define PNTR_APP_ESP32_CMD_CASET   0x2A
#define PNTR_APP_ESP32_CMD_RASET   0x2B
#define PNTR_APP_ESP32_CMD_RAMWR   0x2C
#define PNTR_APP_ESP32_CMD_MADCTL  0x36
#define PNTR_APP_ESP32_CMD_COLMOD  0x3A

/**
 * Raised before every SPI transaction to drive the data/command line.
 *
 * The transaction's `user` field holds 0 for a command, 1 for data.
 */
static void IRAM_ATTR pntr_app_esp32_lcd_dc(spi_transaction_t* transaction) {
    gpio_set_level((gpio_num_t)PNTR_APP_ESP32_LCD_DC, (uint32_t)(uintptr_t)transaction->user);
}

static void pntr_app_esp32_lcd_command(pntr_app_esp32_platform* platform, uint8_t command) {
    spi_transaction_t transaction;
    memset(&transaction, 0, sizeof(transaction));
    transaction.flags = SPI_TRANS_USE_TXDATA;
    transaction.length = 8;
    transaction.tx_data[0] = command;
    transaction.user = (void*)0;
    spi_device_polling_transmit(platform->lcd, &transaction);
}

/**
 * Sends up to four bytes of command parameters.
 *
 * They travel inside the transaction struct, so the caller does not need a DMA-capable
 * buffer. Pixel data goes out through pntr_app_esp32_lcd_send_band() instead.
 */
static void pntr_app_esp32_lcd_data(pntr_app_esp32_platform* platform, const uint8_t* data, int length) {
    if (length <= 0 || length > 4) {
        return;
    }

    spi_transaction_t transaction;
    memset(&transaction, 0, sizeof(transaction));
    transaction.flags = SPI_TRANS_USE_TXDATA;
    transaction.length = (size_t)length * 8;
    memcpy(transaction.tx_data, data, (size_t)length);
    transaction.user = (void*)1;
    spi_device_polling_transmit(platform->lcd, &transaction);
}

/**
 * Points the panel's memory writes at the given rectangle.
 */
static void pntr_app_esp32_lcd_window(pntr_app_esp32_platform* platform, int x, int y, int width, int height) {
    uint8_t bounds[4];
    int right = x + width - 1;
    int bottom = y + height - 1;

    bounds[0] = (uint8_t)(x >> 8);
    bounds[1] = (uint8_t)(x & 0xFF);
    bounds[2] = (uint8_t)(right >> 8);
    bounds[3] = (uint8_t)(right & 0xFF);
    pntr_app_esp32_lcd_command(platform, PNTR_APP_ESP32_CMD_CASET);
    pntr_app_esp32_lcd_data(platform, bounds, 4);

    bounds[0] = (uint8_t)(y >> 8);
    bounds[1] = (uint8_t)(y & 0xFF);
    bounds[2] = (uint8_t)(bottom >> 8);
    bounds[3] = (uint8_t)(bottom & 0xFF);
    pntr_app_esp32_lcd_command(platform, PNTR_APP_ESP32_CMD_RASET);
    pntr_app_esp32_lcd_data(platform, bounds, 4);

    pntr_app_esp32_lcd_command(platform, PNTR_APP_ESP32_CMD_RAMWR);
}

/**
 * Waits for every band transaction still in flight.
 *
 * Must be called before any polling transaction, and before touching a band buffer that the
 * DMA engine may still be reading.
 */
static void pntr_app_esp32_lcd_flush(pntr_app_esp32_platform* platform) {
    while (platform->bandPending > 0) {
        spi_transaction_t* result;
        spi_device_get_trans_result(platform->lcd, &result, portMAX_DELAY);
        platform->bandPending--;
    }
}

/**
 * Returns the band buffer to fill next, waiting only if it is still being transmitted.
 *
 * With two buffers, the CPU converts one band while the DMA engine sends the other.
 */
static uint16_t* pntr_app_esp32_lcd_next_band(pntr_app_esp32_platform* platform) {
    if (platform->bandPending >= 2) {
        spi_transaction_t* result;
        spi_device_get_trans_result(platform->lcd, &result, portMAX_DELAY);
        platform->bandPending--;
    }

    return platform->band[platform->bandIndex];
}

/**
 * Hands the filled band buffer to the SPI driver without waiting for it to finish.
 */
static void pntr_app_esp32_lcd_send_band(pntr_app_esp32_platform* platform, int pixels) {
    spi_transaction_t* transaction = &platform->bandTransaction[platform->bandIndex];
    memset(transaction, 0, sizeof(spi_transaction_t));
    transaction->length = (size_t)pixels * 16;
    transaction->tx_buffer = platform->band[platform->bandIndex];
    transaction->user = (void*)1;

    if (spi_device_queue_trans(platform->lcd, transaction, portMAX_DELAY) != ESP_OK) {
        return;
    }

    platform->bandPending++;
    platform->bandIndex = platform->bandIndex ^ 1;
}

static void pntr_app_esp32_lcd_init(pntr_app_esp32_platform* platform) {
    #if PNTR_APP_ESP32_LCD_RST >= 0
        gpio_set_level((gpio_num_t)PNTR_APP_ESP32_LCD_RST, 0);
        vTaskDelay(pdMS_TO_TICKS(20));
        gpio_set_level((gpio_num_t)PNTR_APP_ESP32_LCD_RST, 1);
        vTaskDelay(pdMS_TO_TICKS(120));
    #endif

    uint8_t value;

    pntr_app_esp32_lcd_command(platform, PNTR_APP_ESP32_CMD_SWRESET);
    vTaskDelay(pdMS_TO_TICKS(150));

    pntr_app_esp32_lcd_command(platform, PNTR_APP_ESP32_CMD_SLPOUT);
    vTaskDelay(pdMS_TO_TICKS(120));

    value = PNTR_APP_ESP32_LCD_MADCTL;
    pntr_app_esp32_lcd_command(platform, PNTR_APP_ESP32_CMD_MADCTL);
    pntr_app_esp32_lcd_data(platform, &value, 1);

    // 16 bits per pixel.
    value = 0x55;
    pntr_app_esp32_lcd_command(platform, PNTR_APP_ESP32_CMD_COLMOD);
    pntr_app_esp32_lcd_data(platform, &value, 1);

    pntr_app_esp32_lcd_command(platform, PNTR_APP_ESP32_CMD_INVOFF);
    pntr_app_esp32_lcd_command(platform, PNTR_APP_ESP32_CMD_DISPON);
    vTaskDelay(pdMS_TO_TICKS(20));
}

/**
 * Paints the whole panel a single colour, used to clear the letterbox around the screen.
 */
static void pntr_app_esp32_lcd_fill(pntr_app_esp32_platform* platform, uint16_t color) {
    pntr_app_esp32_lcd_flush(platform);
    pntr_app_esp32_lcd_window(platform, 0, 0, PNTR_APP_ESP32_LCD_WIDTH, PNTR_APP_ESP32_LCD_HEIGHT);

    for (int y = 0; y < PNTR_APP_ESP32_LCD_HEIGHT; y += platform->bandRows) {
        int rows = PNTR_APP_ESP32_LCD_HEIGHT - y;
        if (rows > platform->bandRows) {
            rows = platform->bandRows;
        }

        uint16_t* buffer = pntr_app_esp32_lcd_next_band(platform);
        int pixels = PNTR_APP_ESP32_LCD_WIDTH * rows;
        for (int i = 0; i < pixels; i++) {
            buffer[i] = color;
        }

        pntr_app_esp32_lcd_send_band(platform, pixels);
    }

    pntr_app_esp32_lcd_flush(platform);
}

#ifndef PNTR_APP_ESP32_NO_TOUCH
/**
 * Drives one bit-banged clock pulse, sampling MISO while the clock is high.
 *
 * The XPT2046 latches MOSI on the rising edge and shifts MISO out on the falling one, so
 * the value is read after raising the clock.
 */
static int pntr_app_esp32_touch_clock(void) {
    gpio_set_level((gpio_num_t)PNTR_APP_ESP32_TOUCH_SCLK, 1);
    esp_rom_delay_us(PNTR_APP_ESP32_TOUCH_DELAY);
    int bit = gpio_get_level((gpio_num_t)PNTR_APP_ESP32_TOUCH_MISO);
    gpio_set_level((gpio_num_t)PNTR_APP_ESP32_TOUCH_SCLK, 0);
    esp_rom_delay_us(PNTR_APP_ESP32_TOUCH_DELAY);

    return bit;
}

/**
 * Reads one 12-bit channel from the XPT2046 over software SPI.
 *
 * The ESP32's two hardware SPI hosts are taken by the display and the SD card, and the CYD
 * wires the touch panel to a third set of pins, so this is bit-banged instead.
 */
static int pntr_app_esp32_touch_channel(uint8_t command) {
    gpio_set_level((gpio_num_t)PNTR_APP_ESP32_TOUCH_CS, 0);
    esp_rom_delay_us(PNTR_APP_ESP32_TOUCH_DELAY);

    // Command byte, most significant bit first.
    for (int i = 7; i >= 0; i--) {
        gpio_set_level((gpio_num_t)PNTR_APP_ESP32_TOUCH_MOSI, (command >> i) & 1);
        esp_rom_delay_us(PNTR_APP_ESP32_TOUCH_DELAY);
        pntr_app_esp32_touch_clock();
    }

    // Sixteen more clocks carry the result back, left aligned with three padding bits.
    int value = 0;
    for (int i = 0; i < 16; i++) {
        value = (value << 1) | pntr_app_esp32_touch_clock();
    }

    gpio_set_level((gpio_num_t)PNTR_APP_ESP32_TOUCH_CS, 1);

    return (value >> 3) & 0x0FFF;
}

/**
 * Puts the bit-banged touch pins into a known state.
 *
 * @return True if the pins could be configured.
 */
static bool pntr_app_esp32_touch_init(void) {
    gpio_config_t outputs;
    memset(&outputs, 0, sizeof(outputs));
    outputs.mode = GPIO_MODE_OUTPUT;
    outputs.pin_bit_mask = (1ULL << PNTR_APP_ESP32_TOUCH_MOSI) |
        (1ULL << PNTR_APP_ESP32_TOUCH_SCLK) |
        (1ULL << PNTR_APP_ESP32_TOUCH_CS);
    if (gpio_config(&outputs) != ESP_OK) {
        return false;
    }

    // MISO and IRQ are inputs. Both are input-only pins on the CYD, so no pull is set.
    gpio_config_t inputs;
    memset(&inputs, 0, sizeof(inputs));
    inputs.mode = GPIO_MODE_INPUT;
    inputs.pin_bit_mask = (1ULL << PNTR_APP_ESP32_TOUCH_MISO) |
        (1ULL << PNTR_APP_ESP32_TOUCH_IRQ);
    if (gpio_config(&inputs) != ESP_OK) {
        return false;
    }

    gpio_set_level((gpio_num_t)PNTR_APP_ESP32_TOUCH_CS, 1);
    gpio_set_level((gpio_num_t)PNTR_APP_ESP32_TOUCH_SCLK, 0);
    gpio_set_level((gpio_num_t)PNTR_APP_ESP32_TOUCH_MOSI, 0);

    return true;
}

/**
 * Samples the touch panel and maps it onto the panel's pixel grid.
 *
 * @return True when a touch is currently registered.
 */
static bool pntr_app_esp32_touch_read(pntr_app_esp32_platform* platform, float* x, float* y) {
    if (gpio_get_level((gpio_num_t)PNTR_APP_ESP32_TOUCH_IRQ) != 0) {
        return false;
    }

    // Throw away the first sample of each axis, it settles late.
    pntr_app_esp32_touch_channel(0xD0);
    int rawX = pntr_app_esp32_touch_channel(0xD0);
    pntr_app_esp32_touch_channel(0x90);
    int rawY = pntr_app_esp32_touch_channel(0x90);

    if (rawX <= 0 || rawY <= 0) {
        return false;
    }

    float normalX = (float)(rawX - PNTR_APP_ESP32_TOUCH_MIN_X) /
        (float)(PNTR_APP_ESP32_TOUCH_MAX_X - PNTR_APP_ESP32_TOUCH_MIN_X);
    float normalY = (float)(rawY - PNTR_APP_ESP32_TOUCH_MIN_Y) /
        (float)(PNTR_APP_ESP32_TOUCH_MAX_Y - PNTR_APP_ESP32_TOUCH_MIN_Y);

    #if PNTR_APP_ESP32_TOUCH_SWAP_XY
        float swap = normalX;
        normalX = normalY;
        normalY = swap;
    #endif
    #if PNTR_APP_ESP32_TOUCH_INVERT_X
        normalX = 1.0f - normalX;
    #endif
    #if PNTR_APP_ESP32_TOUCH_INVERT_Y
        normalY = 1.0f - normalY;
    #endif

    if (normalX < 0.0f || normalX > 1.0f || normalY < 0.0f || normalY > 1.0f) {
        return false;
    }

    // Undo the letterboxing so the app sees coordinates in its own screen space.
    *x = (normalX * (float)PNTR_APP_ESP32_LCD_WIDTH - (float)platform->offsetX) / (float)platform->scale;
    *y = (normalY * (float)PNTR_APP_ESP32_LCD_HEIGHT - (float)platform->offsetY) / (float)platform->scale;

    return true;
}
#endif  // PNTR_APP_ESP32_NO_TOUCH

#ifdef PNTR_APP_ESP32_SD
/**
 * Resolves an application path against the card's mount point.
 *
 * Applications ask for "resources/logo.png" the same way they do on the desktop, and that
 * becomes "/sdcard/resources/logo.png". A path that is already absolute is left alone.
 */
static const char* pntr_app_esp32_sd_path(const char* fileName, char* buffer, size_t size) {
    if (fileName == NULL) {
        return NULL;
    }

    if (fileName[0] == '/') {
        return fileName;
    }

    int written = snprintf(buffer, size, "%s/%s", PNTR_APP_ESP32_SD_MOUNT_POINT, fileName);
    if (written <= 0 || (size_t)written >= size) {
        return NULL;
    }

    return buffer;
}

/**
 * Mounts the card's FAT filesystem.
 *
 * @return True if a card was mounted.
 */
static bool pntr_app_esp32_sd_mount(pntr_app_esp32_platform* platform) {
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = PNTR_APP_ESP32_SD_SPI_HOST;
    host.max_freq_khz = PNTR_APP_ESP32_SD_SPEED;

    spi_bus_config_t bus;
    memset(&bus, 0, sizeof(bus));
    bus.mosi_io_num = PNTR_APP_ESP32_SD_MOSI;
    bus.miso_io_num = PNTR_APP_ESP32_SD_MISO;
    bus.sclk_io_num = PNTR_APP_ESP32_SD_SCLK;
    bus.quadwp_io_num = -1;
    bus.quadhd_io_num = -1;
    bus.max_transfer_sz = 4096;

    if (spi_bus_initialize((spi_host_device_t)host.slot, &bus, SPI_DMA_CH_AUTO) != ESP_OK) {
        return false;
    }

    sdspi_device_config_t slot = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot.gpio_cs = (gpio_num_t)PNTR_APP_ESP32_SD_CS;
    slot.host_id = (spi_host_device_t)host.slot;

    esp_vfs_fat_sdmmc_mount_config_t mount;
    memset(&mount, 0, sizeof(mount));
    mount.format_if_mount_failed = false;
    mount.max_files = PNTR_APP_ESP32_SD_MAX_FILES;
    mount.allocation_unit_size = 16 * 1024;

    if (esp_vfs_fat_sdspi_mount(PNTR_APP_ESP32_SD_MOUNT_POINT, &host, &slot, &mount, &platform->card) != ESP_OK) {
        spi_bus_free((spi_host_device_t)host.slot);
        platform->card = NULL;
        return false;
    }

    return true;
}

static void pntr_app_esp32_sd_unmount(pntr_app_esp32_platform* platform) {
    if (platform->card == NULL) {
        return;
    }

    esp_vfs_fat_sdcard_unmount(PNTR_APP_ESP32_SD_MOUNT_POINT, platform->card);
    spi_bus_free((spi_host_device_t)PNTR_APP_ESP32_SD_SPI_HOST);
    platform->card = NULL;
}

#ifndef PNTR_LOAD_FILE
#define PNTR_LOAD_FILE pntr_app_esp32_load_file
/**
 * Reads a whole file from the SD card into memory for pntr_load_file().
 */
unsigned char* pntr_app_esp32_load_file(const char* fileName, unsigned int* bytesRead) {
    if (bytesRead != NULL) {
        *bytesRead = 0;
    }

    char resolved[256];
    const char* path = pntr_app_esp32_sd_path(fileName, resolved, sizeof(resolved));
    if (path == NULL) {
        return NULL;
    }

    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return NULL;
    }

    long size = ftell(file);
    if (size <= 0) {
        fclose(file);
        return NULL;
    }
    rewind(file);

    unsigned char* data = (unsigned char*)pntr_load_memory((size_t)size);
    if (data == NULL) {
        fclose(file);
        return NULL;
    }

    size_t read = fread(data, 1, (size_t)size, file);
    fclose(file);

    if (read != (size_t)size) {
        pntr_unload_memory(data);
        return NULL;
    }

    if (bytesRead != NULL) {
        *bytesRead = (unsigned int)size;
    }

    return data;
}
#endif  // PNTR_LOAD_FILE

#ifndef PNTR_SAVE_FILE
#define PNTR_SAVE_FILE pntr_app_esp32_save_file
/**
 * Writes a buffer out to the SD card for pntr_save_file().
 */
bool pntr_app_esp32_save_file(const char* fileName, const void* data, unsigned int bytesToWrite) {
    if (data == NULL) {
        return false;
    }

    char resolved[256];
    const char* path = pntr_app_esp32_sd_path(fileName, resolved, sizeof(resolved));
    if (path == NULL) {
        return false;
    }

    FILE* file = fopen(path, "wb");
    if (file == NULL) {
        return false;
    }

    size_t written = fwrite(data, 1, (size_t)bytesToWrite, file);
    fclose(file);

    return written == (size_t)bytesToWrite;
}
#endif  // PNTR_SAVE_FILE
#endif  // PNTR_APP_ESP32_SD

/**
 * Works out the integer upscale and letterbox offsets for a screen of the given size.
 *
 * The size is passed in rather than read from the app, because pntr_app_set_size() asks the
 * platform to resize before it updates `app->width` and `app->height`.
 */
static void pntr_app_esp32_update_layout(pntr_app* app, int width, int height) {
    pntr_app_esp32_platform* platform = (pntr_app_esp32_platform*)app->platform;

    #ifdef PNTR_APP_ESP32_SCALE
        platform->scale = PNTR_APP_ESP32_SCALE;
    #else
        int scaleX = PNTR_APP_ESP32_LCD_WIDTH / (width > 0 ? width : 1);
        int scaleY = PNTR_APP_ESP32_LCD_HEIGHT / (height > 0 ? height : 1);
        platform->scale = scaleX < scaleY ? scaleX : scaleY;
    #endif

    if (platform->scale < 1) {
        platform->scale = 1;
    }

    // A band buffer holds `bandRows` destination rows, and one source row expands to `scale`
    // of them, so a scale beyond that would overrun the buffer during render.
    if (platform->scale > platform->bandRows) {
        platform->scale = platform->bandRows;
    }

    platform->offsetX = (PNTR_APP_ESP32_LCD_WIDTH - width * platform->scale) / 2;
    platform->offsetY = (PNTR_APP_ESP32_LCD_HEIGHT - height * platform->scale) / 2;

    if (platform->offsetX < 0) {
        platform->offsetX = 0;
    }
    if (platform->offsetY < 0) {
        platform->offsetY = 0;
    }
}

bool pntr_app_platform_init(pntr_app* app) {
    if (app == NULL) {
        return false;
    }

    pntr_app_esp32_platform* platform = (pntr_app_esp32_platform*)pntr_load_memory(sizeof(pntr_app_esp32_platform));
    if (platform == NULL) {
        return false;
    }
    memset(platform, 0, sizeof(pntr_app_esp32_platform));
    app->platform = platform;

    platform->bandRows = PNTR_APP_ESP32_LCD_BAND_ROWS;

    // Data/command, reset and backlight lines.
    gpio_config_t outputs;
    memset(&outputs, 0, sizeof(outputs));
    outputs.mode = GPIO_MODE_OUTPUT;
    outputs.pin_bit_mask = 1ULL << PNTR_APP_ESP32_LCD_DC;
    #if PNTR_APP_ESP32_LCD_RST >= 0
        outputs.pin_bit_mask |= 1ULL << PNTR_APP_ESP32_LCD_RST;
    #endif
    #if PNTR_APP_ESP32_LCD_BACKLIGHT >= 0
        outputs.pin_bit_mask |= 1ULL << PNTR_APP_ESP32_LCD_BACKLIGHT;
    #endif
    gpio_config(&outputs);

    // Display bus.
    spi_bus_config_t bus;
    memset(&bus, 0, sizeof(bus));
    bus.mosi_io_num = PNTR_APP_ESP32_LCD_MOSI;
    bus.miso_io_num = -1;
    bus.sclk_io_num = PNTR_APP_ESP32_LCD_SCLK;
    bus.quadwp_io_num = -1;
    bus.quadhd_io_num = -1;
    bus.max_transfer_sz = PNTR_APP_ESP32_LCD_WIDTH * platform->bandRows * 2;

    if (spi_bus_initialize(PNTR_APP_ESP32_LCD_SPI_HOST, &bus, SPI_DMA_CH_AUTO) != ESP_OK) {
        pntr_app_log(PNTR_APP_LOG_ERROR, "pntr_app_esp32: failed to initialize the display SPI bus");
        pntr_unload_memory(platform);
        app->platform = NULL;
        return false;
    }

    spi_device_interface_config_t lcd;
    memset(&lcd, 0, sizeof(lcd));
    lcd.clock_speed_hz = PNTR_APP_ESP32_LCD_SPEED;
    lcd.mode = 0;
    lcd.spics_io_num = PNTR_APP_ESP32_LCD_CS;
    lcd.queue_size = 2;
    lcd.pre_cb = pntr_app_esp32_lcd_dc;
    lcd.flags = SPI_DEVICE_NO_DUMMY;

    if (spi_bus_add_device(PNTR_APP_ESP32_LCD_SPI_HOST, &lcd, &platform->lcd) != ESP_OK) {
        pntr_app_log(PNTR_APP_LOG_ERROR, "pntr_app_esp32: failed to add the display SPI device");
        spi_bus_free(PNTR_APP_ESP32_LCD_SPI_HOST);
        pntr_unload_memory(platform);
        app->platform = NULL;
        return false;
    }

    // Scratch buffers for RGBA to RGB565 conversion, which the DMA engine reads from.
    size_t bandSize = (size_t)PNTR_APP_ESP32_LCD_WIDTH * (size_t)platform->bandRows * sizeof(uint16_t);
    for (int i = 0; i < 2; i++) {
        platform->band[i] = (uint16_t*)heap_caps_malloc(bandSize, MALLOC_CAP_DMA);
        if (platform->band[i] == NULL) {
            pntr_app_log(PNTR_APP_LOG_ERROR, "pntr_app_esp32: out of DMA memory for the band buffers");
            pntr_app_platform_close(app);
            return false;
        }
    }

    pntr_app_esp32_lcd_init(platform);
    pntr_app_esp32_update_layout(app, app->width, app->height);
    pntr_app_esp32_lcd_fill(platform, 0x0000);

    #if PNTR_APP_ESP32_LCD_BACKLIGHT >= 0
        gpio_set_level((gpio_num_t)PNTR_APP_ESP32_LCD_BACKLIGHT, 1);
    #endif

    #ifndef PNTR_APP_ESP32_NO_TOUCH
        platform->touchAvailable = pntr_app_esp32_touch_init();
        if (!platform->touchAvailable) {
            pntr_app_log(PNTR_APP_LOG_WARNING, "pntr_app_esp32: touch pins unavailable, continuing without it");
        }
    #endif

    #ifdef PNTR_APP_ESP32_SD
        if (!pntr_app_esp32_sd_mount(platform)) {
            // A missing or unformatted card is not fatal, file loading just returns NULL.
            pntr_app_log(PNTR_APP_LOG_WARNING, "pntr_app_esp32: no SD card mounted, file loading is unavailable");
        }
    #endif

    // Random Number Generator
    pntr_app_random_set_seed(app, (uint64_t)esp_timer_get_time());

    platform->lastTime = esp_timer_get_time();

    return true;
}

bool pntr_app_platform_events(pntr_app* app) {
    if (app == NULL || app->platform == NULL) {
        return false;
    }

    #ifndef PNTR_APP_ESP32_NO_TOUCH
        pntr_app_esp32_platform* platform = (pntr_app_esp32_platform*)app->platform;
        if (!platform->touchAvailable) {
            return true;
        }

        pntr_app_event event;
        memset(&event, 0, sizeof(event));
        event.app = app;
        event.mouseButton = PNTR_APP_MOUSE_BUTTON_LEFT;

        float x = 0.0f;
        float y = 0.0f;
        bool down = pntr_app_esp32_touch_read(platform, &x, &y);

        if (down) {
            event.mouseX = x;
            event.mouseY = y;
            event.mouseDeltaX = x - platform->mouseX;
            event.mouseDeltaY = y - platform->mouseY;
            platform->mouseX = x;
            platform->mouseY = y;

            if (event.mouseDeltaX != 0.0f || event.mouseDeltaY != 0.0f) {
                event.type = PNTR_APP_EVENTTYPE_MOUSE_MOVE;
                pntr_app_process_event(app, &event);
            }

            if (!platform->touchDown) {
                platform->touchDown = true;
                event.type = PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN;
                pntr_app_process_event(app, &event);
            }
        }
        else if (platform->touchDown) {
            platform->touchDown = false;
            event.mouseX = platform->mouseX;
            event.mouseY = platform->mouseY;
            event.type = PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP;
            pntr_app_process_event(app, &event);
        }
    #endif

    return true;
}

bool pntr_app_platform_render(pntr_app* app) {
    if (app == NULL || app->platform == NULL || app->screen == NULL) {
        return false;
    }

    pntr_app_esp32_platform* platform = (pntr_app_esp32_platform*)app->platform;
    pntr_image* screen = app->screen;

    int scale = platform->scale;

    // Clip to whatever actually lands on the panel.
    int columns = screen->width;
    if (platform->offsetX + columns * scale > PNTR_APP_ESP32_LCD_WIDTH) {
        columns = (PNTR_APP_ESP32_LCD_WIDTH - platform->offsetX) / scale;
    }
    int rows = screen->height;
    if (platform->offsetY + rows * scale > PNTR_APP_ESP32_LCD_HEIGHT) {
        rows = (PNTR_APP_ESP32_LCD_HEIGHT - platform->offsetY) / scale;
    }
    if (columns <= 0 || rows <= 0) {
        return true;
    }

    int width = columns * scale;
    int height = rows * scale;

    pntr_app_esp32_lcd_flush(platform);
    pntr_app_esp32_lcd_window(platform, platform->offsetX, platform->offsetY, width, height);

    // Bands have to hold whole source rows so a row is only converted once.
    int rowsPerBand = platform->bandRows / scale;
    if (rowsPerBand < 1) {
        rowsPerBand = 1;
    }

    for (int y = 0; y < rows; y += rowsPerBand) {
        int sourceRows = rows - y;
        if (sourceRows > rowsPerBand) {
            sourceRows = rowsPerBand;
        }

        uint16_t* buffer = pntr_app_esp32_lcd_next_band(platform);
        uint16_t* out = buffer;

        for (int sourceY = y; sourceY < y + sourceRows; sourceY++) {
            uint16_t* rowStart = out;
            pntr_color* source = (pntr_color*)((unsigned char*)screen->data + (size_t)sourceY * (size_t)screen->pitch);

            // Convert one source row, expanding each pixel horizontally.
            for (int sourceX = 0; sourceX < columns; sourceX++) {
                pntr_color color = source[sourceX];
                uint16_t pixel = (uint16_t)(((color.rgba.r & 0xF8) << 8) |
                    ((color.rgba.g & 0xFC) << 3) |
                    (color.rgba.b >> 3));

                // The panel expects big-endian RGB565, the ESP32 is little-endian.
                pixel = (uint16_t)((pixel >> 8) | (pixel << 8));

                for (int i = 0; i < scale; i++) {
                    *out++ = pixel;
                }
            }

            // Repeat the converted row to expand vertically.
            for (int i = 1; i < scale; i++) {
                memcpy(out, rowStart, (size_t)width * sizeof(uint16_t));
                out += width;
            }
        }

        pntr_app_esp32_lcd_send_band(platform, (int)(out - buffer));
    }

    pntr_app_esp32_lcd_flush(platform);

    return true;
}

void pntr_app_platform_close(pntr_app* app) {
    if (app == NULL || app->platform == NULL) {
        return;
    }

    pntr_app_esp32_platform* platform = (pntr_app_esp32_platform*)app->platform;

    pntr_app_esp32_lcd_flush(platform);

    #if PNTR_APP_ESP32_LCD_BACKLIGHT >= 0
        gpio_set_level((gpio_num_t)PNTR_APP_ESP32_LCD_BACKLIGHT, 0);
    #endif

    #ifdef PNTR_APP_ESP32_SD
        pntr_app_esp32_sd_unmount(platform);
    #endif

    for (int i = 0; i < 2; i++) {
        if (platform->band[i] != NULL) {
            heap_caps_free(platform->band[i]);
            platform->band[i] = NULL;
        }
    }

    if (platform->lcd != NULL) {
        spi_bus_remove_device(platform->lcd);
        platform->lcd = NULL;
    }
    spi_bus_free(PNTR_APP_ESP32_LCD_SPI_HOST);

    pntr_unload_memory(platform);
    app->platform = NULL;
}

bool pntr_app_platform_update_delta_time(pntr_app* app) {
    if (app == NULL || app->platform == NULL) {
        return false;
    }

    pntr_app_esp32_platform* platform = (pntr_app_esp32_platform*)app->platform;

    int64_t now = esp_timer_get_time();
    int64_t elapsed = now - platform->lastTime;

    if (app->fps > 0) {
        int64_t target = 1000000 / app->fps;
        if (elapsed < target) {
            return false;
        }
    }

    platform->lastTime = now;
    app->deltaTime = (float)elapsed / 1000000.0f;

    return true;
}

bool pntr_app_platform_set_size(pntr_app* app, int width, int height) {
    if (app == NULL || app->platform == NULL) {
        return false;
    }

    pntr_app_esp32_platform* platform = (pntr_app_esp32_platform*)app->platform;

    // The letterbox moves with the screen, so repaint the border.
    pntr_app_esp32_update_layout(app, width, height);
    pntr_app_esp32_lcd_fill(platform, 0x0000);

    return true;
}

#ifndef PNTR_APP_LOG
    #define PNTR_APP_LOG pntr_app_esp32_log
    void pntr_app_esp32_log(pntr_app_log_type type, const char* message) {
        #ifdef NDEBUG
            if (type == PNTR_APP_LOG_DEBUG) {
                return;
            }
        #endif

        const char* logType;
        switch (type) {
            case PNTR_APP_LOG_INFO: logType = "INFO"; break;
            case PNTR_APP_LOG_WARNING: logType = "WARN"; break;
            case PNTR_APP_LOG_ERROR: logType = "ERROR"; break;
            default: logType = "DEBUG"; break;
        }

        printf("[%s] %s\n", logType, message);
    }
#endif

PNTR_APP_API bool pntr_app_esp32_setup(void) {
    pntr_app_esp32_app = PNTR_APP_MAIN(0, NULL);

    // pntr allocates the screen as one contiguous RGBA8888 block, which is the first thing
    // to run out on a board without PSRAM. Say so plainly rather than failing silently.
    size_t needed = (size_t)pntr_app_esp32_app.width * (size_t)pntr_app_esp32_app.height * 4;
    size_t largest = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
    if (needed > largest) {
        char message[160];
        snprintf(message, sizeof(message),
            "pntr_app_esp32: a %dx%d screen needs %u bytes but the largest free block is %u. Use a smaller size.",
            pntr_app_esp32_app.width, pntr_app_esp32_app.height, (unsigned int)needed, (unsigned int)largest);
        pntr_app_log(PNTR_APP_LOG_ERROR, message);
        return false;
    }

    if (!pntr_app_init(&pntr_app_esp32_app, 0, NULL)) {
        pntr_app_log(PNTR_APP_LOG_ERROR, "pntr_app_esp32: failed to initialize the application");
        return false;
    }

    return true;
}

PNTR_APP_API bool pntr_app_esp32_loop(void) {
    pntr_app* app = &pntr_app_esp32_app;

    pntr_app_pre_events(app);
    if (!pntr_app_platform_events(app)) {
        return false;
    }

    if (pntr_app_platform_update_delta_time(app) && app->update != NULL) {
        pntr_app_update_fps(app);
        if (!app->update(app, app->screen)) {
            return false;
        }

        // Nothing has changed unless update() ran, so only redraw alongside it.
        if (!pntr_app_platform_render(app)) {
            return false;
        }
    }
    else {
        // Waiting for the next frame. Sleep so the idle task and the watchdog get a turn.
        vTaskDelay(1);
    }

    return true;
}

#ifndef PNTR_APP_ESP32_NO_ENTRY_POINT
#ifdef ARDUINO
// Arduino-ESP32 owns app_main() and calls setup()/loop() from its own task. Those two have
// C++ linkage, so a C application needs the small shim in the example's main.cpp.
#else
// Plain ESP-IDF, where app_main() is ours to define.
void app_main(void) {
    if (!pntr_app_esp32_setup()) {
        return;
    }

    while (pntr_app_esp32_loop()) {
        // Keep running.
    }

    pntr_app_close(&pntr_app_esp32_app);
}
#endif  // ARDUINO
#endif  // PNTR_APP_ESP32_NO_ENTRY_POINT

#ifdef __cplusplus
}
#endif

#endif  // PNTR_APP_ESP32_IMPLEMENTATION_ONCE
#endif  // PNTR_APP_IMPLEMENTATION && !PNTR_APP_HEADER_ONLY
#endif  // PNTR_APP_ESP32
