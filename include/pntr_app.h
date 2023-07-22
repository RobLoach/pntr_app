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

typedef struct pntr_app {
    int width;
    int height;
    const char* title;
    bool (*init)(void* userData);
    bool (*update)(pntr_image* screen, void* userData);
    void (*close)(void* userData);
    int fps;
    void* userData;
    pntr_image* screen;
} pntr_app;

bool pntr_app_render(pntr_image* screen);
bool pntr_app_init(pntr_app* app);
void pntr_app_close(pntr_app* app);

#ifdef __cplusplus
}
#endif

#endif  // PNTR_APP_H__

#ifdef PNTR_APP_IMPLEMENTATION
#ifndef PNTR_APP_IMPLEMENTATION_ONCE
#define PNTR_APP_IMPLEMENTATION_ONCE

#define PNTR_IMPLEMENTATION
#include PNTR_APP_PNTR_H

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

pntr_app PNTR_APP_MAIN(int argc, char* argv[]);

// emscripten.h
#if defined(EMSCRIPTEN)
#ifndef PNTR_APP_EMSCRIPTEN_H
#define PNTR_APP_EMSCRIPTEN_H "emscripten.h"
#endif
#include PNTR_APP_EMSCRIPTEN_H
#endif  // EMSCRIPTEN

#ifdef __cplusplus
extern "C" {
#endif

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
        app.update = NULL;
        app.close = NULL;
    }

    if (pntr_app_init(&app) == false) {
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
        emscripten_set_main_loop_arg(pntr_app_update, &app, app.fps, 1);
#else
        // Continue running when update returns TRUE.
        while(app.update(app.screen, app.userData) == true) {
            if (!pntr_app_render(app.screen)) {
                break;
            }
        };
#endif
    }

    // Tell the application to close.
    if (app.close != NULL) {
        app.close(app.userData);
    }

    // Clear the screen
    if (app.screen != NULL) {
        pntr_unload_image(app.screen);
        app.screen = NULL;
    }

    // Clear up any user data.
    if (app.userData != NULL) {
        PNTR_FREE(app.userData);
    }

    pntr_app_close(&app);

    // Return an error state if update was nullified.
    return (app.update == NULL) ? 1 : 0;
}
#endif // PNTR_APP_NO_ENTRY

#ifdef __cplusplus
}
#endif

#endif  // PNTR_APP_IMPLEMENTATION_ONCE
#endif  // PNTR_APP_IMPLEMENTATION