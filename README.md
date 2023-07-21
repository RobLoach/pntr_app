# pntr_app

Manages a pntr application for a number of different platforms.

## Targets

- SDL
- raylib
- Web (emscripten)

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