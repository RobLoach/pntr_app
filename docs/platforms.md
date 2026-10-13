# Platforms

pntr_app is one core (`include/pntr_app.h`) and one header per platform backend. The core owns
the `pntr_app` struct, the input state, the screen buffer and the main loop. A backend owns the
window, the display, the input devices and the clock.

This document is the contract a backend has to satisfy. Read it alongside
[`include/pntr_app_cli.h`](../include/pntr_app_cli.h) (the smallest backend) and
[`include/pntr_app_sdl.h`](../include/pntr_app_sdl.h) (the most complete one). Line numbers below
are a convenience; the function names are the reliable anchor, so grep if one has drifted.

## File layout

Every backend is a single header named `pntr_app_<platform>.h` in `include/`, wrapped in three
nested guards:

1. `#ifdef PNTR_APP_<PLATFORM>` so the file compiles to nothing unless selected.
2. `#ifndef PNTR_APP_<PLATFORM>_H__` for the declarations, which are included twice: once by the
   header-only pass in `include/pntr_app.h:833-839`, and once by the implementation pass at
   `include/pntr_app.h:911-916`.
3. `#if defined(PNTR_APP_IMPLEMENTATION) && !defined(PNTR_APP_HEADER_ONLY)` plus a
   `PNTR_APP_<PLATFORM>_IMPLEMENTATION_ONCE` guard for the function bodies.

Anything the declaration block exposes to the core (the capability macros, a
`pntr_app_<platform>_platform` struct, the file hook prototypes) must survive the header-only
pass. Anything that needs the platform SDK goes in the implementation block.

## The six required functions

These are declared in `include/pntr_app.h:756-812` and are the entire surface the core calls.
A backend defines all six, with these exact signatures.

### `bool pntr_app_platform_init(pntr_app* app)`

Called from `pntr_app_init()` after the core has defaulted `app->width`/`app->height` and
allocated `app->screen`, and before `PNTR_APP_INIT_AUDIO` and the application's own `init()`.

Must:

- Allocate the backend's private state with `pntr_load_memory()`, zero it, and store it in
  `app->platform`. The core never touches that pointer; only `pntr_app_platform_close()` frees it.
- Create the window or output device at whatever size the platform wants. `app->width` and
  `app->height` are the *pixel buffer* size, not the window size; the SDL backend opens a window
  at double that and scales.
- Open any input devices, e.g. gamepads.
- Seed the random number generator with something platform specific, via
  `pntr_app_random_set_seed(app, seed)`. The core has already seeded it with 0, which asks the
  platform to decide.

Return `true` on success. Returning `false` makes `pntr_app_init()` unload the screen, shut
sokol_args down and return `false`, which makes `main()` return 1. Note that
`pntr_app_platform_close()` is **not** called in that path, so clean up everything the function
allocated before returning `false`. See `pntr_app_platform_init()` in `include/pntr_app_sdl.h`
for the unwind pattern. (If the *application's* `init()` is the thing that fails, the core does
call `PNTR_APP_CLOSE_AUDIO` and `pntr_app_platform_close()` for you.)

### `bool pntr_app_platform_events(pntr_app* app)`

Called once per frame, immediately after `pntr_app_pre_events()` and before `update()`
(`include/pntr_app.h:1031`).

