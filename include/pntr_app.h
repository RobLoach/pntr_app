/**********************************************************************************************
*
*   pntr_app - Manage a pntr application for a number of different platforms.
*
*   Copyright 2023 Rob Loach (@RobLoach)
*
*   DEPENDENCIES:
*       pntr https://github.com/robloach/pntr
*
*   CONFIGURATION:
*       PNTR_APP_RAYLIB
*       PNTR_APP_SDL
*       PNTR_APP_LIBRETRO
*       EMSCRIPTEN
*
*   LICENSE: zlib/libpng
*
*   pntr_app is licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software:
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#ifndef PNTR_APP_H__
#define PNTR_APP_H__

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// pntr configuration
#ifdef PNTR_APP_SDL
#define PNTR_PIXELFORMAT_ARGB
#elif PNTR_APP_LIBRETRO
#define PNTR_PIXELFORMAT_ARGB
#endif

#ifndef PNTR_APP_PNTR_H
#define PNTR_APP_PNTR_H "pntr.h"
#endif
#include PNTR_APP_PNTR_H

#ifndef PNTR_APP_API
    #define PNTR_APP_API PNTR_API
#endif

/**
 * Key code.
 */
typedef enum pntr_app_key {
    PNTR_APP_KEY_INVALID          = 0,
    PNTR_APP_KEY_FIRST            = 32,
    PNTR_APP_KEY_SPACE            = 32,
    PNTR_APP_KEY_APOSTROPHE       = 39,  /* ' */
    PNTR_APP_KEY_COMMA            = 44,  /* , */
    PNTR_APP_KEY_MINUS            = 45,  /* - */
    PNTR_APP_KEY_PERIOD           = 46,  /* . */
    PNTR_APP_KEY_SLASH            = 47,  /* / */
    PNTR_APP_KEY_0                = 48,
    PNTR_APP_KEY_1                = 49,
    PNTR_APP_KEY_2                = 50,
    PNTR_APP_KEY_3                = 51,
    PNTR_APP_KEY_4                = 52,
    PNTR_APP_KEY_5                = 53,
    PNTR_APP_KEY_6                = 54,
    PNTR_APP_KEY_7                = 55,
    PNTR_APP_KEY_8                = 56,
    PNTR_APP_KEY_9                = 57,
    PNTR_APP_KEY_SEMICOLON        = 59,  /* ; */
    PNTR_APP_KEY_EQUAL            = 61,  /* = */
    PNTR_APP_KEY_A                = 65,
    PNTR_APP_KEY_B                = 66,
    PNTR_APP_KEY_C                = 67,
    PNTR_APP_KEY_D                = 68,
    PNTR_APP_KEY_E                = 69,
    PNTR_APP_KEY_F                = 70,
    PNTR_APP_KEY_G                = 71,
    PNTR_APP_KEY_H                = 72,
    PNTR_APP_KEY_I                = 73,
    PNTR_APP_KEY_J                = 74,
    PNTR_APP_KEY_K                = 75,
    PNTR_APP_KEY_L                = 76,
    PNTR_APP_KEY_M                = 77,
    PNTR_APP_KEY_N                = 78,
    PNTR_APP_KEY_O                = 79,
    PNTR_APP_KEY_P                = 80,
    PNTR_APP_KEY_Q                = 81,
    PNTR_APP_KEY_R                = 82,
    PNTR_APP_KEY_S                = 83,
    PNTR_APP_KEY_T                = 84,
    PNTR_APP_KEY_U                = 85,
    PNTR_APP_KEY_V                = 86,
    PNTR_APP_KEY_W                = 87,
    PNTR_APP_KEY_X                = 88,
    PNTR_APP_KEY_Y                = 89,
    PNTR_APP_KEY_Z                = 90,
    PNTR_APP_KEY_LEFT_BRACKET     = 91,  /* [ */
    PNTR_APP_KEY_BACKSLASH        = 92,  /* \ */
    PNTR_APP_KEY_RIGHT_BRACKET    = 93,  /* ] */
    PNTR_APP_KEY_GRAVE_ACCENT     = 96,  /* ` */
    PNTR_APP_KEY_WORLD_1          = 161, /* non-US #1 */
    PNTR_APP_KEY_WORLD_2          = 162, /* non-US #2 */
    PNTR_APP_KEY_ESCAPE           = 256,
    PNTR_APP_KEY_ENTER            = 257,
    PNTR_APP_KEY_TAB              = 258,
    PNTR_APP_KEY_BACKSPACE        = 259,
    PNTR_APP_KEY_INSERT           = 260,
    PNTR_APP_KEY_DELETE           = 261,
    PNTR_APP_KEY_RIGHT            = 262,
    PNTR_APP_KEY_LEFT             = 263,
    PNTR_APP_KEY_DOWN             = 264,
    PNTR_APP_KEY_UP               = 265,
    PNTR_APP_KEY_PAGE_UP          = 266,
    PNTR_APP_KEY_PAGE_DOWN        = 267,
    PNTR_APP_KEY_HOME             = 268,
    PNTR_APP_KEY_END              = 269,
    PNTR_APP_KEY_CAPS_LOCK        = 280,
    PNTR_APP_KEY_SCROLL_LOCK      = 281,
    PNTR_APP_KEY_NUM_LOCK         = 282,
    PNTR_APP_KEY_PRINT_SCREEN     = 283,
    PNTR_APP_KEY_PAUSE            = 284,
    PNTR_APP_KEY_F1               = 290,
    PNTR_APP_KEY_F2               = 291,
    PNTR_APP_KEY_F3               = 292,
    PNTR_APP_KEY_F4               = 293,
    PNTR_APP_KEY_F5               = 294,
    PNTR_APP_KEY_F6               = 295,
    PNTR_APP_KEY_F7               = 296,
    PNTR_APP_KEY_F8               = 297,
    PNTR_APP_KEY_F9               = 298,
    PNTR_APP_KEY_F10              = 299,
    PNTR_APP_KEY_F11              = 300,
    PNTR_APP_KEY_F12              = 301,
    PNTR_APP_KEY_F13              = 302,
    PNTR_APP_KEY_F14              = 303,
    PNTR_APP_KEY_F15              = 304,
    PNTR_APP_KEY_F16              = 305,
    PNTR_APP_KEY_F17              = 306,
    PNTR_APP_KEY_F18              = 307,
    PNTR_APP_KEY_F19              = 308,
    PNTR_APP_KEY_F20              = 309,
    PNTR_APP_KEY_F21              = 310,
    PNTR_APP_KEY_F22              = 311,
    PNTR_APP_KEY_F23              = 312,
    PNTR_APP_KEY_F24              = 313,
    PNTR_APP_KEY_F25              = 314,
    PNTR_APP_KEY_KP_0             = 320,
    PNTR_APP_KEY_KP_1             = 321,
    PNTR_APP_KEY_KP_2             = 322,
    PNTR_APP_KEY_KP_3             = 323,
    PNTR_APP_KEY_KP_4             = 324,
    PNTR_APP_KEY_KP_5             = 325,
    PNTR_APP_KEY_KP_6             = 326,
    PNTR_APP_KEY_KP_7             = 327,
    PNTR_APP_KEY_KP_8             = 328,
    PNTR_APP_KEY_KP_9             = 329,
    PNTR_APP_KEY_KP_DECIMAL       = 330,
    PNTR_APP_KEY_KP_DIVIDE        = 331,
    PNTR_APP_KEY_KP_MULTIPLY      = 332,
    PNTR_APP_KEY_KP_SUBTRACT      = 333,
    PNTR_APP_KEY_KP_ADD           = 334,
    PNTR_APP_KEY_KP_ENTER         = 335,
    PNTR_APP_KEY_KP_EQUAL         = 336,
    PNTR_APP_KEY_LEFT_SHIFT       = 340,
    PNTR_APP_KEY_LEFT_CONTROL     = 341,
    PNTR_APP_KEY_LEFT_ALT         = 342,
    PNTR_APP_KEY_LEFT_SUPER       = 343,
    PNTR_APP_KEY_RIGHT_SHIFT      = 344,
    PNTR_APP_KEY_RIGHT_CONTROL    = 345,
    PNTR_APP_KEY_RIGHT_ALT        = 346,
    PNTR_APP_KEY_RIGHT_SUPER      = 347,
    PNTR_APP_KEY_MENU             = 348,
    PNTR_APP_KEY_LAST             = 349
} pntr_app_key;

