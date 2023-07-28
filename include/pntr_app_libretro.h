#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
//#include <math.h>

#ifndef PNTR_APP_LIBRETRO_H
#define PNTR_APP_LIBRETRO_H "libretro.h"
#endif
#include PNTR_APP_LIBRETRO_H

#define PNTR_APP_NO_ENTRY

pntr_app* pntr_app_libretro;

static struct retro_log_callback logging;
static retro_log_printf_t log_cb;

typedef struct pntr_app_libretro_platform {
    int16_t mouseButtonState[PNTR_APP_MOUSE_BUTTON_LAST];
    int16_t mouseX;
    int16_t mouseY;
    int16_t gamepadState[4][PNTR_APP_GAMEPAD_BUTTON_LAST];
} pntr_app_libretro_platform;

pntr_app_gamepad_button pntr_app_libretro_gamepad_button(int button) {
    switch (button) {
        case RETRO_DEVICE_ID_JOYPAD_UP: return PNTR_APP_GAMEPAD_BUTTON_LEFT_FACE_UP;
        case RETRO_DEVICE_ID_JOYPAD_RIGHT: return PNTR_APP_GAMEPAD_BUTTON_LEFT_FACE_RIGHT;
        case RETRO_DEVICE_ID_JOYPAD_DOWN: return PNTR_APP_GAMEPAD_BUTTON_LEFT_FACE_DOWN;
        case RETRO_DEVICE_ID_JOYPAD_LEFT: return PNTR_APP_GAMEPAD_BUTTON_LEFT_FACE_LEFT;
        case RETRO_DEVICE_ID_JOYPAD_X: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_FACE_UP;
        case RETRO_DEVICE_ID_JOYPAD_A: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_FACE_RIGHT;
        case RETRO_DEVICE_ID_JOYPAD_B: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_FACE_DOWN;
        case RETRO_DEVICE_ID_JOYPAD_Y: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_FACE_LEFT;
        case RETRO_DEVICE_ID_JOYPAD_L: return PNTR_APP_GAMEPAD_BUTTON_LEFT_TRIGGER_1;
        case RETRO_DEVICE_ID_JOYPAD_L2: return PNTR_APP_GAMEPAD_BUTTON_LEFT_TRIGGER_2;
        case RETRO_DEVICE_ID_JOYPAD_R: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_TRIGGER_1;
        case RETRO_DEVICE_ID_JOYPAD_R2: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_TRIGGER_2;
        case RETRO_DEVICE_ID_JOYPAD_SELECT: return PNTR_APP_GAMEPAD_BUTTON_MIDDLE_LEFT;
        //case RETRO_DEVICE_ID_JOYPAD_L3: return PNTR_APP_GAMEPAD_BUTTON_MIDDLE;
        case RETRO_DEVICE_ID_JOYPAD_START: return PNTR_APP_GAMEPAD_BUTTON_MIDDLE_RIGHT;
        case RETRO_DEVICE_ID_JOYPAD_L3: return PNTR_APP_GAMEPAD_BUTTON_LEFT_THUMB;
        case RETRO_DEVICE_ID_JOYPAD_R3: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_THUMB;
    }

    return PNTR_APP_GAMEPAD_BUTTON_UNKNOWN;
}

