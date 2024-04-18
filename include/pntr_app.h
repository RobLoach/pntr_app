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
*       PNTR_APP_CLI
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

#ifdef __LIBRETRO__
    #ifndef PNTR_APP_LIBRETRO
        #define PNTR_APP_LIBRETRO
    #endif
#endif

// pntr configuration
#if defined(PNTR_APP_SDL) || defined(PNTR_APP_LIBRETRO)
    #define PNTR_PIXELFORMAT_ARGB
#endif

// pntr.h
#ifndef PNTR_APP_PNTR_H
    #define PNTR_APP_PNTR_H "pntr.h"
#endif
#include PNTR_APP_PNTR_H

// Random Number Generator
#include "external/prand.h"

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
    PNTR_APP_KEY_LAST
} pntr_app_key;

#ifndef PNTR_APP_MAX_GAMEPADS
/**
 * The maximum number of supported gamepads
 */
#define PNTR_APP_MAX_GAMEPADS 4
#endif

/**
 * Gamepad button.
 */
typedef enum pntr_app_gamepad_button {
    PNTR_APP_GAMEPAD_BUTTON_UNKNOWN = 0,        // Unknown button, just for error checking
    PNTR_APP_GAMEPAD_BUTTON_FIRST = 1,
    PNTR_APP_GAMEPAD_BUTTON_UP = 1,             // Gamepad left DPAD up button
    PNTR_APP_GAMEPAD_BUTTON_RIGHT,              // Gamepad left DPAD right button
    PNTR_APP_GAMEPAD_BUTTON_DOWN,               // Gamepad left DPAD down button
    PNTR_APP_GAMEPAD_BUTTON_LEFT,               // Gamepad left DPAD left button
    PNTR_APP_GAMEPAD_BUTTON_Y,                  // Gamepad right button up (i.e. PS3: Triangle, Xbox: Y)
    PNTR_APP_GAMEPAD_BUTTON_B,                  // Gamepad right button right (i.e. PS3: Square, Xbox: X)
    PNTR_APP_GAMEPAD_BUTTON_A,                  // Gamepad right button down (i.e. PS3: Cross, Xbox: A)
    PNTR_APP_GAMEPAD_BUTTON_X,                  // Gamepad right button left (i.e. PS3: Circle, Xbox: B)
    PNTR_APP_GAMEPAD_BUTTON_LEFT_SHOULDER,      // Gamepad top/back trigger left (first), it could be a trailing button
    PNTR_APP_GAMEPAD_BUTTON_LEFT_TRIGGER,       // Gamepad top/back trigger left (second), it could be a trailing button
    PNTR_APP_GAMEPAD_BUTTON_RIGHT_SHOULDER,     // Gamepad top/back trigger right (one), it could be a trailing button
    PNTR_APP_GAMEPAD_BUTTON_RIGHT_TRIGGER,      // Gamepad top/back trigger right (second), it could be a trailing button
    PNTR_APP_GAMEPAD_BUTTON_SELECT,             // Gamepad center buttons, left one (i.e. PS3: Select)
    PNTR_APP_GAMEPAD_BUTTON_MENU,               // Gamepad center buttons, middle one (i.e. PS3: PS, Xbox: XBOX)
    PNTR_APP_GAMEPAD_BUTTON_START,              // Gamepad center buttons, right one (i.e. PS3: Start)
    PNTR_APP_GAMEPAD_BUTTON_LEFT_THUMB,         // Gamepad joystick pressed button left
    PNTR_APP_GAMEPAD_BUTTON_RIGHT_THUMB,        // Gamepad joystick pressed button right
    PNTR_APP_GAMEPAD_BUTTON_LAST
} pntr_app_gamepad_button;

/**
 * Mouse button.
 */
typedef enum pntr_app_mouse_button {
    PNTR_APP_MOUSE_BUTTON_UNKNOWN = 0,
    PNTR_APP_MOUSE_BUTTON_FIRST = 1,
    PNTR_APP_MOUSE_BUTTON_LEFT = 1,
    PNTR_APP_MOUSE_BUTTON_RIGHT = 2,
    PNTR_APP_MOUSE_BUTTON_MIDDLE = 3,
    PNTR_APP_MOUSE_BUTTON_LAST
} pntr_app_mouse_button;

/**
 * A list of events that are passed through pntr_app::event.
 *
 * @see pntr_app::event
 * @see pntr_app_event
 */
