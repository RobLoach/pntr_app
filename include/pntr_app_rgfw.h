#ifdef PNTR_APP_RGFW
#ifndef PNTR_APP_RGFW_H__
#define PNTR_APP_RGFW_H__

// Platform capabilities, reported by pntr_app_has_feature().
// No gamepad, sound, clipboard or cursor support.
#define PNTR_APP_HAS_KEYBOARD
#define PNTR_APP_HAS_MOUSE
#define PNTR_APP_HAS_FILE_DROPPED
#define PNTR_APP_HAS_SAVE_LOAD

#ifndef PNTR_APP_RGFW_H
#define PNTR_APP_RGFW_H "RGFW.h"
#endif
#include PNTR_APP_RGFW_H

typedef struct pntr_app_rgfw_platform {
    RGFW_window* window;
    RGFW_surface* surface;
    double timerLast;
} pntr_app_rgfw_platform;

void pntr_app_rgfw_set_title(pntr_app* app, const char* title);
void pntr_app_rgfw_set_icon(pntr_app* app, pntr_image* icon);

#endif  // PNTR_APP_RGFW_H__

#if defined(PNTR_APP_IMPLEMENTATION) && !defined(PNTR_APP_HEADER_ONLY)
#ifndef PNTR_APP_RGFW_IMPLEMENTATION_ONCE
#define PNTR_APP_RGFW_IMPLEMENTATION_ONCE

// Define PNTR_APP_RGFW_NO_IMPLEMENTATION if RGFW's implementation is compiled elsewhere.
#ifndef PNTR_APP_RGFW_NO_IMPLEMENTATION
    #define RGFW_IMPLEMENTATION
    #include PNTR_APP_RGFW_H
#endif

#include <string.h> // memset
#include <time.h> // timespec_get

static double pntr_app_rgfw_time(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
}

