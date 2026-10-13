# include/external

Vendored third-party headers. These are copies of upstream projects, kept here so that pntr_app
stays dependency-free. Do not hand-edit them. To change one, take the new version from upstream
and re-apply any pntr_app-specific configuration through the macros the header already exposes.

Every licence below is the one the file declares in its own header comment.

| File | Project | Upstream | Licence |
| --- | --- | --- | --- |
| `termbox2.h` | termbox2 | https://github.com/termbox/termbox2 (*) | MIT. Copyright (c) 2015-2026 Adam Saponara, 2010-2020 nsf. |
| `sokol_args.h` | sokol | https://github.com/floooh/sokol | zlib/libpng. Copyright (c) 2018 Andre Weissflog. |
| `pico_b64.h` | pico_headers | https://github.com/empyreanx/pico_headers (*) | MIT. Copyright (c) 2022 James McLean, (c) 2014 Little Star Media, Inc. (Joseph Werle). |
| `prand.h` | prand | https://github.com/RobLoach/prand | zlib/libpng. Copyright (c) 2026 Ramon Santamaria (@raysan5), Rob Loach (@RobLoach). |
| `emscripten_clipboard.h` | emscripten_clipboard | https://github.com/RobLoach/emscripten_clipboard | zlib/libpng. Copyright 2024 Rob Loach (@RobLoach). |

Notes:

- `sokol_args.h`, `prand.h` and `emscripten_clipboard.h` state their own project URL in the
  header comment. The two marked (*) do not, so those URLs are not taken from the vendored file:
    - `termbox2.h` declares only the MIT licence and `TB_VERSION_STR "2.7.0-dev"`.
    - `pico_b64.h` declares the MIT licence and describes itself as a repackaged, modified version
      of [b64.c](https://github.com/littlstar/b64.c) by Joseph Werle, which is the only URL it
      gives for its own code. It ships as part of the pico_headers collection.
- `prand.h` notes that it is built on raylib's
  [`rprand.h`](https://github.com/raysan5/raylib/blob/master/src/external/rprand.h), which is why
  Ramon Santamaria holds the first copyright.