typedef enum pntr_app_event_type {
    PNTR_APP_EVENTTYPE_UNKNOWN = 0,
    PNTR_APP_EVENTTYPE_KEY_DOWN,
    PNTR_APP_EVENTTYPE_KEY_UP,
    PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN,
    PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP,
    PNTR_APP_EVENTTYPE_MOUSE_MOVE,
    PNTR_APP_EVENTTYPE_MOUSE_WHEEL,
    PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_DOWN,
    PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_UP,

    /**
     * Evoked when a file is drag and dropped onto the application.
     *
     * @see pntr_app_event::fileDropped
     */
    PNTR_APP_EVENTTYPE_FILE_DROPPED
} pntr_app_event_type;

typedef enum pntr_app_sound_type {
    PNTR_APP_SOUND_TYPE_UNKNOWN = 0,
    PNTR_APP_SOUND_TYPE_WAV,
    PNTR_APP_SOUND_TYPE_OGG
} pntr_app_sound_type;

/**
 * Priority level for logging.
 *
 * @see pntr_app_log()
 */
typedef enum pntr_app_log_type {
    PNTR_APP_LOG_DEBUG = 0,
    PNTR_APP_LOG_INFO,
    PNTR_APP_LOG_WARNING,
    PNTR_APP_LOG_ERROR
} pntr_app_log_type;

typedef struct pntr_app pntr_app;

typedef struct pntr_app_event {
    /**
     * The application associated with the event.
     */
    pntr_app* app;

    /**
     * The type of the event that has been pushed.
     */
    pntr_app_event_type type;

    /**
     * With PNTR_APP_EVENTTYPE_KEY_DOWN or PNTR_APP_EVENTTYPE_KEY_UP, will determine the key that was affected.
     */
    pntr_app_key key;

    /**
     * PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN or PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP
     */
    pntr_app_mouse_button mouseButton;
    float mouseX;
    float mouseY;
    float mouseDeltaX;
    float mouseDeltaY;

    /**
     * When type is PNTR_APP_EVENTTYPE_MOUSE_WHEEL, mouseWheel will be -1 when the mouse wheel is scrolling up, and 1 when scrolling down.
     */
    int mouseWheel;

    pntr_app_gamepad_button gamepadButton;
    int gamepad;

    /**
     * When a file is drag and dropped on the application, this contains the path to the file.
     *
     * @see PNTR_APP_EVENTTYPE_DRAG_AND_DROP
     */
    const char* fileDropped;
} pntr_app_event;

/**
 * Application definition.
 */
struct pntr_app {
    int width;                      // The pixel width of the application.
    int height;                     // The pixel height of the application.
    const char* title;              // The name of the application, which will usually become the window title.
    bool (*init)(pntr_app* app);
    bool (*update)(pntr_app* app, pntr_image* screen);
    void (*close)(pntr_app* app);
    void (*event)(pntr_app* app, pntr_app_event* event);
    int fps;                        // The desired framerate. Use 0 for a variable framerate.
    void* userData;                 // A pointer to a custom state in memory that is passed across all pntr_app callbacks.
    pntr_image* screen;
    void* platform;
    float deltaTime;
    unsigned int deltaTimeCounter;

    // Input state

    // Keyboard
    bool keysChanged;
    bool keysDown[PNTR_APP_KEY_LAST];
    bool keysDownLast[PNTR_APP_KEY_LAST];

    // Gamepad
    int gamepadButtonState[PNTR_APP_MAX_GAMEPADS];
    int gamepadButtonStatePrevious[PNTR_APP_MAX_GAMEPADS];

    // Mouse
    float mouseX;
    float mouseY;
    float mouseDeltaX;
    float mouseDeltaY;
    int mouseWheel;
    bool mouseWheelChanged;
    bool mouseChanged;
    bool mouseButtonsDown[PNTR_APP_MOUSE_BUTTON_LAST];
    bool mouseButtonsDownLast[PNTR_APP_MOUSE_BUTTON_LAST];
    bool mouseButtonsChanged;

    // Command Line Arguments
    int argc;
    char** argv;
    void* argFileData;
    unsigned int argFileDataSize;
    bool argFileDataUnloadOnExit;

    // Random Number Generator
    prand_t prand;
};

typedef void pntr_sound;

/**
 * Load a sound from the given path. Supports .wav or .ogg files.
 *
 * @param fileName The filename of the sound file to load.
 *
 * @return The loaded sound, or NULL on failure.
 */
