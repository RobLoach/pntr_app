#ifdef PNTR_APP_RSGL
#ifndef PNTR_APP_RSGL_H__
#define PNTR_APP_RSGL_H__

typedef struct pntr_app_rsgl_platform {
    int mouseX;
    int mouseY;
    bool keysEnabled[PNTR_APP_KEY_LAST];
    bool mouseButtonsPressed[PNTR_APP_MOUSE_BUTTON_LAST];
} pntr_app_rsgl_platform;

#endif  // PNTR_APP_RSGL_H__

#if defined(PNTR_APP_IMPLEMENTATION) && !defined(PNTR_APP_HEADER_ONLY)
#ifndef PNTR_APP_RSGL_IMPLEMENTATION_ONCE
#define PNTR_APP_RSGL_IMPLEMENTATION_ONCE

// TODO: is this the correct way to do this?
#define RSGL_IMPLEMENTATION
#define RSGL_NO_STB_IMAGE_IMP
#include "RSGL.h"

bool pntr_app_platform_events(pntr_app* app) {
    if (app == NULL || app->platform == NULL) {
        return false;
    }

    // TODO
    printf("events\n");

    return true;
}

/**
 * Pushes the given image to the screen.
 */
bool pntr_app_platform_render(pntr_app* app) {
    if (app == NULL || app->screen == NULL) {
        return false;
    }

    // TODO
    printf("render\n");

    return true;
}

bool pntr_app_platform_init(pntr_app* app) {
    if (app == NULL) {
        return false;
    }

    // TODO
    printf("init\n");

    return true;
}

void pntr_app_platform_close(pntr_app* app) {
    if (app == NULL) {
        return;
    }
    printf("close\n");

    pntr_unload_memory(app->platform);
    app->platform = NULL;
}

// TODO: make sound more modular, so I can mix & match

pntr_sound* pntr_load_sound_from_memory(pntr_app_sound_type type, unsigned char* data, unsigned int dataSize) {
    (void)type;
    (void)data;
    (void)dataSize;
    return NULL;
}

void pntr_unload_sound(pntr_sound* sound) {
    (void)sound;
}

void pntr_play_sound(pntr_sound* sound, bool loop) {
    (void)sound;
    (void)loop;
}

void pntr_stop_sound(pntr_sound* sound) {
    (void)sound;
}

bool pntr_app_platform_update_delta_time(pntr_app* app) {
    // TODO: Make RSGL delta time get the actual delta time.
    if (app->fps <= 0) {
        app->deltaTime = 0.1f;
        return true;
    }

    app->deltaTime = 1.0f / (float)app->fps;

    return true;
}

PNTR_APP_API void pntr_app_set_title(pntr_app* app, const char* title) {
    if (app == NULL) {
        return;
    }

    app->title = title;

    // TODO

    printf("title\n");
}

bool pntr_app_platform_set_size(pntr_app* app, int width, int height) {
    (void)app;
    (void)width;
    (void)height;

    // TODO

    printf("size\n");

    return true;
}

PNTR_APP_API void pntr_app_set_icon(pntr_app* app, pntr_image* icon) {
    (void)app;
    (void)icon;

    // TODO
}

#endif  // PNTR_APP_RSGL_IMPLEMENTATION_ONCE
#endif  // PNTR_APP_IMPLEMENTATION && !PNTR_APP_HEADER_ONLY
#endif  // PNTR_APP_RSGL
