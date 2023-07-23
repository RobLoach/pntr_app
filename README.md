# pntr_app

Build [pntr](https://github.com/robloach/pntr) applications with the same code for a number of different targets, including SDL, raylib, the web, and more.

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
        .close = Close,
        .fps = 60,
        .userData = NULL
    };
}
```

## Configuration

When compiling, define one of the following to determine which platform you are targeting...
```
PNTR_APP_SDL
PNTR_APP_RAYLIB
PNTR_APP_LIBRETRO
EMSCRIPTEN
```

Make sure to link the related library when building.

## Features

- [x] Target: [raylib](https://www.raylib.com/)
- [x] Target: [SDL](https://www.libsdl.org/)
- [x] Target: [Emscripten](https://emscripten.org/)
- [x] Target: [libretro](https://www.libretro.com/)
- [ ] Target: [WASI](https://github.com/WebAssembly/wasi-sdk)
- [ ] Target: [GLFW](https://www.glfw.org/)
- [ ] Target: [sokol](https://github.com/floooh/sokol)
- [ ] Target: Command Line Interface?
- [x] Mouse
- [x] Keyboard
- [ ] Gamepads
- [ ] Audio?

## Build

pntr_app is a header-only library, so you can use any build method you would like. Building the examples though, you can use [CMake](https://cmake.org/)...

### Build

``` bash
mkdir build
cd build
cmake ..
make
```

### Emscripten

``` bash
mkdir build
cd build
emcmake cmake ..
emmake make
```

## License

Unless stated otherwise, all works are:

- Copyright (c) 2023 [Rob Loach](https://robloach.net)

... and licensed under:

- [zlib License](LICENSE)