pntr_app_key pntr_app_libretro_key(int key) {
    switch (key) {
        case RETROK_UNKNOWN: return PNTR_APP_KEY_INVALID;
        case RETROK_BACKSPACE: return PNTR_APP_KEY_BACKSPACE;
        case RETROK_TAB: return PNTR_APP_KEY_TAB;
        //case RETROK_CLEAR: return PNTR_APP_KEY_CLEAR;
        case RETROK_RETURN: return PNTR_APP_KEY_ENTER;
        //case RETROK_PAUSE: return PNTR_APP_KEY_PAUSE;
        case RETROK_ESCAPE: return PNTR_APP_KEY_ESCAPE;
        case RETROK_SPACE: return PNTR_APP_KEY_SPACE;
        // case RETROK_EXCLAIM: return PNTR_APP_KEY_EXCLAIM;
        // case RETROK_QUOTEDBL: return PNTR_APP_KEY_D;
        // case RETROK_HASH: return PNTR_APP_KEY_HASH;
        // case RETROK_DOLLAR: return PNTR_APP_KEY_DOLLAR;
        // case RETROK_AMPERSAND: return PNTR_APP_KEY_AMPERSAND;
        // case RETROK_QUOTE: return PNTR_APP_KEY_QUOTE;
        // case RETROK_LEFTPAREN: return PNTR_APP_KEY_LEFTPAREN;
        // case RETROK_RIGHTPAREN: return PNTR_APP_KEY_RIGHTPAREN;
        // case RETROK_ASTERISK: return PNTR_APP_KEY_ASTERISK;
        // case RETROK_PLUS: return PNTR_APP_KEY_PLUS;
        // case RETROK_COMMA: return PNTR_APP_KEY_COMMA;
        // case RETROK_MINUS: return PNTR_APP_KEY_MINUS;
        // case RETROK_PERIOD: return PNTR_APP_KEY_PERIOD;
        // case RETROK_SLASH: return PNTR_APP_KEY_SLASH;
        case RETROK_0: return PNTR_APP_KEY_0;
        case RETROK_1: return PNTR_APP_KEY_1;
        case RETROK_2: return PNTR_APP_KEY_2;
        case RETROK_3: return PNTR_APP_KEY_3;
        case RETROK_4: return PNTR_APP_KEY_4;
        case RETROK_5: return PNTR_APP_KEY_5;
        case RETROK_6: return PNTR_APP_KEY_6;
        case RETROK_7: return PNTR_APP_KEY_7;
        case RETROK_8: return PNTR_APP_KEY_8;
        case RETROK_9: return PNTR_APP_KEY_9;
        //case RETROK_COLON: return PNTR_APP_KEY_COLON;
        case RETROK_SEMICOLON: return PNTR_APP_KEY_SEMICOLON;
        case RETROK_LESS: return PNTR_APP_KEY_MINUS;
        case RETROK_EQUALS: return PNTR_APP_KEY_EQUAL;
        //case RETROK_GREATER: return PNTR_APP_KEY_GREATER;
        //case RETROK_QUESTION: return PNTR_APP_KEY_QUESTION;
        //case RETROK_AT: return PNTR_APP_KEY_AT;
        case RETROK_LEFTBRACKET: return PNTR_APP_KEY_LEFT_BRACKET;
        case RETROK_BACKSLASH: return PNTR_APP_KEY_BACKSLASH;
        case RETROK_RIGHTBRACKET: return PNTR_APP_KEY_RIGHT_BRACKET;
        //case RETROK_CARET: return PNTR_APP_KEY_CARET;
        //case RETROK_UNDERSCORE: return PNTR_APP_KEY_UNDERSCORE;
        //case RETROK_BACKQUOTE: return PNTR_APP_KEY_BACKQUOTE;
        case RETROK_a: return PNTR_APP_KEY_A;
        case RETROK_b: return PNTR_APP_KEY_B;
        case RETROK_c: return PNTR_APP_KEY_C;
        case RETROK_d: return PNTR_APP_KEY_D;
        case RETROK_e: return PNTR_APP_KEY_E;
        case RETROK_f: return PNTR_APP_KEY_F;
        case RETROK_g: return PNTR_APP_KEY_G;
        case RETROK_h: return PNTR_APP_KEY_H;
        case RETROK_i: return PNTR_APP_KEY_I;
        case RETROK_j: return PNTR_APP_KEY_J;
        case RETROK_k: return PNTR_APP_KEY_K;
        case RETROK_l: return PNTR_APP_KEY_L;
        case RETROK_m: return PNTR_APP_KEY_M;
        case RETROK_n: return PNTR_APP_KEY_N;
        case RETROK_o: return PNTR_APP_KEY_O;
        case RETROK_p: return PNTR_APP_KEY_P;
        case RETROK_q: return PNTR_APP_KEY_Q;
        case RETROK_r: return PNTR_APP_KEY_R;
        case RETROK_s: return PNTR_APP_KEY_S;
        case RETROK_t: return PNTR_APP_KEY_T;
        case RETROK_u: return PNTR_APP_KEY_U;
        case RETROK_v: return PNTR_APP_KEY_V;
        case RETROK_w: return PNTR_APP_KEY_W;
        case RETROK_x: return PNTR_APP_KEY_X;
        case RETROK_y: return PNTR_APP_KEY_Y;
        case RETROK_z: return PNTR_APP_KEY_Z;
        //case RETROK_LEFTBRACE: return PNTR_APP_KEY_LEFT_BRACE;
        //case RETROK_BAR: return PNTR_APP_KEY_BAR;
        //case RETROK_RIGHTBRACE: return PNTR_APP_KEY_RIGHT_BRACE;
        //case RETROK_TILDE: return PNTR_APP_KEY_TILDE;
        case RETROK_DELETE: return PNTR_APP_KEY_DELETE;
        case RETROK_KP0: return PNTR_APP_KEY_KP_0;
        case RETROK_KP1: return PNTR_APP_KEY_KP_1;
        case RETROK_KP2: return PNTR_APP_KEY_KP_2;
        case RETROK_KP3: return PNTR_APP_KEY_KP_3;
        case RETROK_KP4: return PNTR_APP_KEY_KP_4;
        case RETROK_KP5: return PNTR_APP_KEY_KP_5;
        case RETROK_KP6: return PNTR_APP_KEY_KP_6;
        case RETROK_KP7: return PNTR_APP_KEY_KP_7;
        case RETROK_KP8: return PNTR_APP_KEY_KP_8;
        case RETROK_KP9: return PNTR_APP_KEY_KP_9;
        case RETROK_KP_PERIOD: return PNTR_APP_KEY_KP_DECIMAL;
        case RETROK_KP_DIVIDE: return PNTR_APP_KEY_KP_DIVIDE;
        case RETROK_KP_MULTIPLY: return PNTR_APP_KEY_KP_MULTIPLY;
        case RETROK_KP_MINUS: return PNTR_APP_KEY_KP_SUBTRACT;
        case RETROK_KP_PLUS: return PNTR_APP_KEY_KP_ADD;
        case RETROK_KP_ENTER: return PNTR_APP_KEY_KP_ENTER;
        case RETROK_KP_EQUALS: return PNTR_APP_KEY_KP_EQUAL;
        case RETROK_UP: return PNTR_APP_KEY_UP;
        case RETROK_DOWN: return PNTR_APP_KEY_DOWN;
        case RETROK_RIGHT: return PNTR_APP_KEY_RIGHT;
        case RETROK_LEFT: return PNTR_APP_KEY_LEFT;
        case RETROK_INSERT: return PNTR_APP_KEY_INSERT;
        case RETROK_HOME: return PNTR_APP_KEY_HOME;
        case RETROK_END: return PNTR_APP_KEY_END;
        case RETROK_PAGEUP: return PNTR_APP_KEY_PAGE_UP;
        case RETROK_PAGEDOWN: return PNTR_APP_KEY_PAGE_DOWN;
        case RETROK_F1: return PNTR_APP_KEY_F1;
        case RETROK_F2: return PNTR_APP_KEY_F2;
        case RETROK_F3: return PNTR_APP_KEY_F3;
        case RETROK_F4: return PNTR_APP_KEY_F4;
        case RETROK_F5: return PNTR_APP_KEY_F5;
        case RETROK_F6: return PNTR_APP_KEY_F6;
        case RETROK_F7: return PNTR_APP_KEY_F7;
        case RETROK_F8: return PNTR_APP_KEY_F8;
        case RETROK_F9: return PNTR_APP_KEY_F9;
        case RETROK_F10: return PNTR_APP_KEY_F10;
        case RETROK_F11: return PNTR_APP_KEY_F11;
        case RETROK_F12: return PNTR_APP_KEY_F12;
        case RETROK_F13: return PNTR_APP_KEY_F13;
        case RETROK_F14: return PNTR_APP_KEY_F14;
        case RETROK_F15: return PNTR_APP_KEY_F15;
        case RETROK_NUMLOCK: return PNTR_APP_KEY_NUM_LOCK;
        case RETROK_CAPSLOCK: return PNTR_APP_KEY_CAPS_LOCK;
        case RETROK_SCROLLOCK: return PNTR_APP_KEY_SCROLL_LOCK;
        case RETROK_RSHIFT: return PNTR_APP_KEY_RIGHT_SHIFT;
        case RETROK_LSHIFT: return PNTR_APP_KEY_LEFT_SHIFT;
        case RETROK_RCTRL: return PNTR_APP_KEY_RIGHT_CONTROL;
        case RETROK_LCTRL: return PNTR_APP_KEY_LEFT_CONTROL;
        case RETROK_RALT: return PNTR_APP_KEY_RIGHT_ALT;
        case RETROK_LALT: return PNTR_APP_KEY_LEFT_ALT;
        // case RETROK_RMETA: return PNTR_APP_KEY_RMETA;
        // case RETROK_LMETA: return PNTR_APP_KEY_LMETA;
        // case RETROK_LSUPER: return PNTR_APP_KEY_LSUPER;
        // case RETROK_RSUPER: return PNTR_APP_KEY_RSUPER;
    }

    return PNTR_APP_KEY_INVALID;
}