static pntr_app_key pntr_app_rgfw_key(RGFW_key key) {
    switch (key) {
        case RGFW_escape: return PNTR_APP_KEY_ESCAPE;
        case RGFW_F1: return PNTR_APP_KEY_F1;
        case RGFW_F2: return PNTR_APP_KEY_F2;
        case RGFW_F3: return PNTR_APP_KEY_F3;
        case RGFW_F4: return PNTR_APP_KEY_F4;
        case RGFW_F5: return PNTR_APP_KEY_F5;
        case RGFW_F6: return PNTR_APP_KEY_F6;
        case RGFW_F7: return PNTR_APP_KEY_F7;
        case RGFW_F8: return PNTR_APP_KEY_F8;
        case RGFW_F9: return PNTR_APP_KEY_F9;
        case RGFW_F10: return PNTR_APP_KEY_F10;
        case RGFW_F11: return PNTR_APP_KEY_F11;
        case RGFW_F12: return PNTR_APP_KEY_F12;
        case RGFW_backtick: return PNTR_APP_KEY_GRAVE_ACCENT;
        case RGFW_0: return PNTR_APP_KEY_0;
        case RGFW_1: return PNTR_APP_KEY_1;
        case RGFW_2: return PNTR_APP_KEY_2;
        case RGFW_3: return PNTR_APP_KEY_3;
        case RGFW_4: return PNTR_APP_KEY_4;
        case RGFW_5: return PNTR_APP_KEY_5;
        case RGFW_6: return PNTR_APP_KEY_6;
        case RGFW_7: return PNTR_APP_KEY_7;
        case RGFW_8: return PNTR_APP_KEY_8;
        case RGFW_9: return PNTR_APP_KEY_9;
        case RGFW_minus: return PNTR_APP_KEY_MINUS;
        case RGFW_equals: return PNTR_APP_KEY_EQUAL;
        case RGFW_backSpace: return PNTR_APP_KEY_BACKSPACE;
        case RGFW_tab: return PNTR_APP_KEY_TAB;
        case RGFW_capsLock: return PNTR_APP_KEY_CAPS_LOCK;
        case RGFW_shiftL: return PNTR_APP_KEY_LEFT_SHIFT;
        case RGFW_controlL: return PNTR_APP_KEY_LEFT_CONTROL;
        case RGFW_altL: return PNTR_APP_KEY_LEFT_ALT;
        case RGFW_superL: return PNTR_APP_KEY_LEFT_SUPER;
        case RGFW_shiftR: return PNTR_APP_KEY_RIGHT_SHIFT;
        case RGFW_controlR: return PNTR_APP_KEY_RIGHT_CONTROL;
        case RGFW_altR: return PNTR_APP_KEY_RIGHT_ALT;
        case RGFW_superR: return PNTR_APP_KEY_RIGHT_SUPER;
        case RGFW_space: return PNTR_APP_KEY_SPACE;
        case RGFW_a: return PNTR_APP_KEY_A;
        case RGFW_b: return PNTR_APP_KEY_B;
        case RGFW_c: return PNTR_APP_KEY_C;
        case RGFW_d: return PNTR_APP_KEY_D;
        case RGFW_e: return PNTR_APP_KEY_E;
        case RGFW_f: return PNTR_APP_KEY_F;
        case RGFW_g: return PNTR_APP_KEY_G;
        case RGFW_h: return PNTR_APP_KEY_H;
        case RGFW_i: return PNTR_APP_KEY_I;
        case RGFW_j: return PNTR_APP_KEY_J;
        case RGFW_k: return PNTR_APP_KEY_K;
        case RGFW_l: return PNTR_APP_KEY_L;
        case RGFW_m: return PNTR_APP_KEY_M;
        case RGFW_n: return PNTR_APP_KEY_N;
        case RGFW_o: return PNTR_APP_KEY_O;
        case RGFW_p: return PNTR_APP_KEY_P;
        case RGFW_q: return PNTR_APP_KEY_Q;
        case RGFW_r: return PNTR_APP_KEY_R;
        case RGFW_s: return PNTR_APP_KEY_S;
        case RGFW_t: return PNTR_APP_KEY_T;
        case RGFW_u: return PNTR_APP_KEY_U;
        case RGFW_v: return PNTR_APP_KEY_V;
        case RGFW_w: return PNTR_APP_KEY_W;
        case RGFW_x: return PNTR_APP_KEY_X;
        case RGFW_y: return PNTR_APP_KEY_Y;
        case RGFW_z: return PNTR_APP_KEY_Z;
        case RGFW_period: return PNTR_APP_KEY_PERIOD;
        case RGFW_comma: return PNTR_APP_KEY_COMMA;
        case RGFW_slash: return PNTR_APP_KEY_SLASH;
        case RGFW_bracket: return PNTR_APP_KEY_LEFT_BRACKET;
        case RGFW_closeBracket: return PNTR_APP_KEY_RIGHT_BRACKET;
        case RGFW_semicolon: return PNTR_APP_KEY_SEMICOLON;
        case RGFW_return: return PNTR_APP_KEY_ENTER;
        case RGFW_apostrophe: return PNTR_APP_KEY_APOSTROPHE;
        case RGFW_backSlash: return PNTR_APP_KEY_BACKSLASH;
        case RGFW_up: return PNTR_APP_KEY_UP;
        case RGFW_down: return PNTR_APP_KEY_DOWN;
        case RGFW_left: return PNTR_APP_KEY_LEFT;
        case RGFW_right: return PNTR_APP_KEY_RIGHT;
        case RGFW_delete: return PNTR_APP_KEY_DELETE;
        case RGFW_insert: return PNTR_APP_KEY_INSERT;
        case RGFW_menu: return PNTR_APP_KEY_MENU;
        case RGFW_end: return PNTR_APP_KEY_END;
        case RGFW_home: return PNTR_APP_KEY_HOME;
        case RGFW_pageUp: return PNTR_APP_KEY_PAGE_UP;
        case RGFW_pageDown: return PNTR_APP_KEY_PAGE_DOWN;
        case RGFW_numLock: return PNTR_APP_KEY_NUM_LOCK;
        case RGFW_scrollLock: return PNTR_APP_KEY_SCROLL_LOCK;
        case RGFW_printScreen: return PNTR_APP_KEY_PRINT_SCREEN;
        case RGFW_pause: return PNTR_APP_KEY_PAUSE;
        case RGFW_kpSlash: return PNTR_APP_KEY_KP_DIVIDE;
        case RGFW_kpMultiply: return PNTR_APP_KEY_KP_MULTIPLY;
        case RGFW_kpPlus: return PNTR_APP_KEY_KP_ADD;
        case RGFW_kpMinus: return PNTR_APP_KEY_KP_SUBTRACT;
        case RGFW_kpEqual: return PNTR_APP_KEY_KP_EQUAL;
        case RGFW_kp1: return PNTR_APP_KEY_KP_1;
        case RGFW_kp2: return PNTR_APP_KEY_KP_2;
        case RGFW_kp3: return PNTR_APP_KEY_KP_3;
        case RGFW_kp4: return PNTR_APP_KEY_KP_4;
        case RGFW_kp5: return PNTR_APP_KEY_KP_5;
        case RGFW_kp6: return PNTR_APP_KEY_KP_6;
        case RGFW_kp7: return PNTR_APP_KEY_KP_7;
        case RGFW_kp8: return PNTR_APP_KEY_KP_8;
        case RGFW_kp9: return PNTR_APP_KEY_KP_9;
        case RGFW_kp0: return PNTR_APP_KEY_KP_0;
        case RGFW_kpPeriod: return PNTR_APP_KEY_KP_DECIMAL;
        case RGFW_kpReturn: return PNTR_APP_KEY_KP_ENTER;
        case RGFW_world1: return PNTR_APP_KEY_WORLD_1;
        case RGFW_world2: return PNTR_APP_KEY_WORLD_2;
        default: return PNTR_APP_KEY_INVALID;
    }
}