PNTR_APP_API pntr_sound* pntr_load_sound(const char* fileName);

/**
 * Load a sound from memory. Supports .wav or .ogg files.
 *
 * Will take ownership of the data and clear it when the sound is unloaded.
 *
 * @param type The type of sound that we'll be loading (PNTR_APP_SOUND_TYPE_WAV, PNTR_APP_SOUND_TYPE_WAV, etc).
 * @param data The file data.
 * @param dataSize The size of the data in memory.
 *
 * @return The loaded sound, or NULL on failure.
 * @see PNTR_APP_SOUND_TYPE_WAV
 * @see PNTR_APP_SOUND_TYPE_OGG
 */
PNTR_APP_API pntr_sound* pntr_load_sound_from_memory(pntr_app_sound_type type, unsigned char* data, unsigned int dataSize);

/**
 * Unload the given sound.
 *
 * @param sound The sound to unload.
 */
PNTR_APP_API void pntr_unload_sound(pntr_sound* sound);

/**
 * Play the given sound.
 *
 * TODO: Sounds: Add volume
 *
 * @param sound The sound to play.
 * @param loop Whether or not to loop back to the beginning when the sound finishes.
 */
PNTR_APP_API void pntr_play_sound(pntr_sound* sound, bool loop);

/**
 * Stop playing the given sound.
 *
 * @param sound The sound to stop playing.
 */
PNTR_APP_API void pntr_stop_sound(pntr_sound* sound);

/**
 * Get the sound type of the given file from its file path.
 */
PNTR_APP_API pntr_app_sound_type pntr_app_get_file_sound_type(const char* fileName);

/**
 * Get the user data associated with the application.
 *
 * @param app The application.
 *
 * @return A pointer to the user data associated with the application.
 */
PNTR_APP_API void* pntr_app_userdata(pntr_app* app);

/**
 * Sets the user data for the application.
 */
PNTR_APP_API void pntr_app_set_userdata(pntr_app* app, void* userData);

/**
 * Get the screen width of the application.
 */
PNTR_APP_API int pntr_app_width(pntr_app* app);

/**
 * Get the screen height of the application.
 */
PNTR_APP_API int pntr_app_height(pntr_app* app);

/**
 * Retrieves the change in time in seconds since the last update run.
 */
PNTR_APP_API float pntr_app_delta_time(pntr_app* app);

/**
 * Get a random value between min and max (both included)
 *
 * @param min The minimum value.
 * @param max The maximum value.
 *
 * @return A random integer between the min and max values.
 */
PNTR_APP_API int pntr_app_random(pntr_app* app, int min, int max);

/**
 * Sets the random number generator seed.
 *
 * @note This is automatically called when the application is loading.
 *
 * @param seed The seed to use for the random number generator. If set to 0, will let the platform decide which seed to use.
 */
PNTR_APP_API void pntr_app_random_seed(pntr_app* app, unsigned int seed);

/**
 * Log a message.
 *
 * @param type The type of message to be logged.
 * @param message The message.
 *
 * @see pntr_app_log_type
 */
PNTR_APP_API void pntr_app_log(pntr_app_log_type type, const char* message);
PNTR_APP_API bool pntr_app_key_pressed(pntr_app* app, pntr_app_key key);
PNTR_APP_API bool pntr_app_key_down(pntr_app* app, pntr_app_key key);
PNTR_APP_API bool pntr_app_key_released(pntr_app* app, pntr_app_key key);
PNTR_APP_API bool pntr_app_key_up(pntr_app* app, pntr_app_key key);
PNTR_APP_API bool pntr_app_gamepad_button_pressed(pntr_app* app, int gamepad, pntr_app_gamepad_button key);
PNTR_APP_API bool pntr_app_gamepad_button_down(pntr_app* app, int gamepad, pntr_app_gamepad_button key);
PNTR_APP_API bool pntr_app_gamepad_button_released(pntr_app* app, int gamepad, pntr_app_gamepad_button key);
PNTR_APP_API bool pntr_app_gamepad_button_up(pntr_app* app, int gamepad, pntr_app_gamepad_button key);
PNTR_APP_API float pntr_app_mouse_x(pntr_app* app);
PNTR_APP_API float pntr_app_mouse_y(pntr_app* app);
PNTR_APP_API float pntr_app_mouse_delta_x(pntr_app* app);
PNTR_APP_API float pntr_app_mouse_delta_y(pntr_app* app);
PNTR_APP_API bool pntr_app_mouse_button_pressed(pntr_app* app, pntr_app_mouse_button button);
PNTR_APP_API bool pntr_app_mouse_button_down(pntr_app* app, pntr_app_mouse_button button);
PNTR_APP_API bool pntr_app_mouse_button_released(pntr_app* app, pntr_app_mouse_button button);
PNTR_APP_API bool pntr_app_mouse_button_up(pntr_app* app, pntr_app_mouse_button button);
PNTR_APP_API void pntr_app_set_title(pntr_app* app, const char* title);
PNTR_APP_API const char* pntr_app_title(pntr_app* app);
PNTR_APP_API bool pntr_app_set_size(pntr_app* app, int width, int height);