static void fallback_log(enum retro_log_level level, const char *fmt, ...) {
    (void)level;
    va_list va;
    va_start(va, fmt);
    vfprintf(stderr, fmt, va);
    va_end(va);
}

void retro_init(void) {
    // Nothing.
}

void retro_deinit(void) {
    // Nothing.
}

unsigned retro_api_version(void) {
    return RETRO_API_VERSION;
}

void retro_set_controller_port_device(unsigned port, unsigned device) {
    log_cb(RETRO_LOG_INFO, "Plugging device %u into port %u.\n", device, port);
}

void retro_get_system_info(struct retro_system_info *info) {
    memset(info, 0, sizeof(*info));

    if (pntr_app_libretro == NULL) {
        char* argv[1];
        argv[0] = "pntr_app";
        pntr_app app = PNTR_APP_MAIN(1, argv);
        info->library_name = app.title;
        // Clear up any user data.
        if (app.userData != NULL) {
            pntr_unload_memory(app.userData);
        }
    }
    else {
        info->library_name = pntr_app_libretro->title;
    }

    info->library_version  = "0.0.1";
    info->need_fullpath    = false;
    info->valid_extensions = NULL; // Anything is fine, we don't care.
}

static retro_video_refresh_t video_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static retro_environment_t environ_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;