static pntr_app_mouse_button pntr_app_rgfw_mouse_button(u8 button) {
    switch (button) {
        case RGFW_mouseLeft: return PNTR_APP_MOUSE_BUTTON_LEFT;
        case RGFW_mouseRight: return PNTR_APP_MOUSE_BUTTON_RIGHT;
        case RGFW_mouseMiddle: return PNTR_APP_MOUSE_BUTTON_MIDDLE;
        case RGFW_mouseMisc1: return PNTR_APP_MOUSE_BUTTON_X1;
        case RGFW_mouseMisc2: return PNTR_APP_MOUSE_BUTTON_X2;
        default: return PNTR_APP_MOUSE_BUTTON_UNKNOWN;
    }
}

bool pntr_app_platform_events(pntr_app* app) {
    if (app == NULL || app->platform == NULL) {
        return false;
    }

    pntr_app_rgfw_platform* platform = (pntr_app_rgfw_platform*)app->platform;
    pntr_app_event event = {0};
    event.app = app;
    RGFW_event rgfwEvent;

    while (RGFW_window_checkEvent(platform->window, &rgfwEvent)) {
        switch (rgfwEvent.type) {
            case RGFW_quit:
                return false;

            case RGFW_keyPressed:
            case RGFW_keyReleased: {
                // Don't process key repeats.
                if (rgfwEvent.type == RGFW_keyPressed && rgfwEvent.key.repeat) {
                    break;
                }

                event.key = pntr_app_rgfw_key(rgfwEvent.key.value);
                if (event.key == PNTR_APP_KEY_INVALID) {
                    break;
                }

                // Save and load state.
                if (event.key == PNTR_APP_KEY_F5 || event.key == PNTR_APP_KEY_F9) {
                    if (rgfwEvent.type == RGFW_keyReleased) {
                        event.type = (event.key == PNTR_APP_KEY_F5) ? PNTR_APP_EVENTTYPE_SAVE : PNTR_APP_EVENTTYPE_LOAD;
                        pntr_app_manual_save_load_data(app, &event, PNTR_APP_SAVE_FILENAME);
                    }
                    break;
                }

                event.type = (rgfwEvent.type == RGFW_keyPressed) ? PNTR_APP_EVENTTYPE_KEY_DOWN : PNTR_APP_EVENTTYPE_KEY_UP;
                pntr_app_process_event(app, &event);
                break;
            }

            case RGFW_mouseButtonPressed:
            case RGFW_mouseButtonReleased: {
                event.mouseButton = pntr_app_rgfw_mouse_button(rgfwEvent.button.value);
                if (event.mouseButton == PNTR_APP_MOUSE_BUTTON_UNKNOWN) {
                    break;
                }
                event.type = (rgfwEvent.type == RGFW_mouseButtonPressed) ? PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN : PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP;
                pntr_app_process_event(app, &event);
                break;
            }

            case RGFW_mouseScroll: {
                if (rgfwEvent.scroll.y == 0.0f) {
                    break;
                }
                event.type = PNTR_APP_EVENTTYPE_MOUSE_WHEEL;
                event.mouseWheel = (rgfwEvent.scroll.y > 0.0f) ? 1 : -1;
                pntr_app_process_event(app, &event);
                break;
            }

            case RGFW_mousePosChanged: {
                // The window is the same size as the screen, so the position maps directly.
                event.type = PNTR_APP_EVENTTYPE_MOUSE_MOVE;
                event.mouseX = rgfwEvent.mouse.x;
                event.mouseY = rgfwEvent.mouse.y;
                event.mouseDeltaX = 0;
                event.mouseDeltaY = 0;
                pntr_app_process_event(app, &event);
                break;
            }

            case RGFW_dataDrop: {
                event.type = PNTR_APP_EVENTTYPE_FILE_DROPPED;
                for (size_t i = 0; i < rgfwEvent.drop.count; i++) {
                    event.fileDropped = rgfwEvent.drop.files[i];
                    pntr_app_process_event(app, &event);
                }
                event.fileDropped = NULL;
                break;
            }

            default:
                break;
        }
    }

    return !RGFW_window_shouldClose(platform->window);
}