typedef enum pntr_app_gamepad_button {
    PNTR_APP_GAMEPAD_BUTTON_UNKNOWN = 0,         // Unknown button, just for error checking
    PNTR_APP_GAMEPAD_BUTTON_LEFT_FACE_UP,        // Gamepad left DPAD up button
    PNTR_APP_GAMEPAD_BUTTON_LEFT_FACE_RIGHT,     // Gamepad left DPAD right button
    PNTR_APP_GAMEPAD_BUTTON_LEFT_FACE_DOWN,      // Gamepad left DPAD down button
    PNTR_APP_GAMEPAD_BUTTON_LEFT_FACE_LEFT,      // Gamepad left DPAD left button
    PNTR_APP_GAMEPAD_BUTTON_RIGHT_FACE_UP,       // Gamepad right button up (i.e. PS3: Triangle, Xbox: Y)
    PNTR_APP_GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,    // Gamepad right button right (i.e. PS3: Square, Xbox: X)
    PNTR_APP_GAMEPAD_BUTTON_RIGHT_FACE_DOWN,     // Gamepad right button down (i.e. PS3: Cross, Xbox: A)
    PNTR_APP_GAMEPAD_BUTTON_RIGHT_FACE_LEFT,     // Gamepad right button left (i.e. PS3: Circle, Xbox: B)
    PNTR_APP_GAMEPAD_BUTTON_LEFT_TRIGGER_1,      // Gamepad top/back trigger left (first), it could be a trailing button
    PNTR_APP_GAMEPAD_BUTTON_LEFT_TRIGGER_2,      // Gamepad top/back trigger left (second), it could be a trailing button
    PNTR_APP_GAMEPAD_BUTTON_RIGHT_TRIGGER_1,     // Gamepad top/back trigger right (one), it could be a trailing button
    PNTR_APP_GAMEPAD_BUTTON_RIGHT_TRIGGER_2,     // Gamepad top/back trigger right (second), it could be a trailing button
    PNTR_APP_GAMEPAD_BUTTON_MIDDLE_LEFT,         // Gamepad center buttons, left one (i.e. PS3: Select)
    PNTR_APP_GAMEPAD_BUTTON_MIDDLE,              // Gamepad center buttons, middle one (i.e. PS3: PS, Xbox: XBOX)
    PNTR_APP_GAMEPAD_BUTTON_MIDDLE_RIGHT,        // Gamepad center buttons, right one (i.e. PS3: Start)
    PNTR_APP_GAMEPAD_BUTTON_LEFT_THUMB,          // Gamepad joystick pressed button left
    PNTR_APP_GAMEPAD_BUTTON_RIGHT_THUMB          // Gamepad joystick pressed button right
} pntr_app_gamepad_button;

