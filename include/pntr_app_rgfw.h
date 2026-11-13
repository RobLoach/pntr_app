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
        case RGFW_keyEscape: return PNTR_APP_KEY_ESCAPE;
        case RGFW_keyF1: return PNTR_APP_KEY_F1;
        case RGFW_keyF2: return PNTR_APP_KEY_F2;
        case RGFW_keyF3: return PNTR_APP_KEY_F3;
        case RGFW_keyF4: return PNTR_APP_KEY_F4;
        case RGFW_keyF5: return PNTR_APP_KEY_F5;
        case RGFW_keyF6: return PNTR_APP_KEY_F6;
        case RGFW_keyF7: return PNTR_APP_KEY_F7;
        case RGFW_keyF8: return PNTR_APP_KEY_F8;
        case RGFW_keyF9: return PNTR_APP_KEY_F9;
        case RGFW_keyF10: return PNTR_APP_KEY_F10;
        case RGFW_keyF11: return PNTR_APP_KEY_F11;
        case RGFW_keyF12: return PNTR_APP_KEY_F12;
        case RGFW_keyBacktick: return PNTR_APP_KEY_GRAVE_ACCENT;
        case RGFW_key0: return PNTR_APP_KEY_0;
        case RGFW_key1: return PNTR_APP_KEY_1;
        case RGFW_key2: return PNTR_APP_KEY_2;
        case RGFW_key3: return PNTR_APP_KEY_3;
        case RGFW_key4: return PNTR_APP_KEY_4;
        case RGFW_key5: return PNTR_APP_KEY_5;
        case RGFW_key6: return PNTR_APP_KEY_6;
        case RGFW_key7: return PNTR_APP_KEY_7;
        case RGFW_key8: return PNTR_APP_KEY_8;
        case RGFW_key9: return PNTR_APP_KEY_9;
        case RGFW_keyMinus: return PNTR_APP_KEY_MINUS;
        case RGFW_keyEquals: return PNTR_APP_KEY_EQUAL;
        case RGFW_keyBackSpace: return PNTR_APP_KEY_BACKSPACE;
        case RGFW_keyTab: return PNTR_APP_KEY_TAB;
        case RGFW_keyCapsLock: return PNTR_APP_KEY_CAPS_LOCK;
        case RGFW_keyShiftL: return PNTR_APP_KEY_LEFT_SHIFT;
        case RGFW_keyControlL: return PNTR_APP_KEY_LEFT_CONTROL;
        case RGFW_keyAltL: return PNTR_APP_KEY_LEFT_ALT;
        case RGFW_keySuperL: return PNTR_APP_KEY_LEFT_SUPER;
        case RGFW_keyShiftR: return PNTR_APP_KEY_RIGHT_SHIFT;
        case RGFW_keyControlR: return PNTR_APP_KEY_RIGHT_CONTROL;
        case RGFW_keyAltR: return PNTR_APP_KEY_RIGHT_ALT;
        case RGFW_keySuperR: return PNTR_APP_KEY_RIGHT_SUPER;
        case RGFW_keySpace: return PNTR_APP_KEY_SPACE;
        case RGFW_keyA: return PNTR_APP_KEY_A;
        case RGFW_keyB: return PNTR_APP_KEY_B;
        case RGFW_keyC: return PNTR_APP_KEY_C;
        case RGFW_keyD: return PNTR_APP_KEY_D;
        case RGFW_keyE: return PNTR_APP_KEY_E;
        case RGFW_keyF: return PNTR_APP_KEY_F;
        case RGFW_keyG: return PNTR_APP_KEY_G;
        case RGFW_keyH: return PNTR_APP_KEY_H;
        case RGFW_keyI: return PNTR_APP_KEY_I;
        case RGFW_keyJ: return PNTR_APP_KEY_J;
        case RGFW_keyK: return PNTR_APP_KEY_K;
        case RGFW_keyL: return PNTR_APP_KEY_L;
        case RGFW_keyM: return PNTR_APP_KEY_M;
        case RGFW_keyN: return PNTR_APP_KEY_N;
        case RGFW_keyO: return PNTR_APP_KEY_O;
        case RGFW_keyP: return PNTR_APP_KEY_P;
        case RGFW_keyQ: return PNTR_APP_KEY_Q;
        case RGFW_keyR: return PNTR_APP_KEY_R;
        case RGFW_keyS: return PNTR_APP_KEY_S;
        case RGFW_keyT: return PNTR_APP_KEY_T;
        case RGFW_keyU: return PNTR_APP_KEY_U;
        case RGFW_keyV: return PNTR_APP_KEY_V;
        case RGFW_keyW: return PNTR_APP_KEY_W;
        case RGFW_keyX: return PNTR_APP_KEY_X;
        case RGFW_keyY: return PNTR_APP_KEY_Y;
        case RGFW_keyZ: return PNTR_APP_KEY_Z;
        case RGFW_keyPeriod: return PNTR_APP_KEY_PERIOD;
        case RGFW_keyComma: return PNTR_APP_KEY_COMMA;
        case RGFW_keySlash: return PNTR_APP_KEY_SLASH;
        case RGFW_keyBracket: return PNTR_APP_KEY_LEFT_BRACKET;
        case RGFW_keyCloseBracket: return PNTR_APP_KEY_RIGHT_BRACKET;
        case RGFW_keySemicolon: return PNTR_APP_KEY_SEMICOLON;
        case RGFW_keyReturn: return PNTR_APP_KEY_ENTER;
        case RGFW_keyApostrophe: return PNTR_APP_KEY_APOSTROPHE;
        case RGFW_keyBackSlash: return PNTR_APP_KEY_BACKSLASH;
        case RGFW_keyUp: return PNTR_APP_KEY_UP;
        case RGFW_keyDown: return PNTR_APP_KEY_DOWN;
        case RGFW_keyLeft: return PNTR_APP_KEY_LEFT;
        case RGFW_keyRight: return PNTR_APP_KEY_RIGHT;
        case RGFW_keyDelete: return PNTR_APP_KEY_DELETE;
        case RGFW_keyInsert: return PNTR_APP_KEY_INSERT;
        case RGFW_keyMenu: return PNTR_APP_KEY_MENU;
        case RGFW_keyEnd: return PNTR_APP_KEY_END;
        case RGFW_keyHome: return PNTR_APP_KEY_HOME;
        case RGFW_keyPageUp: return PNTR_APP_KEY_PAGE_UP;
        case RGFW_keyPageDown: return PNTR_APP_KEY_PAGE_DOWN;
        case RGFW_keyNumLock: return PNTR_APP_KEY_NUM_LOCK;
        case RGFW_keyScrollLock: return PNTR_APP_KEY_SCROLL_LOCK;
        case RGFW_keyPrintScreen: return PNTR_APP_KEY_PRINT_SCREEN;
        case RGFW_keyPause: return PNTR_APP_KEY_PAUSE;
        case RGFW_keyPadSlash: return PNTR_APP_KEY_KP_DIVIDE;
        case RGFW_keyPadMultiply: return PNTR_APP_KEY_KP_MULTIPLY;
        case RGFW_keyPadPlus: return PNTR_APP_KEY_KP_ADD;
        case RGFW_keyPadMinus: return PNTR_APP_KEY_KP_SUBTRACT;
        case RGFW_keyPadEqual: return PNTR_APP_KEY_KP_EQUAL;
        case RGFW_keyPad1: return PNTR_APP_KEY_KP_1;
        case RGFW_keyPad2: return PNTR_APP_KEY_KP_2;
        case RGFW_keyPad3: return PNTR_APP_KEY_KP_3;
        case RGFW_keyPad4: return PNTR_APP_KEY_KP_4;
        case RGFW_keyPad5: return PNTR_APP_KEY_KP_5;
        case RGFW_keyPad6: return PNTR_APP_KEY_KP_6;
        case RGFW_keyPad7: return PNTR_APP_KEY_KP_7;
        case RGFW_keyPad8: return PNTR_APP_KEY_KP_8;
        case RGFW_keyPad9: return PNTR_APP_KEY_KP_9;
        case RGFW_keyPad0: return PNTR_APP_KEY_KP_0;
        case RGFW_keyPadPeriod: return PNTR_APP_KEY_KP_DECIMAL;
        case RGFW_keyPadReturn: return PNTR_APP_KEY_KP_ENTER;
        case RGFW_keyWorld1: return PNTR_APP_KEY_WORLD_1;
        case RGFW_keyWorld2: return PNTR_APP_KEY_WORLD_2;
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
            case RGFW_windowClose:
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
                if (rgfwEvent.delta.y == 0.0f) {
                    break;
                }
                event.type = PNTR_APP_EVENTTYPE_MOUSE_WHEEL;
                event.mouseWheel = (rgfwEvent.delta.y > 0.0f) ? 1 : -1;
                pntr_app_process_event(app, &event);
                break;
            }

            case RGFW_mouseMotion: {
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
                // Each dropped file arrives as its own event.
                const RGFW_dataDropNode* node = rgfwEvent.drop.value;
                if (node == NULL || node->type != RGFW_dataFile || node->data == NULL) {
                    break;
                }
                event.type = PNTR_APP_EVENTTYPE_FILE_DROPPED;
                event.fileDropped = node->data;
                pntr_app_process_event(app, &event);
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

    // RGFW requires its context to be initialized before creating a window.
    if (RGFW_init(app->title != NULL ? app->title : "pntr_app", 0) < 0) {
        pntr_unload_memory(app->platform);
        app->platform = NULL;
        return false;
    }

    // The screen is blitted without scaling, so keep the window the same size as the screen.
    platform->window = RGFW_createWindow(app->title != NULL ? app->title : "pntr_app", 0, 0, app->width, app->height,
        RGFW_windowCenter | RGFW_windowNoResize | RGFW_windowAllowDND);
    if (platform->window == NULL) {
        RGFW_deinit();
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
    RGFW_deinit();

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