bool pntr_app_platform_render(pntr_app* app) {
    if (app == NULL || app->screen == NULL || app->platform == NULL) {
        return false;
    }

    pntr_app_rgfw_platform* platform = (pntr_app_rgfw_platform*)app->platform;
    pntr_image* screen = app->screen;

    // The surface wraps the screen's pixels, and is rebuilt after pntr_app_set_size().
    if (platform->surface == NULL) {
        platform->surface = RGFW_window_createSurface(platform->window, (u8*)screen->data, screen->width, screen->height, RGFW_formatRGBA8);
        if (platform->surface == NULL) {
            return false;
        }
    }

    RGFW_window_blitSurface(platform->window, platform->surface);

    return true;
}

bool pntr_app_platform_init(pntr_app* app) {
    if (app == NULL) {
        return false;
    }

    app->platform = pntr_load_memory(sizeof(pntr_app_rgfw_platform));
    if (app->platform == NULL) {
        return false;
    }
    memset(app->platform, 0, sizeof(pntr_app_rgfw_platform));

    pntr_app_rgfw_platform* platform = (pntr_app_rgfw_platform*)app->platform;

    // The screen is blitted without scaling, so keep the window the same size as the screen.
    platform->window = RGFW_createWindow(app->title != NULL ? app->title : "pntr_app", 0, 0, app->width, app->height,
        RGFW_windowCenter | RGFW_windowNoResize | RGFW_windowAllowDND);
    if (platform->window == NULL) {
        pntr_unload_memory(app->platform);
        app->platform = NULL;
        return false;
    }

    // Timer
    platform->timerLast = pntr_app_rgfw_time();

    // Random Number Generator
    pntr_app_random_set_seed(app, (uint64_t)(platform->timerLast * 1000000.0));

    return true;
}

void pntr_app_platform_close(pntr_app* app) {
    if (app == NULL || app->platform == NULL) {
        return;
    }

    pntr_app_rgfw_platform* platform = (pntr_app_rgfw_platform*)app->platform;
    if (platform->surface != NULL) {
        RGFW_surface_free(platform->surface);
    }
    if (platform->window != NULL) {
        RGFW_window_close(platform->window);
    }

    pntr_unload_memory(app->platform);
    app->platform = NULL;
}

bool pntr_app_platform_update_delta_time(pntr_app* app) {
    if (app == NULL || app->platform == NULL) {
        return false;
    }

    pntr_app_rgfw_platform* platform = (pntr_app_rgfw_platform*)app->platform;
    double currentTime = pntr_app_rgfw_time();
    double deltaTime = currentTime - platform->timerLast;

    if (app->fps > 0 && deltaTime < 1.0 / (double)app->fps) {
        return false;
    }

    app->deltaTime = (float)deltaTime;
    platform->timerLast = currentTime;
    return true;
}

bool pntr_app_platform_set_size(pntr_app* app, int width, int height) {
    if (app == NULL || app->platform == NULL) {
        return false;
    }

    pntr_app_rgfw_platform* platform = (pntr_app_rgfw_platform*)app->platform;
    RGFW_window_resize(platform->window, width, height);

    // The surface is rebuilt against the new screen at the next render.
    if (platform->surface != NULL) {
        RGFW_surface_free(platform->surface);
        platform->surface = NULL;
    }

    return true;
}

#ifndef PNTR_APP_SET_TITLE
#define PNTR_APP_SET_TITLE pntr_app_rgfw_set_title
void pntr_app_rgfw_set_title(pntr_app* app, const char* title) {
    if (app == NULL || app->platform == NULL || title == NULL) {
        return;
    }

    pntr_app_rgfw_platform* platform = (pntr_app_rgfw_platform*)app->platform;
    RGFW_window_setName(platform->window, title);
}
#endif

#ifndef PNTR_APP_SET_ICON
#define PNTR_APP_SET_ICON pntr_app_rgfw_set_icon
void pntr_app_rgfw_set_icon(pntr_app* app, pntr_image* icon) {
    if (app == NULL || app->platform == NULL || icon == NULL) {
        return;
    }

    pntr_app_rgfw_platform* platform = (pntr_app_rgfw_platform*)app->platform;
    RGFW_window_setIcon(platform->window, (u8*)icon->data, icon->width, icon->height, RGFW_formatRGBA8);
}
#endif

#endif  // PNTR_APP_RGFW_IMPLEMENTATION_ONCE
#endif  // PNTR_APP_IMPLEMENTATION && !PNTR_APP_HEADER_ONLY
#endif  // PNTR_APP_RGFW