typedef enum pntr_app_mouse_button {
    PNTR_APP_MOUSE_BUTTON_LEFT,
    PNTR_APP_MOUSE_BUTTON_RIGHT,
    PNTR_APP_MOUSE_BUTTON_MIDDLE,
    PNTR_APP_MOUSE_BUTTON_UNKNOWN
} pntr_app_mouse_button;

typedef enum pntr_app_event_type {
    PNTR_APP_EVENTTYPE_UNKNOWN = 0,
    PNTR_APP_EVENTTYPE_KEY_DOWN,
    PNTR_APP_EVENTTYPE_KEY_UP,
    PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN,
    PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP,
    PNTR_APP_EVENTTYPE_MOUSE_MOVE,
    PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_DOWN,
    PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_UP
} pntr_app_event_type;

typedef struct pntr_app_event {
    /**
     * The type of the event that has been pushed.
     */
    pntr_app_event_type type;

    /**
     * With PNTR_APP_EVENTTYPE_KEY_DOWN or PNTR_APP_EVENTTYPE_KEY_UP, will determine the key that was affected.
     */
    pntr_app_key key;
    pntr_app_mouse_button mouseButton;
    int mouseX;
    int mouseY;

    pntr_app_gamepad_button gamepadButton;
    int gamepad;
} pntr_app_event;