/**
 * Sets the application's window icon.
 *
 * @param app The application to act on.
 * @param icon The icon for the image.
 */
PNTR_APP_API void pntr_app_set_icon(pntr_app* app, pntr_image* icon);

/**
 * When the application is passed a file to load, this will retrieve the file argument's file data.
 *
 * @note This function can only be called during or after \c init().
 *
 * @param app The application to act on.
 * @param size A pointer to an unsigned int that will represent the size in bytes of the memory buffer.
 *
 * @return A memory buffer for the file data that was passed in. This must be cleared with pntr_unload_file() afterwards.
 */
PNTR_APP_API void* pntr_app_load_arg_file(pntr_app* app, unsigned int* size);

/**
 * Retrieve the clipboard text.
 *
 * @param app The application to act on.
 *
 * @return The clipboard text.
 */
PNTR_APP_API const char* pntr_app_clipboard(pntr_app* app);

/**
 * Set the clipboard text.
 *
 * @param app The application to act on.
 * @param text The text to set the clipboard to.
 */
PNTR_APP_API void pntr_app_set_clipboard(pntr_app* app, const char* text);

/**
 * Platform callback to initialize the platform.
 *
 * @return True if initialization was successful, false otherwise.
 *
 * @internal
 */
bool pntr_app_init(pntr_app* app);

/**
 * Platform callback to invoke all events for the platform.
 *
 * @return True if the application should continue to run, false if the platform is requested to close.
 *
 * @internal
 */
bool pntr_app_events(pntr_app* app);

/**
 * @internal
 */
void pntr_app_pre_events(pntr_app* app);

/**
 * @internal
 */
void pntr_app_process_event(pntr_app* app, pntr_app_event* event);

/**
 * Platform callback to render to the screen.
 *
 * @return True if rendering was successful, false otherwise.
 *
 * @internal
 */
bool pntr_app_render(pntr_app* app);

/**
 * Platform callback to close the application.
 *
 * @internal
 */
void pntr_app_close(pntr_app* app);


/**
 * Asks the platform to update the delta time, and indicates if it's time to run an update.
 *
 * @return True of false depending on if it's time to run an update frame.
 *
 * @internal
 */
bool pntr_app_platform_update_delta_time(pntr_app* app);

/**
 * @internal
 */
bool _pntr_app_platform_set_size(pntr_app* app, int width, int height);

#ifdef PNTR_ENABLE_VARGS
PNTR_APP_API void pntr_app_log_ex(pntr_app_log_type type, const char* message, ...);
#endif

#ifdef __cplusplus
}
#endif

#endif  // PNTR_APP_H__

#ifdef PNTR_APP_IMPLEMENTATION
#ifndef PNTR_APP_IMPLEMENTATION_ONCE
#define PNTR_APP_IMPLEMENTATION_ONCE

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

// Platform
#if defined(PNTR_APP_SDL)
    #include "pntr_app_sdl.h"
#elif defined(PNTR_APP_RAYLIB)
    #include "pntr_app_raylib.h"
#elif defined(PNTR_APP_LIBRETRO)
    #include "pntr_app_libretro.h"
#elif defined(PNTR_APP_WEB)
    #include "pntr_app_web.h"
#elif defined(PNTR_APP_CLI)
    #include "pntr_app_cli.h"
#else
    #error "[pntr_app] No target found. Define PNTR_APP_SDL, PNTR_APP_CLI, PNTR_APP_RAYLIB, or PNTR_APP_LIBRETRO."
#endif

#ifdef __cplusplus
}
#endif