void retro_get_system_av_info(struct retro_system_av_info *info) {
    int fps = 60;
    int width = 640;
    int height = 480;
    if (pntr_app_libretro == NULL) {
        char* argv[1];
        argv[0] = "pntr_app";
        pntr_app app = PNTR_APP_MAIN(1, argv);
        fps = app.fps;
        width = app.width;
        height = app.height;
        if (app.userData != NULL) {
            pntr_unload_memory(app.userData);
        }
    }
    else {
        fps = pntr_app_libretro->fps;
        width = pntr_app_libretro->width;
        height = pntr_app_libretro->height;
    }

    info->timing = (struct retro_system_timing) {
        .fps = (fps > 0) ? (double)fps : 0.0,
        .sample_rate = 0.0,
    };
    info->geometry = (struct retro_game_geometry) {
        .base_width   = width,
        .base_height  = height,
        .max_width    = width,
        .max_height   = height,
        .aspect_ratio = (float)width / (float)height,
    };
}

void retro_set_environment(retro_environment_t cb) {
    environ_cb = cb;

    bool no_content = true;
    cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_content);

    if (cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &logging)) {
        log_cb = logging.log;
    }
    else {
        log_cb = fallback_log;
    }
}

void retro_set_audio_sample(retro_audio_sample_t cb) {
    audio_cb = cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) {
    audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb) {
    input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb) {
    input_state_cb = cb;
}

void retro_set_video_refresh(retro_video_refresh_t cb) {
    video_cb = cb;
}

void retro_reset(void) {
    // Nothing
}

static void check_variables(void) {
     // Nothing.
}

static void audio_callback(void) {
    audio_cb(0, 0);
}

int pntr_app_libretro_mouse_button_to_retro(pntr_app_mouse_button button) {
    switch (button) {
        case PNTR_APP_MOUSE_BUTTON_LEFT: return RETRO_DEVICE_ID_MOUSE_LEFT;
        case PNTR_APP_MOUSE_BUTTON_RIGHT: return RETRO_DEVICE_ID_MOUSE_RIGHT;
        case PNTR_APP_MOUSE_BUTTON_MIDDLE: return RETRO_DEVICE_ID_MOUSE_MIDDLE;
    }
    return -1;
}

