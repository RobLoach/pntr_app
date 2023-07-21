# pntr_app

Build [pntr](https://github.com/robloach/pntr) applications for a number of different targets, including SDL, raylib, the web, and more. Use the same code across the different platforms.

## Example

The following will build and run with SDL, raylib, or on the web.

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
EMSCRIPTEN
```

## Features

- [x] Target: raylib
- [x] Target: SDL
- [x] Target: Emscripten
- [ ] Target: libretro
- [ ] Target: WASI
- [ ] Target: GLFW
- [ ] Target: sokol
- [ ] Target: Command Line Interface?
- [ ] Input API
- [ ] Audio?

## Build

### Desktop

``` bash
mkdir build
cd build
cmake ..
make
```

### Web

``` bash
emcc examples/pntr_app_example.c -o build/index.html --preload-file examples/resources@/resources --shell-file examples/shell.html -Ibuild/_deps/pntr-src -Iinclude
```

## License

Unless stated otherwise, all works are:

- Copyright (c) 2023 [Rob Loach](https://robloach.net)

... and licensed under:

- [zlib License](LICENSE)