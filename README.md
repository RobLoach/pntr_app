# pntr_app

Build [pntr](https://github.com/robloach/pntr) applications with the same code for a number of different targets, including SDL, raylib, libretro, the web, and more.

## Features

- Compile for a number of platforms...
    - [raylib](https://www.raylib.com/)
    - [SDL](https://www.libsdl.org/)
    - Web with [Emscripten](https://emscripten.org/)
    - [libretro](https://www.libretro.com/) and RetroArch
    - Command Line Interfaces with [termbox2](https://github.com/termbox/termbox2)
- Software rendering with [pntr](https://github.com/robloach/pntr)
- Audio (*.wav* or *.ogg*)
- User input...
    - Mouse
    - Keyboard
    - Gamepads

## Example

``` c
#define PNTR_APP_IMPLEMENTATION
#include "pntr_app.h"

bool Init(pntr_app* app) {
    // Initialize the application, return false on failure.
    return true;
}

bool Update(pntr_app* app, pntr_image* screen) {
    // Clear the screen.
    pntr_clear_background(screen, PNTR_WHITE);

    // Draw a circle.
    pntr_draw_circle_fill(screen, screen->width / 2, screen->height / 2, 100, PNTR_BLUE);

    // Continue running the application.
    return true;
}

void Event(pntr_app* app, pntr_app_event* event) {
    switch (event->type) {
        case PNTR_APP_EVENTTYPE_KEY_DOWN:
            if (event->key == PNTR_APP_KEY_SPACE) {
                // SPACE is pressed!
            }
        break;
    }
}

void Close(pntr_app* app) {
    // Uninitialize the application.
}

pntr_app Main(int argc, char* argv[]) {
    return (pntr_app) {
        .width = 800,
        .height = 450,
        .title = "pntr_app",
        .init = Init,
        .update = Update,
        .event = Event,
        .close = Close,
        .fps = 60
    };
}
```

## Configuration

When compiling, define one of the following to determine which platform you are targeting...
```
PNTR_APP_SDL
PNTR_APP_RAYLIB
PNTR_APP_LIBRETRO
PNTR_APP_CLI
EMSCRIPTEN
```

## API

``` c
pntr_app Main(int argc, char* argv[]);
PNTR_APP_API pntr_sound* pntr_load_sound(const char* fileName);
PNTR_APP_API pntr_sound* pntr_load_sound_from_memory(const char* fileName, unsigned char* data, unsigned int dataSize);
PNTR_APP_API void pntr_unload_sound(pntr_sound* sound);
PNTR_APP_API void pntr_play_sound(pntr_sound* sound, bool loop);
PNTR_APP_API void pntr_stop_sound(pntr_sound* sound);
PNTR_APP_API void* pntr_app_userdata(pntr_app* app);
PNTR_APP_API void pntr_app_set_userdata(pntr_app* app, void* userData);
PNTR_APP_API int pntr_app_width(pntr_app* app);
PNTR_APP_API int pntr_app_height(pntr_app* app);
PNTR_APP_API float pntr_app_delta_time(pntr_app* app);
PNTR_APP_API bool pntr_app_key_pressed(pntr_app* app, pntr_app_key key);
PNTR_APP_API bool pntr_app_key_down(pntr_app* app, pntr_app_key key);
PNTR_APP_API bool pntr_app_key_released(pntr_app* app, pntr_app_key key);
PNTR_APP_API bool pntr_app_key_up(pntr_app* app, pntr_app_key key);
PNTR_APP_API bool pntr_app_gamepad_button_pressed(pntr_app* app, int gamepad, pntr_app_gamepad_button key);
PNTR_APP_API bool pntr_app_gamepad_button_down(pntr_app* app, int gamepad, pntr_app_gamepad_button key);
PNTR_APP_API bool pntr_app_gamepad_button_released(pntr_app* app, int gamepad, pntr_app_gamepad_button key);
PNTR_APP_API bool pntr_app_gamepad_button_up(pntr_app* app, int gamepad, pntr_app_gamepad_button key);
PNTR_APP_API int pntr_app_mouse_x(pntr_app* app);
PNTR_APP_API int pntr_app_mouse_y(pntr_app* app);
PNTR_APP_API int pntr_app_mouse_delta_x(pntr_app* app);
PNTR_APP_API int pntr_app_mouse_delta_y(pntr_app* app);
PNTR_APP_API bool pntr_app_mouse_button_pressed(pntr_app* app, pntr_app_mouse_button button);
PNTR_APP_API bool pntr_app_mouse_button_down(pntr_app* app, pntr_app_mouse_button button);
PNTR_APP_API bool pntr_app_mouse_button_released(pntr_app* app, pntr_app_mouse_button button);
PNTR_APP_API bool pntr_app_mouse_button_up(pntr_app* app, pntr_app_mouse_button button);
PNTR_APP_API void pntr_app_set_title(pntr_app* app, const char* title);
PNTR_APP_API bool pntr_app_set_size(pntr_app* app, int width, int height);
PNTR_APP_API void* pntr_app_file_data(pntr_app* app, unsigned int* size, bool unloadOnExit);
```

## Build

There are a few platforms supported by pntr_app, which have their own build methods...

### Desktop

To build the raylib and SDL applications, use [CMake](https://cmake.org/). Depends on either [raylib](https://www.raylib.com/), or [SDL](https://www.libsdl.org/) along with [SDL_mixer](https://github.com/libsdl-org/SDL_mixer)...

``` bash
cmake -B build
cmake --build build
```

You can disable building some examples by using...

``` bash
cmake -B build -DPNTR_APP_BUILD_EXAMPLE_SDL=false -DPNTR_APP_BUILD_EXAMPLE_RAYLIB=false
cmake --build build
```

### libretro

To build the libretro core, use `make`. Depends on [libretro-common](https://github.com/libretro/libretro-common).

``` bash
git submodule update --init
cd example
make
```

#### libretro WASM

``` bash
cd example
emmake make platform=emscripten
```

### Web

Build for the web with [Emscripten](https://emscripten.org/) and raylib. Depends on [emsdk](https://emscripten.org/docs/tools_reference/emsdk.html).

``` bash
emcmake cmake -B build -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS="-s USE_GLFW=3" -DCMAKE_EXECUTABLE_SUFFIX=".html"
emmake make -C build
emrun build/example/index.html
```

## License

Unless stated otherwise, all works are:

- Copyright (c) 2023 [Rob Loach](https://robloach.net)

... and licensed under:

- [zlib License](LICENSE)