typedef struct pntr_app {
    int width;
    int height;
    const char* title;
    bool (*init)(void* userData);
    bool (*update)(pntr_image* screen, void* userData);
    void (*close)(void* userData);
    void (*event)(pntr_app_event* event, void* userData);
    int fps;
    pntr_image* screen;
    void* userData;
    void* platform;
} pntr_app;

bool pntr_app_init(pntr_app* app);
bool pntr_app_events(pntr_app* app);
bool pntr_app_render(pntr_app* app);
void pntr_app_close(pntr_app* app);

#ifdef __cplusplus
}
#endif

#endif  // PNTR_APP_H__

#ifdef PNTR_APP_IMPLEMENTATION
#ifndef PNTR_APP_IMPLEMENTATION_ONCE
#define PNTR_APP_IMPLEMENTATION_ONCE

#include <stddef.h>

#ifndef PNTR_APP_MAIN
/**
 * The name of the entry point in your application.
 *
 * Will default to "Main".
 *
 * @code
 * #define PNTR_APP_MAIN MyOwnMain
 * #define PNTR_APP_IMPLEMENTATION
 * #include "pntr_app.h"
 *
 * pntr_app MyOwnMain(int argc, char* argv[]) {
 *   // Stuff
 * }
 * @endcode
 **/
#define PNTR_APP_MAIN Main
#endif  // PNTR_APP_MAIN

#ifdef __cplusplus
extern "C" {
#endif

pntr_app PNTR_APP_MAIN(int argc, char* argv[]);

#if defined(PNTR_APP_SDL)
#include "pntr_app_sdl.h"
#elif defined(PNTR_APP_RAYLIB)
#include "pntr_app_raylib.h"
#elif defined(PNTR_APP_LIBRETRO)
#include "pntr_app_libretro.h"
#elif defined(EMSCRIPTEN)
#include "pntr_app_emscripten.h"
#else
#error "[pntr_app] No target found. Set PNTR_APP_SDL, PNTR_APP_RAYLIB, PNTR_APP_LIBRETRO, or EMSCRIPTEN."
#endif

#ifdef __cplusplus
}
#endif

#define PNTR_IMPLEMENTATION
#include PNTR_APP_PNTR_H

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(PNTR_APP_NO_ENTRY)
/**
 * The main entry point of the application.
 *
 * Will call the Main() function.
 *
 * @see PNTR_APP_MAIN
 */
int main(int argc, char* argv[]) {
    pntr_app app = PNTR_APP_MAIN(argc, argv);

    app.screen = pntr_gen_image_color(app.width, app.height, PNTR_BLACK);
    if (app.screen == NULL) {
        app.init = NULL;
        app.update = NULL;
        app.close = NULL;
    }
    else if (pntr_app_init(&app) == false) {
        pntr_app_close(&app);
        pntr_unload_image(app.screen);
        return 1;
    }

    // Call the init callback.
    if (app.init != NULL) {
        // Check if initialization worked.
        if (app.init(app.userData) == false) {
            // Skip calling the other callbacks if it failed.
            app.update = NULL;
            app.close = NULL;
        }
    }

    // Start the update loop
    if (app.update != NULL) {
#if defined(EMSCRIPTEN)
        // Set up the main loop.
        emscripten_set_main_loop_arg(pntr_app_update_loop, &app, app.fps, 1);
#else
        // Continue running when update returns TRUE.
        while (pntr_app_events(&app) &&
            app.update(app.screen, app.userData) &&
            pntr_app_render(&app));
#endif
    }

    // Tell the application to close.
    if (app.close != NULL) {
        app.close(app.userData);
    }

    // Clear the screen
    pntr_unload_image(app.screen);

    pntr_app_close(&app);

    // Clear up any user data.
    pntr_unload_memory(app.userData);

    // Clear up any user data.
    pntr_unload_memory(app.platform);

    // Return an error state if update was nullified.
    return (app.update == NULL) ? 1 : 0;
}
#endif // PNTR_APP_NO_ENTRY

#ifdef __cplusplus
}
#endif

#endif  // PNTR_APP_IMPLEMENTATION_ONCE
#endif  // PNTR_APP_IMPLEMENTATION