Must drain the platform's input queue, translate each input into a `pntr_app_event` and hand it to
`pntr_app_process_event()`. See [Pushing input](#pushing-input).

Return `true` to keep running. Returning `false` breaks the main loop, and the core then calls
`pntr_app_close()` normally. This is how the CLI backend quits on Ctrl+C, Ctrl+X, Ctrl+Z and
Escape.

A backend that defines `PNTR_APP_NO_ENTRY` supplies its own loop and only needs the functions that
loop actually calls. The SDL backend is the one exception in the tree: it runs on SDL3's
`SDL_AppInit`/`SDL_AppEvent`/`SDL_AppIterate` callbacks and so never defines
`pntr_app_platform_events()` at all. libretro and ESP32 also set `PNTR_APP_NO_ENTRY` but still
define it, because their own loops (`retro_run()` in `include/pntr_app_libretro.h` and
`pntr_app_esp32_loop()` in `include/pntr_app_esp32.h`) call it.

### `bool pntr_app_platform_render(pntr_app* app)`

Called once per frame after `update()`. Must push `app->screen` to the display. The screen is a
`pntr_image`; use `app->screen->data`, `->width`, `->height` and `->pitch`. The pixel order is
chosen for you in `include/pntr_app_platform.h:39-53` — SDL and libretro get
`PNTR_PIXELFORMAT_ARGB`, everything else gets `PNTR_PIXELFORMAT_RGBA`. Add the new platform to
that block if it needs ARGB.

If the output is a different size than the screen, scale with `pntr_app_letterbox()` rather than
rolling your own aspect-ratio maths. See [Scaling and letterboxing](#scaling-and-letterboxing).

Return `true` on success. Returning `false` ends the loop, the same as `_events()`.

### `void pntr_app_platform_close(pntr_app* app)`

Called from `pntr_app_close()` last, after the application's `close()`, after
`PNTR_APP_CLOSE_AUDIO`, and after the core has already unloaded `app->screen`, the arg file data
and the clipboard. Do not dereference `app->screen` here.

Must free `app->platform`, set it to `NULL`, and shut the platform down. Has to tolerate
`app == NULL` and `app->platform == NULL`, because it is also called on the application-`init()`
failure path.

### `bool pntr_app_platform_update_delta_time(pntr_app* app)`

Called once per frame before `update()`. Two jobs:

- Set `app->deltaTime` to the seconds elapsed since the previous update.
- Return whether it is time to run an update frame. Return `false` to skip `update()` and the
  render for this iteration.

`app->fps` is the requested framerate, where 0 means "as fast as possible". The SDL backend
returns `false` until a frame's worth of nanoseconds has passed; the CLI backend does its frame
limiting inside `_events()` with `tb_peek_event()` and simply reports `1.0f / app->fps`.

Do not update the FPS counter here. The loop calls `pntr_app_update_fps()` itself.

### `bool pntr_app_platform_set_size(pntr_app* app, int width, int height)`

Called from `pntr_app_set_size()` (`include/pntr_app.h:1599`) *after* the new backbuffer has been
created but *before* it is swapped in. `app->screen` is still the old buffer at this point.

Must resize the window, texture or render target to accept a `width` x `height` pixel buffer, and
return `true` if the platform can honour it. Returning `false` makes `pntr_app_set_size()` discard
the new backbuffer, leave the screen alone and report failure to the application. A backend that
cannot resize should still return `true` if a `width` x `height` buffer will render acceptably
(the CLI backend does); return `false` only when the new size genuinely will not work.

## Capability macros

`pntr_app_has_feature()` lets an application tell "this platform cannot do that" apart from "that
call silently did nothing". Most features are inferred from the hook macros the backend defined,
but six have no hook to infer from and must be declared in the backend's declaration block:

| Macro | Reports |
| --- | --- |
| `PNTR_APP_HAS_KEYBOARD` | `PNTR_APP_FEATURE_KEYBOARD` |
| `PNTR_APP_HAS_MOUSE` | `PNTR_APP_FEATURE_MOUSE` |
| `PNTR_APP_HAS_GAMEPAD` | `PNTR_APP_FEATURE_GAMEPAD` |
| `PNTR_APP_HAS_FILE_DROPPED` | `PNTR_APP_FEATURE_FILE_DROPPED` |
| `PNTR_APP_HAS_SAVE_LOAD` | `PNTR_APP_FEATURE_SAVE_LOAD` |
| `PNTR_APP_HAS_CHEAT` | `PNTR_APP_FEATURE_CHEAT` |

They are bare `#define`s with no value, declared at the top of the declaration block with a
comment listing what the platform does *not* support — see `include/pntr_app_cli.h:5-9`.

The rest are derived by `pntr_app_has_feature()` in `include/pntr_app.h`, so defining the hook is
all that is needed: `PNTR_APP_FEATURE_SOUND` from `PNTR_APP_PLAY_SOUND`,
`PNTR_APP_FEATURE_VOLUME` from `PNTR_APP_SET_VOLUME`, `PNTR_APP_FEATURE_CLIPBOARD` from
`PNTR_APP_CLIPBOARD` **and** `PNTR_APP_SET_CLIPBOARD` together, `PNTR_APP_FEATURE_TITLE` from
`PNTR_APP_SET_TITLE`, `PNTR_APP_FEATURE_ICON` from `PNTR_APP_SET_ICON`, and
`PNTR_APP_FEATURE_CURSOR` from `PNTR_APP_SHOW_MOUSE`.

## Optional hook macros

Everything else is a macro. Each is defined by the backend with an `#ifndef` guard so that an
application can override it before including `pntr_app.h`. The core tests each with `#ifdef` and
falls back to the behaviour in the last column.

| Macro | Expected signature | When undefined |
| --- | --- | --- |
| `PNTR_APP_LOG` | `void (pntr_app_log_type type, const char* message)` | `printf()`, or `fprintf(stderr)` for `PNTR_APP_LOG_ERROR`; `PNTR_APP_LOG_DEBUG` is dropped under `NDEBUG` |
| `PNTR_APP_SHOW_MOUSE` | `bool (pntr_app* app, bool show)` | `pntr_app_show_mouse()` returns `false` |
| `PNTR_APP_SET_CLIPBOARD` | `void (pntr_app* app, const char* text)` | Text is still copied into `app->clipboard`, but the system clipboard is untouched |
| `PNTR_APP_CLIPBOARD` | `const char* (pntr_app* app)` | `pntr_app_clipboard()` returns whatever was last set on `app->clipboard` |
| `PNTR_APP_SET_TITLE` | `void (pntr_app* app, const char* title)` | `app->title` is updated, the window title is not |
| `PNTR_APP_SET_ICON` | `void (pntr_app* app, pntr_image* icon)` | No-op |
| `PNTR_APP_INIT_AUDIO` | `void (pntr_app* app)` | No audio system is started |
| `PNTR_APP_CLOSE_AUDIO` | `void (pntr_app* app)` | Nothing is torn down |
| `PNTR_APP_LOAD_SOUND_FROM_MEMORY` | `pntr_sound* (pntr_app_sound_type type, unsigned char* data, unsigned int dataSize)` | `pntr_load_sound_from_memory()` returns `NULL`, so `pntr_load_sound()` fails too |
| `PNTR_APP_UNLOAD_SOUND` | `void (pntr_sound* sound)` | No-op |
| `PNTR_APP_PLAY_SOUND` | `void (pntr_sound* sound, bool loop)` | No-op |
| `PNTR_APP_STOP_SOUND` | `void (pntr_sound* sound)` | No-op |
| `PNTR_APP_SET_VOLUME` | `void (pntr_sound* sound, float volume)` | No-op |
| `PNTR_APP_SOUND_PLAYING` | `bool (pntr_sound* sound)` | `pntr_sound_playing()` returns `false` |
| `PNTR_LOAD_FILE` | `unsigned char* (const char* fileName, unsigned int* bytesRead)` | pntr's own `stdio` implementation |
| `PNTR_SAVE_FILE` | `bool (const char* fileName, const void* data, unsigned int bytesToWrite)` | pntr's own `stdio` implementation |

Notes on the ones with contracts beyond the signature:

- `PNTR_APP_LOAD_SOUND_FROM_MEMORY` takes ownership of `data` when it succeeds, and must release
  it with `pntr_unload_file()` when the sound is unloaded. When it returns `NULL`,
  `pntr_load_sound()` unloads the data itself, so do not free it on the failure path.
  `pntr_sound` is an opaque `void`, so the backend returns a pointer to its own struct and casts
  it back in the other sound hooks.
- `PNTR_APP_SET_CLIPBOARD` is called *after* the core has copied the text into `app->clipboard`,
  and is handed `app->clipboard` rather than the caller's string.
- `PNTR_APP_CLIPBOARD` must return newly allocated memory that the core takes ownership of (it
  unloads the previous `app->clipboard`), or `NULL` to leave the cached value alone.
- `PNTR_APP_SET_VOLUME` receives a volume the core has already clamped to `0.0f..1.0f`.
- `PNTR_LOAD_FILE` and `PNTR_SAVE_FILE` belong to pntr, not pntr_app, so they have to be defined
  in the declaration block before `pntr.h` is included — see the top of `include/pntr_app_sdl.h`.

A backend may also define:

- `PNTR_APP_NO_ENTRY` to suppress the core's `int main()` and supply its own loop.
- `PNTR_MALLOC` / `PNTR_FREE` to route pntr's allocations through the platform allocator, as the
  SDL backend does. Keep any third-party library the backend vendors on a single allocator; pntr
  has no `realloc`, which is why the CLI backend hands termbox2 plain `malloc`/`realloc`/`free`.
- Platform-local switches such as `PNTR_APP_DISABLE_TERMBOX`. Document them in the header's top
  comment, as the ESP32 backend does.

## Pushing input

Input is pushed, never polled by the core. The pattern is the same everywhere:

``` c
pntr_app_event event = {0};
event.app = app;
event.type = PNTR_APP_EVENTTYPE_KEY_DOWN;
event.key = PNTR_APP_KEY_SPACE;
pntr_app_process_event(app, &event);
```

`pntr_app_process_event()` updates the input state arrays on `app` and then forwards the event to
the application's `event()` callback if it has one. It stamps `event->app` itself, so setting it
is belt and braces rather than a requirement. Zero-initialise the struct, and reuse one
`pntr_app_event` for every event in a frame the way the CLI and SDL backends do.

### Map platform codes, do not pass them through

`pntr_app_process_event()` range-checks the index fields and **silently drops** anything out of
range:

- `event->key` must be in `[PNTR_APP_KEY_FIRST, PNTR_APP_KEY_LAST)` for key events.
- `event->gamepad` must be in `[0, PNTR_APP_MAX_GAMEPADS)` and `event->gamepadButton` in
  `[PNTR_APP_GAMEPAD_BUTTON_FIRST, PNTR_APP_GAMEPAD_BUTTON_LAST)` for gamepad events.
- `event->mouseButton` must be in `[PNTR_APP_MOUSE_BUTTON_FIRST, PNTR_APP_MOUSE_BUTTON_LAST)` for
  mouse button events.

These fields index fixed-size arrays on `pntr_app`, so a raw scancode, a raw button number or a
device id would write outside the struct. Write explicit mapping functions —
`pntr_app_sdl_key()`, `pntr_app_sdl_gamepad_button()` and `pntr_app_sdl_mouse_button()` in
`include/pntr_app_sdl.h` — and drop anything they cannot map, rather than relying on the range
check to catch it. Device ids need the same treatment: SDL joystick instance ids count up
forever, so the SDL backend keeps its own slot table and translates with
`pntr_app_sdl_gamepad_index()`.

### One `pntr_app_pre_events()` per frame

`pntr_app_pre_events()` rolls the current input state into the "previous frame" state. Without it
`pntr_app_key_pressed()`, `pntr_app_key_released()`, `pntr_app_gamepad_button_pressed()`,
`pntr_app_mouse_button_pressed()` and the mouse wheel never reset, so a single press reads as held
forever.

It must run exactly once per frame. If the backend uses the core's `main()` the core already
handles it (`include/pntr_app.h:1031`). A backend with its own loop calls it itself:

- libretro, at the top of `retro_run()` before `_events()` (`include/pntr_app_libretro.h:751`).
- ESP32, at the top of `pntr_app_esp32_loop()` (`include/pntr_app_esp32.h:1185`).
- SDL, at the **end** of `SDL_AppIterate()` (`include/pntr_app_sdl.h:286`). SDL3 delivers events
  through `SDL_AppEvent()` *before* `SDL_AppIterate()` runs, so rolling the state over at the top
  of iterate would discard the presses `update()` has not seen yet. Any backend whose event
  delivery happens before its frame callback needs the same ordering.

### Absolute vs relative `PNTR_APP_EVENTTYPE_MOUSE_MOVE`

The core decides which kind of movement you reported by looking at the delta fields, in the
`PNTR_APP_EVENTTYPE_MOUSE_MOVE` case of `pntr_app_process_event()`
(`include/pntr_app.h:1315`). This is the only signal; there is no separate event type.

- **Absolute position.** Leave `mouseDeltaX` and `mouseDeltaY` at zero and set `mouseX`/`mouseY`
  to the position in *screen buffer* pixels. The core derives the delta as
  `mouseX - app->mouseX`, assigns the position, and returns early without firing the application
  callback if the position has not actually changed.
- **Relative movement.** Set a non-zero `mouseDeltaX` or `mouseDeltaY`. The core accumulates it
  onto `app->mouseX`/`app->mouseY` and **ignores `mouseX`/`mouseY` entirely**.

So a zero delta means "absolute", not "did not move": do not send a `MOUSE_MOVE` with all four
fields zero expecting it to mean anything. Either way the core clamps the result to
`0..app->width - 1` and `0..app->height - 1` so the position is always a valid pixel, and sets
`app->mouseDeltaX`/`Y` for `pntr_app_mouse_delta_x()`.

Both styles are in use, chosen by whether the cursor is captured:

- SDL checks `SDL_GetWindowRelativeMouseMode()` and reports deltas when captured, otherwise
  unprojected absolute coordinates (`pntr_app_platform_fix_mouse_coordinates()`).
- raylib checks `IsCursorHidden()` and does the same (`pntr_app_raylib_fix_mouse_coordinates()`).
- libretro reports absolute for `RETRO_DEVICE_POINTER` and relative for `RETRO_DEVICE_MOUSE`.
- Web, CLI and ESP32 only ever report absolute positions.

For `PNTR_APP_EVENTTYPE_MOUSE_WHEEL`, set `mouseWheel` to `1` for up and `-1` for down; the core
fills in `mouseX`/`mouseY` from its own state. Same for the mouse button events — do not bother
setting a position on those.

### Scaling and letterboxing

Mouse coordinates are in screen buffer pixels, so a backend whose window is a different size than
`app->screen` has to convert. The core provides both halves, so do not reimplement them:

``` c
float destX, destY, destWidth, destHeight;
pntr_app_letterbox(app->screen->width, app->screen->height, windowWidth, windowHeight,
    &destX, &destY, &destWidth, &destHeight);

// Render app->screen into that rectangle, then for a mouse position in window coordinates:
event.mouseX = windowMouseX;
event.mouseY = windowMouseY;
pntr_app_unproject_mouse(app->screen->width, app->screen->height,
    destX, destY, destWidth, destHeight, &event.mouseX, &event.mouseY);
```

`pntr_app_letterbox()` scales the screen up as far as it fits and centres it, which leaves equal
bars top and bottom or left and right. `pntr_app_unproject_mouse()` converts a window position
back onto the screen, clamping positions that land on a letterbox bar to the nearest edge. The
SDL backend uses both (`pntr_app_platform_get_destination()` and
`pntr_app_platform_fix_mouse_coordinates()`).

## Backend skeleton

Copy this to `include/pntr_app_yourplatform.h` and work through the TODOs. It mirrors the
structure of `include/pntr_app_cli.h`.

``` c
#ifdef PNTR_APP_YOURPLATFORM
#ifndef PNTR_APP_YOURPLATFORM_H__
#define PNTR_APP_YOURPLATFORM_H__

// Declarations. This block is compiled twice: once by the header-only pass in pntr_app.h, and
// once by the implementation pass. Keep it free of anything that needs the platform SDK, and
// do not put function bodies here.

// Platform capabilities, reported by pntr_app_has_feature(). List what's missing in a comment.
// The remaining features are inferred from the hook macros defined further down.
// No gamepad, file drop, save state or cheats.
#define PNTR_APP_HAS_KEYBOARD
#define PNTR_APP_HAS_MOUSE

// pntr configuration. PNTR_LOAD_FILE/PNTR_SAVE_FILE have to be defined before pntr.h is
// included, which is why they live up here. Drop these two if the platform's stdio is fine.
#ifndef PNTR_LOAD_FILE
    #define PNTR_LOAD_FILE pntr_app_yourplatform_load_file
#endif
#ifndef PNTR_SAVE_FILE
    #define PNTR_SAVE_FILE(fileName, data, bytesToWrite) pntr_app_yourplatform_save_file(fileName, data, bytesToWrite)
#endif

/**
 * Private state for the platform, stored in pntr_app::platform.
 */
typedef struct pntr_app_yourplatform_platform {
    // TODO: Window handles, textures, timers, gamepad slots, held input state.
    int unused;
} pntr_app_yourplatform_platform;

unsigned char* pntr_app_yourplatform_load_file(const char* fileName, unsigned int* bytesRead);
bool pntr_app_yourplatform_save_file(const char* fileName, const void* data, unsigned int bytesToWrite);

#endif  // PNTR_APP_YOURPLATFORM_H__

#if defined(PNTR_APP_IMPLEMENTATION) && !defined(PNTR_APP_HEADER_ONLY)
#ifndef PNTR_APP_YOURPLATFORM_IMPLEMENTATION_ONCE
#define PNTR_APP_YOURPLATFORM_IMPLEMENTATION_ONCE

#include <string.h> // memset

// TODO: #include the platform SDK here.

// TODO: Map the platform's codes onto pntr_app's enums. Never pass a raw scancode, button
// number or device id into pntr_app_process_event(); it range-checks and drops them.
static pntr_app_key pntr_app_yourplatform_key(int key) {
    switch (key) {
        // case YOUR_KEY_SPACE: return PNTR_APP_KEY_SPACE;
        default: return PNTR_APP_KEY_INVALID;
    }
}

bool pntr_app_platform_init(pntr_app* app) {
    if (app == NULL) {
        return false;
    }

    // Allocate the private state.
    app->platform = pntr_load_memory(sizeof(pntr_app_yourplatform_platform));
    if (app->platform == NULL) {
        return false;
    }
    memset(app->platform, 0, sizeof(pntr_app_yourplatform_platform));

    // TODO: Open the window/display at whatever size suits the platform. app->width and
    // app->height are the pixel buffer size, so scaling up is fine.
    // TODO: Open input devices.
    // TODO: Start the delta time clock.

    // Seed the random number generator with something platform specific.
    pntr_app_random_set_seed(app, 0 /* TODO: a clock or hardware counter */);

    // TODO: On any failure past this point, undo everything and return false.
    // pntr_app_platform_close() is NOT called when this function fails.
    return true;
}

bool pntr_app_platform_events(pntr_app* app) {
    if (app == NULL || app->platform == NULL) {
        return false;
    }

    pntr_app_yourplatform_platform* platform = (pntr_app_yourplatform_platform*)app->platform;
    (void)platform;

    pntr_app_event event = {0};
    event.app = app;

    // TODO: Drain the platform's input queue. For each input, fill in event and push it:
    //
    //   event.type = PNTR_APP_EVENTTYPE_KEY_DOWN;
    //   event.key = pntr_app_yourplatform_key(raw);
    //   if (event.key != PNTR_APP_KEY_INVALID) {
    //       pntr_app_process_event(app, &event);
    //   }
    //
    // Mouse movement: leave mouseDeltaX/Y at zero and set mouseX/mouseY for an absolute
    // position, or set a non-zero mouseDeltaX/Y for relative movement. Positions are in
    // screen buffer pixels, so run window coordinates through pntr_app_unproject_mouse().

    // Return false when the platform asks to quit.
    return true;
}

bool pntr_app_platform_render(pntr_app* app) {
    if (app == NULL || app->screen == NULL || app->platform == NULL) {
        return false;
    }

    // TODO: Push app->screen->data to the display. Use ->width, ->height and ->pitch. The pixel
    // order comes from include/pntr_app_platform.h; add the platform to that block if it needs
    // PNTR_PIXELFORMAT_ARGB instead of RGBA. Use pntr_app_letterbox() to place the screen if
    // the output is a different size.

    return true;
}

void pntr_app_platform_close(pntr_app* app) {
    // TODO: Shut the platform down. app->screen has already been unloaded by the core, so do
    // not touch it here. Tolerate being called with a half-initialized platform.

    if (app == NULL) {
        return;
    }

    pntr_unload_memory(app->platform);
    app->platform = NULL;
}

bool pntr_app_platform_update_delta_time(pntr_app* app) {
    if (app == NULL || app->platform == NULL) {
        return false;
    }

    // TODO: Set app->deltaTime to the seconds since the previous update, and return whether
    // it's time to run an update frame. app->fps is the requested framerate; 0 means variable.
    app->deltaTime = (app->fps <= 0) ? 0.0f : 1.0f / (float)app->fps;

    return true;
}

bool pntr_app_platform_set_size(pntr_app* app, int width, int height) {
    (void)width;
    (void)height;
    if (app == NULL || app->platform == NULL) {
        return false;
    }

    // TODO: Resize the window/texture to take a width x height pixel buffer. Returning false
    // makes pntr_app_set_size() discard the new backbuffer and keep the old screen.

    return true;
}

// Optional hooks. Each one is guarded with #ifndef so an application can override it, and each
// one defined here also flips the matching pntr_app_has_feature() answer to true.

#ifndef PNTR_APP_LOG
    #define PNTR_APP_LOG pntr_app_yourplatform_log
    void pntr_app_yourplatform_log(pntr_app_log_type type, const char* message) {
        #ifdef NDEBUG
            if (type == PNTR_APP_LOG_DEBUG) {
                return;
            }
        #endif
        // TODO: Route to the platform's logger.
        (void)type;
        (void)message;
    }
#endif

// TODO: Add PNTR_APP_SHOW_MOUSE, PNTR_APP_SET_TITLE, PNTR_APP_SET_ICON,
// PNTR_APP_SET_CLIPBOARD / PNTR_APP_CLIPBOARD and the sound hooks as the platform allows.
// Everything left undefined falls back to the no-op behaviour in the table above.

#endif  // PNTR_APP_YOURPLATFORM_IMPLEMENTATION_ONCE
#endif  // PNTR_APP_IMPLEMENTATION && !PNTR_APP_HEADER_ONLY
#endif  // PNTR_APP_YOURPLATFORM
```

### Wiring it in

1. `include/pntr_app_platform.h`: add `PNTR_APP_YOURPLATFORM` to the "already selected" list at
   lines 5-10, add an auto-detection branch (lines 12-35) keyed off a macro the platform SDK
   defines, and add the platform to the `PNTR_PIXELFORMAT_ARGB` list at lines 39-45 if it needs
   ARGB.
2. `include/pntr_app.h`: add `#include "pntr_app_yourplatform.h"` to **both** include lists, the
   header-only one at lines 833-839 and the implementation one at lines 911-916. Keep them in
   alphabetical order.
3. `include/pntr_app.h`: add `PNTR_APP_YOURPLATFORM` to the `CONFIGURATION` comment at the top of
   the file (lines 10-16).
4. `README.md`: add the platform to the Features list and to the Configuration block.
5. `example/CMakeLists.txt`: add a target modelled on the CLI block at line 106 — an
   `add_executable()` on `pntr_app_example.c`, `target_link_libraries(... pntr pntr_app)`,
   `target_compile_definitions(... PUBLIC PNTR_APP_YOURPLATFORM)`, `C_STANDARD 11` and the
   `-Wall -Wextra -Wpedantic -Werror` block. Gate it behind a new
   `PNTR_APP_BUILD_EXAMPLE_YOURPLATFORM` option added to the root `CMakeLists.txt`.
   Platforms with their own build system live under `example/` instead, like `example/esp32`
   for PlatformIO.
6. Build the example with the new define and check it against
   `-Wall -Wextra -Wpedantic -Werror`. See [CONTRIBUTING.md](../CONTRIBUTING.md).

## Support matrix

Derived from the current headers. "yes" means the backend implements it; a blank means it does
not.

| | CLI | SDL | raylib | Web | libretro | ESP32 |
| --- | --- | --- | --- | --- | --- | --- |
| `pntr_app_platform_init` | yes | yes | yes | yes | yes | yes |
| `pntr_app_platform_events` | yes | callbacks | yes | yes | yes | yes |
| `pntr_app_platform_render` | yes | yes | yes | yes | yes | yes |
| `pntr_app_platform_close` | yes | yes | yes | yes | yes | yes |
| `pntr_app_platform_update_delta_time` | yes | yes | yes | yes | yes | yes |
| `pntr_app_platform_set_size` | yes | yes | yes | yes | yes | yes |
| `PNTR_APP_NO_ENTRY` | | yes | | | yes | yes |
| `PNTR_APP_HAS_KEYBOARD` | yes | yes | yes | yes | yes | |
| `PNTR_APP_HAS_MOUSE` | yes | yes | yes | yes | yes | yes (touch) |
| `PNTR_APP_HAS_GAMEPAD` | | yes | yes | yes | yes | |
| `PNTR_APP_HAS_FILE_DROPPED` | | yes | yes | yes | | |
| `PNTR_APP_HAS_SAVE_LOAD` | | yes | yes | yes | yes | |
| `PNTR_APP_HAS_CHEAT` | | | | | yes | |
| Mouse wheel | yes | yes | yes | yes | yes | |
| Relative mouse movement | | yes | yes | | yes | |
| Save / load trigger | | F5 / F9 | F5 / F9 | JavaScript | `retro_serialize` | |
| `PNTR_APP_LOG` | yes | yes | yes | yes | yes | yes |
| `PNTR_APP_SHOW_MOUSE` | | yes | yes | yes | yes | |
| `PNTR_APP_SET_TITLE` | | yes | yes | yes | | |
| `PNTR_APP_SET_ICON` | | yes | yes | yes | | |
| `PNTR_APP_SET_CLIPBOARD` | | yes | yes | yes | | |
| `PNTR_APP_CLIPBOARD` | | yes | yes | yes | | |
| `PNTR_APP_INIT_AUDIO` | | yes | | | yes | |
| `PNTR_APP_CLOSE_AUDIO` | | yes | | | yes | |
| `PNTR_APP_LOAD_SOUND_FROM_MEMORY` | | yes | yes | yes | yes | |
| `PNTR_APP_UNLOAD_SOUND` | | yes | yes | yes | yes | |
| `PNTR_APP_PLAY_SOUND` | | yes | yes | yes | yes | |
| `PNTR_APP_STOP_SOUND` | | yes | yes | yes | yes | |
| `PNTR_APP_SET_VOLUME` | | yes | yes | yes | yes | |
| `PNTR_APP_SOUND_PLAYING` | | yes | yes | yes | yes | |
| `PNTR_LOAD_FILE` | | yes | yes | | yes | SD only |
| `PNTR_SAVE_FILE` | | yes | yes | | yes | SD only |

Known gaps, so a new backend knows what it is allowed to leave out:

- **CLI and ESP32 define no sound hooks at all.** Every sound call is a silent no-op there and
  `pntr_load_sound()` returns `NULL`.
- **Cheats are libretro-only.** `PNTR_APP_EVENTTYPE_CHEAT` is pushed from
  `include/pntr_app_libretro.h` and nowhere else.
- **File drop is SDL, raylib and web only.**
- **raylib and web have no `PNTR_APP_INIT_AUDIO`/`PNTR_APP_CLOSE_AUDIO`.** raylib calls
  `InitAudioDevice()` and `CloseAudioDevice()` inside its own init and close; the web backend has
  nothing to start.
- **ESP32 has no keyboard or gamepad**, and its touch panel is reported as mouse events, so it
  declares `PNTR_APP_HAS_MOUSE` only.
- **ESP32's `PNTR_LOAD_FILE`/`PNTR_SAVE_FILE` only exist under `PNTR_APP_ESP32_SD`.**
- **CLI and web use pntr's default `stdio` file loading.**
