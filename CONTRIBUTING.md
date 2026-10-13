# Contributing

## Layout

pntr_app is header-only, so everything that ships lives in `include/`:

- `include/pntr_app.h` is the core: the `pntr_app` struct, input state, sounds, and the main loop.
- `include/pntr_app_<platform>.h` is one backend per platform.
- `include/pntr_app_platform.h` picks the backend when none was defined.
- `include/external/` is vendored third-party code. Do not hand-edit it; update it from upstream
  and record the change. See [include/external/README.md](include/external/README.md).

`example/` and `test/` are not installed. They exist to prove the headers compile and work.

## Build

Desktop builds use [CMake](https://cmake.org/). This builds the SDL, raylib, libretro and CLI
examples plus the test:

``` bash
cmake -B build
cmake --build build
```

Turn off anything you do not have the dependencies for, for example
`-DPNTR_APP_BUILD_EXAMPLE_SDL=false`. The options are listed in `CMakeLists.txt`.

Web, with [Emscripten](https://emscripten.org/):

``` bash
emcmake cmake -B build -DPNTR_APP_BUILD_EXAMPLE_RAYLIB=false -DPNTR_APP_BUILD_EXAMPLE_WEB=true
emmake make -C build
```

ESP32, with [PlatformIO](https://platformio.org/):

``` bash
cd example/esp32
pio run -e cyd
```

## Tests

``` bash
cmake -B build
cmake --build build
cd build && ctest -C Debug
```

`test/pntr_app_test.c` is a CLI application (`PNTR_APP_CLI` with `PNTR_APP_DISABLE_TERMBOX`) that
drives the API and checks the results with plain `PNTR_ASSERT`. There is no test framework and
nothing to install. A failing assert aborts the process and ctest reports the failure.

Assertions are compiled out under `NDEBUG`, so a release build of the test passes
unconditionally. Build and run the test as `Debug`.

When fixing a bug, add the case to `test/pntr_app_test.c` first. The test runs on CI, nothing
else does.

## Style

`.editorconfig` is authoritative: 4-space indentation, LF line endings, UTF-8, trailing
whitespace trimmed, final newline. Most editors pick this up on their own.

Targets are built as C11 and must be clean under `-Wall -Wextra -Wpedantic -Werror`
(`/W4 /WX` on MSVC). The test additionally uses `-Wconversion -Wsign-conversion
-Wfloat-conversion`, so be explicit about casts.

Beyond that, match the surrounding code: `pntr_app_` prefixes on everything public,
`PNTR_APP_` on macros, Doxygen `/** */` comments on anything an application calls, and
`#ifndef` guards around every macro a user might want to override.

## Adding a platform

See [docs/platforms.md](docs/platforms.md). It documents the six functions a backend has to
define, the optional hook macros and what the core falls back to without them, how input is
pushed, and includes a skeleton header plus the checklist for wiring it into `include/pntr_app.h`
and `example/CMakeLists.txt`.

## Pull requests

Keep the change focused and say what platforms you built and tested it on. Changes to the core
need a sanity check on more than one backend, since the CLI build is the only one CI covers.
