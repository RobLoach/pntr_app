# API

This document covers the parts of the pntr_app API that need more than a signature. For the full
list of functions, see the [README](../README.md#api), and for rendering, see the
[pntr API](https://github.com/RobLoach/pntr).

## Options

Declare options on the `pntr_app` returned from `Main()`. The values are separated by a `|`, and
the first one is the default. On libretro they're registered as core options, and
`PNTR_APP_EVENTTYPE_OPTIONS_CHANGED` is pushed when the user changes one. Everywhere else,
`pntr_app_option()` reports the default.

``` c
static const pntr_app_option_definition options[] = {
    { "myapp_difficulty", "Difficulty", "normal|hard|easy" },
    { "myapp_lives", "Lives", "3|1|2|5" },
    { "myapp_speed", "Speed", "1.0|0.5|2.0" },
    { "myapp_music", "Music", "enabled|disabled" },
};

pntr_app Main(int argc, char* argv[]) {
    return (pntr_app) {
        // ...
        .options = options,
        .optionsCount = 4
    };
}

// In Init(), or on PNTR_APP_EVENTTYPE_OPTIONS_CHANGED...
const char* difficulty = pntr_app_option(app, "myapp_difficulty");
int lives = pntr_app_option_int(app, "myapp_lives");
float speed = pntr_app_option_float(app, "myapp_speed");
bool music = pntr_app_option_bool(app, "myapp_music");
```

The array must outlive the application, as libretro calls `Main()` from
`retro_set_environment()` to register the options before the content is loaded.

| Function | Returns | Missing key |
| --- | --- | --- |
| `pntr_app_option()` | The value as a string. Don't free it. | `NULL` |
| `pntr_app_option_int()` | An optional sign followed by digits, like `atoi()` | `0` |
| `pntr_app_option_float()` | An optional sign, digits and a decimal point | `0.0f` |
| `pntr_app_option_bool()` | `true` for `true`, `enabled`, `on`, `yes` or `1`, ignoring case | `false` |
