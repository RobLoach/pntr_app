This is help for more advanced topics, with pntr_app.

Before diving into this stuff, have a look at the [README](README.md) and get familiar with the basics, and make sure it doesn't already solve your user-case.

### Backend Drivers

pntr_app is built with the idea of window and sound drivers. You can use a differnt cmake setup, or no cmake at all, if you use the defines that trigger different drivers to tell the headers what backends to load.

Window drivers are the frame of the app, like input, the event loop, and copying the pntr_image to the screen. Sound drivers make the sound work, and are mostly independant of the window (although SDL2 currently requires them to match.) You should only define 1 window/sound per build-target.

Even if you are not using raylib, but need a sound engine, it's recommended to use `PNTR_APP_SOUND_RAYLIB`, since it's just a ncie wrapper around [miniaudio](https://miniaud.io/).

Here are the defines that make that work:

```c
// tells pntr_app to include the implementation of it's functions
// do this exactly once in the build-target
#define PNTR_APP_IMPLEMENTATION

// use raylib window/sound
#define PNTR_APP_WINDOW_RAYLIB
#define PNTR_APP_SOUND_RAYLIB

// use SDL2. These are sort of tied together for now
#define PNTR_APP_WINDOW_SDL2
#define PNTR_APP_SOUND_SDL2

// use emscritpen-based web-only drivers
#define PNTR_APP_WINDOW_WEB
#define PNTR_APP_SOUND_WEB

// make a libretro core. THese are sort of tied togehter for now
#define PNTR_APP_WINDOW_RETRO
#define PNTR_APP_SOUND_RETRO

// don't use a driver. You probably don't want this, but it will allow you to disable sound, for example.
#define PNTR_APP_WINDOW_NONE
#define PNTR_APP_SOUND_NONE

// nice TUI driver, no sound (so use PNTR_APP_SOUND_RAYLIB if you need that)
#define PNTR_APP_WINDOW_TERMBOX

// basic CLI driver, no sound (so use PNTR_APP_SOUND_RAYLIB if you need that)
#define PNTR_APP_WINDOW_CLI
```

With cmake or make, you can put this in your C code, or just add the defines to the command-line:

```
cmake -B build -DPNTR_APP_WINDOW_SDL2 -DPNTR_APP_SOUND_SDL2
```


### No Cmake

You might not want to use cmake, for whatever reason, even though I think it's great. No prob!

Essentially, you need to make sure you've got [pntr](https://github.com/robloach/pntr_app), [pntr_app](https://github.com/robloach/pntr_app), and the dependencies for your window/sound driver in your library & include paths, and any necessary defines set. Here is an example, where I use system-installed raylib headers/library:

```
# build my app
gcc -Ivendor/pntr -Ivendor/pntr_app/include $(pkg-config --cflags --libs raylib) mycode.c -o myapp
```

In `mycode.c` you should have some definitions setup, to get drivers working:

```c
// this tells pntr_app what drivers to load
#define PNTR_APP_WINDOW_RAYLIB
#define PNTR_APP_SOUND_RAYLIB

// this tell pntr to define actual implementations
#define PNTR_APP_IMPLEMENTATION
#include "pntr_app.h"
```

`PNTR_APP_WINDOW_RAYLIB` requires `raylib.h` somewhere in your include-path, and `PNTR_APP_SOUND_RAYLIB` requires `raudio.h` (an miniaudio.h, and a few others that are included with raylib.) In this exmaple, if you are using raylib audio, but not raylib window, you can get away with only referencing [raudio](https://github.com/raysan5/raudio). In addition to headers, you will need to make sure all the C files that implement that stuff are in the build (it will give you errors at link-time telling you what you need.)

# Other Advamced Topics

You can define anything you like, or set up the lib, in any way you want to customize how it. An example is that the raylib audio backend can play lots of other filetypes than just WAV & OGG (like XM/MOD files, and even supports FLAC, MP3 and more!) You can read more about it in [raudio](https://github.com/raysan5/raudio/blob/master/src/raudio.c).

Here is an example of modifying the entry-point, but still using the cmake helper:

```cmake
cmake_minimum_required(VERSION 3.22)
project(myproject)

# find cmake stuff in current dir/cmake
# this is wherever you put Findpntr.cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# this loads the helper
find_package(pntr REQUIRED)

# this is your own C file that sets up the pntr_app entry-point
# do this before calling add_pntr
set(PNTR_APP_ENTRY "src/pntr_fullsound.c")

# create myexe with raylib pntr_app stuff
add_executable(myexe src/main.c)
add_pntr(myexe RAYLIB)
```

Then put this in your own `src/pntr_fullsound.c`:

```c
// support all the raylib sound stuff!
#define SUPPORT_FILEFORMAT_WAV
#define SUPPORT_FILEFORMAT_OGG
#define SUPPORT_FILEFORMAT_MP3
#define SUPPORT_FILEFORMAT_QOA
#define SUPPORT_FILEFORMAT_FLAC
#define SUPPORT_FILEFORMAT_XM
#define SUPPORT_FILEFORMAT_MOD

// all pntr_app stuff has this
#define PNTR_APP_IMPLEMENTATION
#include <pntr_app.h>
```

You can do anything you want in this file, it's just the entry-point that gets built into the static-lib for you, by the `add_pntr` helper.

If you are not using `add_pntr`, just do similar in your own code, to ensure the correct stuff is defined before it loads raylib sound.

This illustrates that you can just override any defines that the backend driver supports, so you will probably need to look into the backend you are using (read it's headers, etc) to do more with this.