#define PNTR_IMPLEMENTATION
#ifndef PNTR_APP_PNTR_H
#define PNTR_APP_PNTR_H "pntr.h"
#endif
#include PNTR_APP_PNTR_H

#ifndef PNTR_APP_LOG
    #include <stdio.h> // printf(), sprintf()
#endif

// prand: Pseudo Random Number Generator
#ifndef PRAND_MALLOC
    #define PRAND_MALLOC(sz) PNTR_MALLOC(sz)
#endif
#ifndef PRAND_FREE
    #define PRAND_FREE(obj) PNTR_FREE(obj)
#endif
#define PRAND_IMPLEMENTATION
#include "external/prand.h"

/**
 * Retrieve a bit flag for the given button.
 */
#define PNTR_APP_GAMEPAD_BUTTON_FLAG(button) (1 << (button))

#ifdef __cplusplus
extern "C" {
#endif

// Whether or not the application uses int main().
#ifndef PNTR_APP_NO_ENTRY

#ifdef EMSCRIPTEN
#ifndef PNTR_APP_EMSCRIPTEN_H
#define PNTR_APP_EMSCRIPTEN_H <emscripten/emscripten.h>
#endif  // PNTR_APP_EMSCRIPTEN_H
#include PNTR_APP_EMSCRIPTEN_H

/**
 * The update callback for web.
 */
void pntr_app_emscripten_update_loop(void* application) {
    if (application == NULL) {
        emscripten_cancel_main_loop();
        return;
    }

    pntr_app* app = (pntr_app*)application;

    // Poll Events
    pntr_app_pre_events(app);
    if (!pntr_app_events(app)) {
        emscripten_cancel_main_loop();
        return;
    }

    // Check the update function.
    if (app->update == NULL) {
        emscripten_cancel_main_loop();
        return;
    }

    // Run the update function.
    if (pntr_app_platform_update_delta_time(app)) {
        if (app->update(app, app->screen) == false) {
            emscripten_cancel_main_loop();
            return;
        }
    }

    // Render
    if (!pntr_app_render(app)) {
        emscripten_cancel_main_loop();
    }
}
#endif

/**
 * The main entry point of the application.
 *
 * Will call the Main() function.
 *
 * @see PNTR_APP_MAIN
 */
int main(int argc, char* argv[]) {
    pntr_app app = PNTR_APP_MAIN(argc, argv);

    app.argc = argc;
    app.argv = argv;

    app.screen = pntr_gen_image_color(app.width, app.height, PNTR_BLACK);
    if (app.screen == NULL) {
        app.init = NULL;
        app.update = NULL;
        app.close = NULL;
    }
    else if (pntr_app_init(&app) == false) {
        pntr_unload_image(app.screen);
        return 1;
    }

    // Call the init callback.
    if (app.init != NULL) {
        // Check if initialization worked.
        if (app.init(&app) == false) {
            // Skip calling the other callbacks if it failed.
            app.update = NULL;
            app.close = NULL;
        }
    }

    // Start the update loop
    if (app.update != NULL) {
        #if defined(EMSCRIPTEN)
            // Set up the main loop.
            emscripten_set_main_loop_arg(pntr_app_emscripten_update_loop, &app, 0, 1);
        #else
            // Continue running when update returns TRUE.
            do {
                // Events
                pntr_app_pre_events(&app);
                if (!pntr_app_events(&app)) {
                    break;
                }

                // Update
                if (pntr_app_platform_update_delta_time(&app)) {
                    if (!app.update(&app, app.screen)) {
                        break;
                    }
                }

                // Render
                if (!pntr_app_render(&app)) {
                    break;
                }
            } while(true);
        #endif
    }

    // Tell the application to close.
    if (app.close != NULL) {
        app.close(&app);
    }

    // Tell the platform to close.
    pntr_app_close(&app);

    // Unload any other associated memory
    pntr_unload_image(app.screen);
    if (app.argFileDataUnloadOnExit) {
        pntr_unload_memory(app.argFileData);
        app.argFileData = NULL;
    }

    // Return an error state if update was nullified.
    return (app.update == NULL) ? 1 : 0;
}
#endif  // PNTR_APP_NO_ENTRY

PNTR_APP_API pntr_app_sound_type pntr_app_get_file_sound_type(const char* fileName) {
    if (strstr(fileName, ".wav")) {
        return PNTR_APP_SOUND_TYPE_WAV;
    }

    if (strstr(fileName, ".ogg")) {
        return PNTR_APP_SOUND_TYPE_OGG;
    }

    return PNTR_APP_SOUND_TYPE_UNKNOWN;
}

PNTR_APP_API pntr_sound* pntr_load_sound(const char* fileName) {
    pntr_app_sound_type type = pntr_app_get_file_sound_type(fileName);
    if (type == PNTR_APP_SOUND_TYPE_UNKNOWN) {
        return pntr_set_error(PNTR_ERROR_NOT_SUPPORTED);
    }

    unsigned int bytesRead;
    unsigned char* data = pntr_load_file(fileName, &bytesRead);
    if (data == NULL) {
        return NULL;
    }

    return pntr_load_sound_from_memory(type, data, bytesRead);
}

PNTR_APP_API inline void* pntr_app_userdata(pntr_app* app) {
    return app->userData;
}

PNTR_APP_API inline int pntr_app_width(pntr_app* app) {
    if (app->screen == NULL) {
        return app->width;
    }

    return app->screen->width;
}

PNTR_APP_API inline int pntr_app_height(pntr_app* app) {
    if (app->screen == NULL) {
        return app->height;
    }

    return app->screen->height;
}

PNTR_APP_API inline float pntr_app_delta_time(pntr_app* app) {
    return app->deltaTime;
}

PNTR_APP_API inline void pntr_app_set_userdata(pntr_app* app, void* userData) {
    if (app == NULL) {
        return;
    }
    app->userData = userData;
}

void pntr_app_process_event(pntr_app* app, pntr_app_event* event) {
    switch (event->type) {
        case PNTR_APP_EVENTTYPE_KEY_DOWN:
            app->keysDown[event->key] = true;
            app->keysChanged = true;
            break;
        case PNTR_APP_EVENTTYPE_KEY_UP:
            app->keysDown[event->key] = false;
            app->keysChanged = true;
            break;
        case PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_DOWN:
            app->gamepadButtonState[event->gamepad] |= PNTR_APP_GAMEPAD_BUTTON_FLAG(event->gamepadButton);
            break;
        case PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_UP:
            app->gamepadButtonState[event->gamepad] &= ~PNTR_APP_GAMEPAD_BUTTON_FLAG(event->gamepadButton);
            break;
        case PNTR_APP_EVENTTYPE_MOUSE_MOVE:
            event->mouseDeltaX = app->mouseX - event->mouseX;
            event->mouseDeltaY = app->mouseY - event->mouseY;
            if (event->mouseDeltaX == 0 && event->mouseDeltaY == 0) {
                return;
            }
            app->mouseDeltaX = event->mouseDeltaX;
            app->mouseDeltaY = event->mouseDeltaY;
            app->mouseX = event->mouseX;
            app->mouseY = event->mouseY;
            app->mouseChanged = true;
            break;
        case PNTR_APP_EVENTTYPE_MOUSE_WHEEL:
            app->mouseWheel = event->mouseWheel;
            app->mouseWheelChanged = true;
            event->mouseX = app->mouseX;
            event->mouseX = app->mouseX;
            break;
        case PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN:
            app->mouseButtonsDown[event->mouseButton] = true;
            app->mouseButtonsChanged = true;
            event->mouseX = app->mouseX;
            event->mouseX = app->mouseX;
            break;
        case PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP:
            app->mouseButtonsDown[event->mouseButton] = false;
            app->mouseButtonsChanged = true;
            event->mouseX = app->mouseX;
            event->mouseX = app->mouseX;
            break;
        default:
            // Not covered here.
            break;
    }

    if (app->event != NULL) {
        app->event(app, event);
    }
}

void pntr_app_pre_events(pntr_app* app) {
    if (app->keysChanged) {
        // Move the active keys to the last keys.
        for (int i = PNTR_APP_KEY_FIRST; i < PNTR_APP_KEY_LAST; i++) {
            app->keysDownLast[i] = app->keysDown[i];
        }
        app->keysChanged = false;
    }

    for (int i = 0; i < PNTR_APP_MAX_GAMEPADS; i++) {
        app->gamepadButtonStatePrevious[i] = app->gamepadButtonState[i];
    }

    if (app->mouseWheelChanged) {
        app->mouseWheel = 0;
        app->mouseWheelChanged = false;
    }

    if (app->mouseButtonsChanged) {
        for (int i = PNTR_APP_MOUSE_BUTTON_FIRST; i < PNTR_APP_MOUSE_BUTTON_LAST; i++) {
            app->mouseButtonsDownLast[i] = app->mouseButtonsDown[i];
        }
        app->mouseButtonsChanged = false;
    }

    if (app->mouseChanged) {
        app->mouseDeltaX = 0;
        app->mouseDeltaY = 0;
        app->mouseChanged = false;
    }
}

PNTR_APP_API bool pntr_app_key_pressed(pntr_app* app, pntr_app_key key) {
    return app->keysDown[key] && !app->keysDownLast[key];
}

PNTR_APP_API bool pntr_app_key_down(pntr_app* app, pntr_app_key key) {
    return app->keysDown[key];
}

PNTR_APP_API bool pntr_app_key_released(pntr_app* app, pntr_app_key key) {
    return !app->keysDown[key] && app->keysDownLast[key];
}

PNTR_APP_API bool pntr_app_key_up(pntr_app* app, pntr_app_key key) {
    return !app->keysDown[key];
}

PNTR_APP_API bool pntr_app_gamepad_button_pressed(pntr_app* app, int gamepad, pntr_app_gamepad_button button) {
    if (app == NULL) {
        return false;
    }

    if (gamepad <= -1) {
        for (int i = 0; i < PNTR_APP_MAX_GAMEPADS; i++) {
            if ((app->gamepadButtonStatePrevious[i] & PNTR_APP_GAMEPAD_BUTTON_FLAG(button)) == 0 &&
			    (app->gamepadButtonState[i] & PNTR_APP_GAMEPAD_BUTTON_FLAG(button)) != 0) {
                return true;
            }
        }

        return false;
    }

    if (gamepad >= PNTR_APP_MAX_GAMEPADS) {
        return false;
    }

    return ((app->gamepadButtonStatePrevious[gamepad] & PNTR_APP_GAMEPAD_BUTTON_FLAG(button)) == 0 &&
			(app->gamepadButtonState[gamepad] & PNTR_APP_GAMEPAD_BUTTON_FLAG(button)) != 0);
}

PNTR_APP_API bool pntr_app_gamepad_button_down(pntr_app* app, int gamepad, pntr_app_gamepad_button button) {
    if (app == NULL) {
        return false;
    }

    if (gamepad <= -1) {
        for (int i = 0; i < PNTR_APP_MAX_GAMEPADS; i++) {
            if ((app->gamepadButtonState[i] & PNTR_APP_GAMEPAD_BUTTON_FLAG(button)) != 0) {
                return true;
            }
        }

        return false;
    }

    if (gamepad >= PNTR_APP_MAX_GAMEPADS) {
        return false;
    }

    return (app->gamepadButtonState[gamepad] & PNTR_APP_GAMEPAD_BUTTON_FLAG(button)) != 0;
}

PNTR_APP_API bool pntr_app_gamepad_button_released(pntr_app* app, int gamepad, pntr_app_gamepad_button button) {
    if (app == NULL) {
        return false;
    }

    if (gamepad <= -1) {
        for (int i = 0; i < PNTR_APP_MAX_GAMEPADS; i++) {
            if ((app->gamepadButtonState[i] & PNTR_APP_GAMEPAD_BUTTON_FLAG(button)) == 0 &&
			    (app->gamepadButtonStatePrevious[i] & PNTR_APP_GAMEPAD_BUTTON_FLAG(button)) != 0) {
                return true;
            }
        }

        return false;
    }

    if (gamepad >= PNTR_APP_MAX_GAMEPADS) {
        return false;
    }

    return ((app->gamepadButtonState[gamepad] & PNTR_APP_GAMEPAD_BUTTON_FLAG(button)) == 0 &&
			(app->gamepadButtonStatePrevious[gamepad] & PNTR_APP_GAMEPAD_BUTTON_FLAG(button)) != 0);
}

PNTR_APP_API bool pntr_app_gamepad_button_up(pntr_app* app, int gamepad, pntr_app_gamepad_button button) {
    return !pntr_app_gamepad_button_down(app, gamepad, button);
}

PNTR_APP_API float pntr_app_mouse_x(pntr_app* app) {
    return app->mouseX;
}

PNTR_APP_API float pntr_app_mouse_y(pntr_app* app) {
    return app->mouseY;
}

PNTR_APP_API float pntr_app_mouse_delta_x(pntr_app* app) {
    return app->mouseDeltaX;
}

PNTR_APP_API float pntr_app_mouse_delta_y(pntr_app* app) {
    return app->mouseDeltaY;
}

PNTR_APP_API int pntr_app_mouse_wheel(pntr_app* app) {
    return app->mouseWheel;
}

PNTR_APP_API bool pntr_app_mouse_button_pressed(pntr_app* app, pntr_app_mouse_button button) {
    return app->mouseButtonsDown[button] && !app->mouseButtonsDownLast[button];
}

PNTR_APP_API bool pntr_app_mouse_button_down(pntr_app* app, pntr_app_mouse_button button) {
    return app->mouseButtonsDown[button];
}

PNTR_APP_API bool pntr_app_mouse_button_released(pntr_app* app, pntr_app_mouse_button button) {
    return !app->mouseButtonsDown[button] && app->mouseButtonsDownLast[button];
}
PNTR_APP_API bool pntr_app_mouse_button_up(pntr_app* app, pntr_app_mouse_button button) {
    return !pntr_app_mouse_button_down(app, button);
}

/**
 * Change the size of the screen.
 *
 * @param app The application to act on.
 * @param width The desired width of the screen.
 * @param height The desired height of the screen.
 */
PNTR_APP_API bool pntr_app_set_size(pntr_app* app, int width, int height) {
    if (app == NULL || width <= 0 || height <= 0) {
        return false;
    }

    // Request that the platform resizes the window.
    if (!_pntr_app_platform_set_size(app, width, height)) {
        return false;
    }

    // Resize the internal screen canvas.
    if (!pntr_image_resize_canvas(app->screen, width, height, 0, 0, PNTR_BLACK)) {
        return false;
    }

    app->width = app->screen->width;
    app->height = app->screen->height;

    return true;
}

void* pntr_app_load_arg_file(pntr_app* app, unsigned int* size) {
    if (app == NULL) {
        return NULL;
    }

    if (app->argFileData != NULL) {
        // Copy the memory as an output, as the application is now responsible to unload it.
        void* output = pntr_load_memory(app->argFileDataSize);
        pntr_memory_copy(output, app->argFileData, app->argFileDataSize);
        if (size != NULL) {
            *size = app->argFileDataSize;
        }
        return output;
    }

    // TODO: pntr_app_load_arg_file: Parse the argv correctly so that it grabs an actual file path.
    if (app->argv[1] != NULL) {
        unsigned int loadedSize;
        void* output = pntr_load_file(app->argv[1], &loadedSize);
        if (size != NULL) {
            *size = loadedSize;
        }
        return output;
    }

    return NULL;
}

PNTR_APP_API void pntr_app_log(pntr_app_log_type type, const char* message) {
#ifdef PNTR_APP_LOG
    PNTR_APP_LOG(type, message);
#else
    switch (type) {
        case PNTR_APP_LOG_ERROR:
            fprintf(stderr, "%s\n", message);
        break;
        case PNTR_APP_LOG_DEBUG:
            // Skip debug messages if NDEBUG is defined.
            #ifndef NDEBUG
                printf("%s\n", message);
            #endif
        break;
        default:
            printf("%s\n", message);
        break;
    }
#endif
}

#ifdef PNTR_ENABLE_VARGS
PNTR_APP_API void pntr_app_log_ex(pntr_app_log_type type, const char* message, ...) {
    #ifndef PNTR_APP_LOG_EX_STRING_LENGTH
    #define PNTR_APP_LOG_EX_STRING_LENGTH 256
    #endif
    char output[PNTR_APP_LOG_EX_STRING_LENGTH];

    va_list arg_ptr;
    va_start(arg_ptr, message);
    vsprintf(output, message, arg_ptr);
    va_end(arg_ptr);

    pntr_app_log(type, output);
}
#endif

PNTR_APP_API const char* pntr_app_title(pntr_app* app) {
    if (app == NULL) {
        return NULL;
    }

    return app->title;
}

PNTR_APP_API inline int pntr_app_random(pntr_app* app, int min, int max) {
    return prand_int(&app->prand, min, max);
}

PNTR_APP_API void pntr_app_random_seed(pntr_app* app, unsigned int seed) {
    prand_set_seed(&app->prand, seed);
}

#ifdef __cplusplus
}
#endif

#endif  // PNTR_APP_IMPLEMENTATION_ONCE
#endif  // PNTR_APP_IMPLEMENTATION
