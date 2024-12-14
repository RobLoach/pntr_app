# pntr_app

Build [pntr](https://github.com/robloach/pntr) applications with the same code for a number of different targets, including SDL, raylib, libretro, the web, and more.

## Features

- Compile for a number of platforms...
    - [raylib](https://www.raylib.com/)
    - [SDL2](https://www.libsdl.org/)
    - Web with [Emscripten](https://emscripten.org/)
    - [libretro core](https://www.libretro.com/) and RetroArch
    - Command Line Interfaces with [termbox2](https://github.com/termbox/termbox2)
- Software rendering with [pntr](https://github.com/robloach/pntr)
- Audio (*.wav* or *.ogg*, or more with seperate defines/backends)
- Input with Mouse, Keyboard, or Gamepads

## Example

``` c
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
        .close = Close,
        .fps = 60
    };
}
```

## Configuration

The easiest way is to use our cmake function, which will define the correct stuff for you, and make a nice static lib that can be shared between multiple targets. If you don't want to use cmake, or want something more advanced, check out [ADVANCED](ADVANCED.md).

Here is an example adding it to your project. First copy [Findpntr.cmake](cmake/Findpntr.cmake) into your project, then do this:

```cmake
cmake_minimum_required(VERSION 3.22)
project(myproject)

# find cmake stuff in current dir/cmake
# this is wherever you put Findpntr.cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# this loads the helper
find_package(pntr REQUIRED)

# create myexe with pntr defaults setup
add_executable(myexe src/main.c)
add_pntr(myexe)
```

It will download any dependencies it needs (even pntr & pntr_app) and build a static lib for the window/sound backends and link your program.

If you want to load different backends, you can use the helper to make it easy. Here is a slightly more advanced config that will build a web-page, and a libretro core, and a native app, all from the same source:

```cmake
cmake_minimum_required(VERSION 3.22)
project(myproject)

# find cmake stuff in current dir/cmake
# this is wherever you put Findpntr.cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# this loads the helper
find_package(pntr REQUIRED)

# create myexe with pntr defaults setup
add_executable(myexe src/main.c)
add_pntr(myexe)

if (EMSCRIPTEN)
  # build the web-page in my /docs dir.
  # I set extension  .mjs here, which triggers modern module mode, and allows you to setup your own nicer shell more easily.
  # you can put any other emscripten options you like here
  set_target_properties(myexe
    PROPERTIES
    SUFFIX ".mjs"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/docs"
  )
else()
  # build the libretro core
  add_library(myexe-libretro SHARED src/main.c)
  add_pntr(myexe-libretro RETRO)
  set_target_properties(myexe-libretro PROPERTIES PREFIX "")
endif()
```

In order to use these:

```bash
# configure for native
cmake -B build -G Ninja

# build for native
cmake --build build

# configure for web
emcmake cmake -B wbuild -G Ninja

# build for web
cmake --build wbuild

# run a little web-server on docs/ dir
npx -y live-server docs
```

Here, I use Ninja, because it's much faster, but you can leave off the `-G Ninja` if you want to use your platform's default (`make`/etc.)

It is highly recommended to keep emscritpen & native builds in a seperate roots, since they compile the same-name targets for differnt platforms, which does not really work, in general. Here I use `build/` vs `wbuild/` for that.

## API

``` c
// Application
pntr_app Main(int argc, char* argv[]);
int pntr_app_width(pntr_app* app);
int pntr_app_height(pntr_app* app);
void pntr_app_set_title(pntr_app* app, const char* title);
const char* pntr_app_title(pntr_app* app);
void* pntr_app_userdata(pntr_app* app);
void pntr_app_set_userdata(pntr_app* app, void* userData);
bool pntr_app_set_size(pntr_app* app, int width, int height);
void pntr_app_set_icon(pntr_app* app, pntr_image* icon);
float pntr_app_delta_time(pntr_app* app);

// Input
bool pntr_app_key_pressed(pntr_app* app, pntr_app_key key);
bool pntr_app_key_down(pntr_app* app, pntr_app_key key);
bool pntr_app_key_released(pntr_app* app, pntr_app_key key);
bool pntr_app_key_up(pntr_app* app, pntr_app_key key);
bool pntr_app_gamepad_button_pressed(pntr_app* app, int gamepad, pntr_app_gamepad_button key);
bool pntr_app_gamepad_button_down(pntr_app* app, int gamepad, pntr_app_gamepad_button key);
bool pntr_app_gamepad_button_released(pntr_app* app, int gamepad, pntr_app_gamepad_button key);
float pntr_app_mouse_x(pntr_app* app);
float pntr_app_mouse_y(pntr_app* app);
float pntr_app_mouse_delta_x(pntr_app* app);
float pntr_app_mouse_delta_y(pntr_app* app);
bool pntr_app_mouse_button_pressed(pntr_app* app, pntr_app_mouse_button button);
bool pntr_app_mouse_button_down(pntr_app* app, pntr_app_mouse_button button);
bool pntr_app_mouse_button_released(pntr_app* app, pntr_app_mouse_button button);
bool pntr_app_mouse_button_up(pntr_app* app, pntr_app_mouse_button button);

// Utility
int pntr_app_random(pntr_app* app, int min, int max);
float pntr_app_random_float(pntr_app* app, float min, float max);
uint64_t pntr_app_random_seed(pntr_app* app);
void pntr_app_random_set_seed(pntr_app* app, uint64_t seed);
void pntr_app_log(pntr_app_log_type type, const char* message);
void* pntr_app_load_arg_file(pntr_app* app, unsigned int* size);

// Sounds
pntr_sound* pntr_load_sound(const char* fileName);
pntr_sound* pntr_load_sound_from_memory(pntr_app_sound_type type, unsigned char* data, unsigned int dataSize);
void pntr_unload_sound(pntr_sound* sound);
void pntr_play_sound(pntr_sound* sound, bool loop);
void pntr_stop_sound(pntr_sound* sound);
```

For drawing, see the [pntr API](https://github.com/RobLoach/pntr).


## License

Unless stated otherwise, all works are:

- Copyright (c) 2023 [Rob Loach](https://robloach.net)

... and licensed under:

- [zlib License](LICENSE)
