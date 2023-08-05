# pntr_app

Build [pntr](https://github.com/robloach/pntr) applications with the same code for a number of different targets, including SDL, raylib, libretro, the web, and more.

## Example

``` c
#define PNTR_APP_IMPLEMENTATION
#include "pntr_app.h"

bool Init(void* userData) {
    // Initialize the application, return false on failure.
    return true;
}

bool Update(pntr_image* screen, void* userData) {
    // Clear the screen.
    pntr_clear_background(screen, PNTR_WHITE);

    // Draw a circle.
    pntr_draw_circle_fill(screen, screen->width / 2, screen->height / 2, 100, PNTR_BLUE);

    // Continue running the application.
    return true;
}

void Event(pntr_app_event* event, void* userData) {
    switch (event->type) {
        case PNTR_APP_EVENTTYPE_KEY_DOWN:
            if (event->key == PNTR_APP_KEY_SPACE) {
                // SPACE is pressed!
            }
        break;
    }
}

void Close(void* userData) {
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
        .fps = 60,
        .userData = NULL
    };
}
```

## Features

- [x] Target: [raylib](https://www.raylib.com/)
- [x] Target: [SDL](https://www.libsdl.org/)
- [x] Target: [Emscripten](https://emscripten.org/)
- [x] Target: [libretro](https://www.libretro.com/)
- [x] Target: Command Line Interface with [termbox2](https://github.com/termbox/termbox2)
- [ ] Target: [sokol](https://github.com/floooh/sokol)
- [x] Mouse
- [x] Keyboard
- [x] Gamepads
- [x] Sound (wav, ogg)
- [ ] Music

## Configuration

When compiling, define one of the following to determine which platform you are targeting...
```
PNTR_APP_SDL
PNTR_APP_RAYLIB
PNTR_APP_LIBRETRO
PNTR_APP_CLI
EMSCRIPTEN
```

## Build

There are a few platforms supported by pntr_app, which have their own build methods...

### Desktop

To build the raylib and SDL applications, use [CMake](https://cmake.org/). Depends on either [raylib](https://www.raylib.com/), or [SDL](https://www.libsdl.org/) along with [SDL_mixer](https://github.com/libsdl-org/SDL_mixer)...

``` bash
cmake -B build
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

To build for the web with [Emscripten](https://emscripten.org/). Depends on [emsdk](https://emscripten.org/docs/tools_reference/emsdk.html).

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