/**
 * Converts a Pointer API coordinates to screen pixel position.
 *
 * @see RETRO_DEVICE_POINTER
 */
int pntr_app_libretro_mouse_pointer_convert(float coord, float full, float margin)
{
	float max         = (float)0x7fff;
	float screenCoord = (((coord + max) / (max * 2.0f) ) * full) - margin;

	// Keep the mouse on the screen.
	if (margin > 0.0f) {
		float limit = full - (margin * 2.0f) - 1.0f;
		screenCoord = (screenCoord < 0.0f)  ? 0.0f  : screenCoord;
		screenCoord = (screenCoord > limit) ? limit : screenCoord;
	}

	return (int)(screenCoord + 0.5f);
}

bool pntr_app_events(pntr_app* app) {
    // Tell the frontend to update its input state.
    if (app == NULL || app->event == NULL || input_poll_cb == NULL || input_state_cb == NULL) {
        return true;
    }

    input_poll_cb();
    pntr_app_libretro_platform* platform = (pntr_app_libretro_platform*)app->platform;

    pntr_app_event event;

    // Mouse Move
    int16_t mouseX = input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_X);
    int16_t mouseY = input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_Y);
    event.mouseX = pntr_app_libretro_mouse_pointer_convert((float)mouseX, app->width, 0.0f);
    event.mouseY = pntr_app_libretro_mouse_pointer_convert((float)mouseY, app->height, 0.0f);
    if (platform->mouseX != event.mouseX || platform->mouseY != event.mouseY) {
        event.type = PNTR_APP_EVENTTYPE_MOUSE_MOVE;
        event.mouseDeltaX = event.mouseX - platform->mouseX;
        event.mouseDeltaY = event.mouseY - platform->mouseY;
        platform->mouseX = event.mouseX;
        platform->mouseY = event.mouseY;

        // Invoke the event.
        app->event(&event, app->userData);
    }

    // Mouse Buttons
    for (event.mouseButton = PNTR_APP_MOUSE_BUTTON_FIRST; event.mouseButton < PNTR_APP_MOUSE_BUTTON_LAST; event.mouseButton++) {
        int retroButton = pntr_app_libretro_mouse_button_to_retro(event.mouseButton);
        if (retroButton == -1) {
            continue;
        }
        int16_t currentState = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, retroButton);
        if (platform->mouseButtonState[event.mouseButton] != currentState) {
            event.type = (currentState == 0) ? PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP : PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN;
            event.mouseX = pntr_app_libretro_mouse_pointer_convert(platform->mouseX, app->width, 0.0f);
            event.mouseY = pntr_app_libretro_mouse_pointer_convert(platform->mouseY, app->height, 0.0f);
            platform->mouseButtonState[event.mouseButton] = currentState;

            // Invoke the event.
            app->event(&event, app->userData);
        }
    }

    // Gamepad Buttons
    for (event.gamepad = 0; event.gamepad < 4; event.gamepad++) {
        // TODO: Switch to libretro gamepad bitmasks?
        for (int button = RETRO_DEVICE_ID_JOYPAD_B; button <= RETRO_DEVICE_ID_JOYPAD_R3; button++) {
            int16_t currentState = input_state_cb(event.gamepad, RETRO_DEVICE_JOYPAD, 0, button);
            if (currentState != platform->gamepadState[event.gamepad][button]) {
                event.gamepadButton = pntr_app_libretro_gamepad_button(button);
                if (event.gamepadButton == PNTR_APP_GAMEPAD_BUTTON_UNKNOWN) {
                    continue;
                }

                if (currentState) {
                    event.type = PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_DOWN;
                }
                else {
                    event.type = PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_UP;
                }

                app->event(&event, app->userData);
                platform->gamepadState[event.gamepad][button] = currentState;
            }
        }
    }

    return true;
}

/**
 * Pushes the given image to the screen.
 */
bool pntr_app_render(pntr_app* app) {
    if (app == NULL || app->screen == NULL) {
        return false;
    }

    pntr_image* screen = app->screen;
    video_cb((void*)screen->data, screen->width, screen->height, screen->pitch);
}

bool pntr_app_init(pntr_app* app) {
    return true;
}

