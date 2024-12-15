# Recipes

These are specific combos/workflows to use pntr_app on different types of hardware/libraries.


## raylib

This is very simple & runs well on most things. It is the default if you use our `add_pntr` cmake function, and it can support regular desktop apps and DRM (modern linux console, with acceleration, especially newer Pi devices, like pizero2w or pi4/5.)

For Windows, Mac, and Linux (pi with X, included):

```cmake
add_pntr(your_target)
```

For Linux with DRM-acceleration (no X, on regular linux with DRM acceleration, and devices like Pi4/5/Zero2 or things like Ambernic/etc)

```cmake
add_pntr(your_target RAYLIB_DRM)
```

For Linux, on older pi-devices, like pi1/2/3, with no X:

```
add_pntr(your_target RAYLIB_OLDPI)
```

You can also define anything you want, from [raylib headers](https://github.com/raysan5/raylib), in combination with using `add_pntr`. Have a look in [ADVANCED](ADVANCED.md) for more info.


## SDL2

This is another library that can target a lot of devices. Currently SDL2 sound is tied to SDL2 window, since it needs it to tset things up.

We default to raylib, since it's simpler, more versatile, and generally faster, but you might want to target some older device, or have SDL2 already installed or something.

```
add_pntr(your_target SDL2)
```


Eventually, I will add lots more here.
