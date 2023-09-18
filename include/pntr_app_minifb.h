#include "MiniFB.h"

typedef struct pntr_app_platform_minifb {
    struct mfb_window* window;
    struct mfb_timer *timer;
} pntr_app_platform_minifb;

pntr_app_mouse_button pntr_app_minifb_mouse_button(mfb_mouse_button button) {
    switch (button) {
        case MOUSE_LEFT: return PNTR_APP_MOUSE_BUTTON_LEFT;
        case MOUSE_RIGHT: return PNTR_APP_MOUSE_BUTTON_RIGHT;
        case MOUSE_MIDDLE: return PNTR_APP_MOUSE_BUTTON_MIDDLE;
    }

    return PNTR_APP_MOUSE_BUTTON_UNKNOWN;
}

bool pntr_app_events(pntr_app* app) {
    if (app == NULL || app->platform == NULL) {
        return false;
    }

    pntr_app_platform_minifb* platform = (pntr_app_platform_minifb*)app->platform;

    // TODO: MiniFB - Gamepads are currently not supported by MiniFB

    return mfb_wait_sync(platform->window);
}

/**
 * Pushes the given image to the screen.
 */
bool pntr_app_render(pntr_app* app) {
    if (app == NULL || app->screen == NULL) {
        return false;
    }

    // Find the window
    pntr_app_platform_minifb* platform = (pntr_app_platform_minifb*)app->platform;

    // Render the screen buffer
    mfb_update_state state = mfb_update_ex(platform->window, app->screen->data, app->screen->width, app->screen->height);

    return state == STATE_OK;
}

void pntr_app_minifb_resize(struct mfb_window *window, int width, int height) {
    pntr_app* app = (pntr_app*)mfb_get_user_data(window);
    mfb_set_viewport_best_fit(window, app->width, app->height);
}

void pntr_app_minifb_keyboard(struct mfb_window *window, mfb_key key, mfb_key_mod mod, bool isPressed) {
    pntr_app* app = (pntr_app*)mfb_get_user_data(window);

    // Handle escape to quit
    if (key == KB_KEY_ESCAPE) {
        if (isPressed) {
            mfb_close(window);
        }
        return;
    }

    pntr_app_event event;
    event.type = isPressed ? PNTR_APP_EVENTTYPE_KEY_DOWN : PNTR_APP_EVENTTYPE_KEY_UP;
    event.key = key;
    pntr_app_process_event(app, &event);
}

void pntr_app_minifb_mouse_btn(struct mfb_window *window, mfb_mouse_button button, mfb_key_mod mod, bool isPressed) {
    pntr_app* app = (pntr_app*)mfb_get_user_data(window);

    pntr_app_event event;
    event.type = isPressed ? PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN : PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP;
    event.mouseButton = pntr_app_minifb_mouse_button(button);

    if (event.mouseButton != PNTR_APP_MOUSE_BUTTON_UNKNOWN) {
        event.mouseX = app->mouseX;
        event.mouseY = app->mouseY;
        pntr_app_process_event(app, &event);
    }
}

void pntr_app_minifb_mouse_move(struct mfb_window *window, int x, int y) {
    pntr_app* app = (pntr_app*)mfb_get_user_data(window);

    // TODO: MiniFB: Don't do this in the mouse_move callback

    pntr_app_event event;
    event.type = PNTR_APP_EVENTTYPE_MOUSE_MOVE;

    // Find the aspect ratio.
    float aspect = (float)app->screen->width / (float)app->screen->height;
    if (aspect <= 0) {
        aspect = (float)app->screen->height / (float)app->screen->width;
    }

    // Calculate the optimal width/height to display in the screen size.
    int height = mfb_get_window_height(window);
    int width = height * aspect;
    if (width > mfb_get_window_width(window)) {
        height = mfb_get_window_width(window) / aspect;
        width = mfb_get_window_width(window);
    }

    // Draw the texture in the middle of the screen.
    int outRectx = (mfb_get_window_width(window) - width) / 2;
    int outRecty = (mfb_get_window_height(window) - height) / 2;

    event.mouseX = (x - outRectx) * app->screen->width / width;
    event.mouseY = (y - outRecty) * app->screen->height / height;

    pntr_app_process_event(app, &event);
}

void pntr_app_minifb_mouse_scroll(struct mfb_window *window, mfb_key_mod mod, float deltaX, float deltaY) {
    pntr_app* app = (pntr_app*)mfb_get_user_data(window);

    pntr_app_event event;
    event.type = PNTR_APP_EVENTTYPE_MOUSE_WHEEL;
    if (deltaY < 0) {
        event.mouseWheel = -1;
    }
    else if (deltaY > 0) {
        event.mouseWheel = 1;
    }
    else {
        return;
    }

    event.mouseX = app->mouseX;
    event.mouseY = app->mouseY;

    pntr_app_process_event(app, &event);
}

bool pntr_app_init(pntr_app* app) {
    if (app == NULL) {
        return false;
    }

    pntr_app_platform_minifb* platform = (pntr_app_platform_minifb*)pntr_load_memory(sizeof(pntr_app_platform_minifb));
    if (platform == NULL) {
        return false;
    }

    struct mfb_window *window = mfb_open_ex(app->title, app->width, app->height, WF_RESIZABLE);
    if (window == NULL) {
        return false;
    }
    platform->window = window;

    // User Data
    mfb_set_user_data(window, (void*)app);

    // FPS
    if (app->fps > 0) {
        mfb_set_target_fps(app->fps);
    }

    // Events
    mfb_set_resize_callback(window, pntr_app_minifb_resize);
    mfb_set_keyboard_callback(window, pntr_app_minifb_keyboard);
    mfb_set_mouse_button_callback(window, pntr_app_minifb_mouse_btn);
    mfb_set_mouse_move_callback(window, pntr_app_minifb_mouse_move);
    mfb_set_mouse_scroll_callback(window, pntr_app_minifb_mouse_scroll);

    // Timer
    platform->timer = mfb_timer_create();

    // Set up the app platform
    app->platform = platform;

    return true;
}

void pntr_app_close(pntr_app* app) {
    if (app == NULL || app->platform == NULL) {
        return;
    }

    pntr_app_platform_minifb* platform = (pntr_app_platform_minifb*)app->platform;

    mfb_timer_destroy(platform->timer);
    pntr_unload_memory(platform);

    app->platform = NULL;
}

bool pntr_app_platform_update_delta_time(pntr_app* app) {
    pntr_app_platform_minifb* platform = (pntr_app_platform_minifb*)app->platform;
    app->deltaTime = (float)mfb_timer_delta(platform->timer);

    return true;
}

PNTR_APP_API void pntr_app_set_title(pntr_app* app, const char* title) {
    // Nothing.
    (void)app;
    (void)title;
}

bool _pntr_app_platform_set_size(pntr_app* app, int width, int height) {
    if (app == NULL || app->platform == NULL) {
        return false;
    }

    pntr_app_platform_minifb* platform = (pntr_app_platform_minifb*)app->platform;
    return mfb_set_viewport_best_fit(platform->window, width, height);
}