void pntr_app_close(pntr_app* app) {
    if (app == NULL) {
        return;
    }

    if (app->close != NULL) {
        app->close(app->userData);
    }

    if (app->screen != NULL) {
        pntr_unload_image(app->screen);
    }

    // Clear up any user data.
    pntr_unload_memory(app->userData);
    pntr_unload_memory(app->platform);
    pntr_unload_memory(app);
}

void retro_run(void) {
    if (pntr_app_libretro == NULL) {
        return;
    }

    if (pntr_app_events(pntr_app_libretro) == false) {
        environ_cb(RETRO_ENVIRONMENT_SHUTDOWN, NULL);
        return;
    };

    if (pntr_app_libretro->update != NULL) {
        if (pntr_app_libretro->update(pntr_app_libretro->screen, pntr_app_libretro->userData) == false) {
            environ_cb(RETRO_ENVIRONMENT_SHUTDOWN, NULL);
            return;
        }
    }

    pntr_app_render(pntr_app_libretro);

    audio_callback();

    bool updated = false;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated) {
        check_variables();
    }
}

void pntr_app_libretro_keyboard_callback(bool down, unsigned keycode, uint32_t character, uint16_t key_modifiers) {
    if (pntr_app_libretro == NULL || pntr_app_libretro->event == NULL) {
        return;
    }

    pntr_app_event event;
    event.key = pntr_app_libretro_key(keycode);
    if (event.key != PNTR_APP_KEY_INVALID) {
        event.type = down ? PNTR_APP_EVENTTYPE_KEY_DOWN : PNTR_APP_EVENTTYPE_KEY_UP;
        pntr_app_libretro->event(&event, pntr_app_libretro->userData);
    }
}

bool retro_load_game(const struct retro_game_info *info) {
    int argc = 1;
    char* argv[2] = {
        "pntr_app",
        info != NULL ? (char*)info->path : NULL
    };
    if (info != NULL && info->path != NULL) {
        argc++;
    }

    pntr_app app = PNTR_APP_MAIN(argc, argv);

    // Call the init callback.
    if (app.init != NULL) {
        // Check if initialization worked.
        if (app.init(app.userData) == false) {
            pntr_unload_memory(app.userData);
            pntr_unload_memory(app.platform);
            return false;
        }
    }

    app.screen = pntr_gen_image_color(app.width, app.height, PNTR_BLACK);
    if (app.screen == NULL) {
        pntr_app_close(&app);
        return false;
    }

    enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
    if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt)) {
        log_cb(RETRO_LOG_INFO, "XRGB8888 is not supported.\n");
        pntr_app_close(&app);
        return false;
    }

    struct retro_keyboard_callback keyboardCallback;
    keyboardCallback.callback = pntr_app_libretro_keyboard_callback;
    if (!environ_cb(RETRO_ENVIRONMENT_SET_KEYBOARD_CALLBACK, &keyboardCallback)) {
        log_cb(RETRO_LOG_INFO, "Failed to set keyboard callback.\n");
        pntr_app_close(&app);
        return false;
    }

    app.platform = pntr_load_memory(sizeof(pntr_app_libretro_platform));

    // Copy the data to the core's app instance.
    pntr_app_libretro = pntr_load_memory(sizeof(pntr_app));
    pntr_memory_copy(pntr_app_libretro, &app, sizeof(pntr_app));

    check_variables();

    return true;
}

void retro_unload_game(void) {
    pntr_app_close(pntr_app_libretro);
    pntr_app_libretro = NULL;
}

unsigned retro_get_region(void) {
    return RETRO_REGION_NTSC;
}

bool retro_load_game_special(unsigned type, const struct retro_game_info *info, size_t num) {
    (void)type;
    (void)info;
    (void)num;
    return retro_load_game(NULL);
}

size_t retro_serialize_size(void) {
    return 0;
}

bool retro_serialize(void *data, size_t size) {
    (void)data;
    (void)size;
    return true;
}

bool retro_unserialize(const void *data, size_t size) {
    (void)data;
    (void)size;
    return true;
}

void *retro_get_memory_data(unsigned id) {
    (void)id;
    return NULL;
}

size_t retro_get_memory_size(unsigned id) {
    (void)id;
    return 0;
}

void retro_cheat_reset(void) {
    // Nothing.
}

void retro_cheat_set(unsigned index, bool enabled, const char *code) {
    (void)index;
    (void)enabled;
    (void)code;
}
