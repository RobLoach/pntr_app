#ifndef PNTR_APP_SDL_H
#define PNTR_APP_SDL_H <SDL2/SDL.h>
#endif

#include PNTR_APP_SDL_H

SDL_Window* pntr_app_sdl_window;
SDL_Surface* pntr_app_sdl_screen;
SDL_Surface* pntr_app_sdl_surface;
uint64_t pntr_app_sdl_start;

typedef struct pntr_app_sdl_platform {
    int mouseX;
    int mouseY;
    SDL_GameController* gameControllers[4];
} pntr_app_sdl_platform;

pntr_app_gamepad_button pntr_app_sdl_gamepad_button(int button) {
    switch (button) {
        case SDL_CONTROLLER_BUTTON_A: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_FACE_DOWN;
        case SDL_CONTROLLER_BUTTON_B: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_FACE_RIGHT;
        case SDL_CONTROLLER_BUTTON_X: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_FACE_LEFT;
        case SDL_CONTROLLER_BUTTON_Y: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_FACE_UP;
        case SDL_CONTROLLER_BUTTON_BACK: return PNTR_APP_GAMEPAD_BUTTON_MIDDLE_LEFT;
        case SDL_CONTROLLER_BUTTON_GUIDE: return PNTR_APP_GAMEPAD_BUTTON_MIDDLE;
        case SDL_CONTROLLER_BUTTON_START: return PNTR_APP_GAMEPAD_BUTTON_MIDDLE_RIGHT;
        case SDL_CONTROLLER_BUTTON_LEFTSTICK: return PNTR_APP_GAMEPAD_BUTTON_LEFT_THUMB;
        case SDL_CONTROLLER_BUTTON_RIGHTSTICK: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_THUMB;
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: return PNTR_APP_GAMEPAD_BUTTON_LEFT_TRIGGER_1;
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_TRIGGER_1;
        case SDL_CONTROLLER_BUTTON_DPAD_UP: return PNTR_APP_GAMEPAD_BUTTON_LEFT_FACE_UP;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN: return PNTR_APP_GAMEPAD_BUTTON_LEFT_FACE_DOWN;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT: return PNTR_APP_GAMEPAD_BUTTON_LEFT_FACE_LEFT;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: return PNTR_APP_GAMEPAD_BUTTON_LEFT_FACE_RIGHT;
    }
    return PNTR_APP_GAMEPAD_BUTTON_UNKNOWN;
}

pntr_app_mouse_button pntr_app_sdl_mouse_button(int button) {
    switch (button) {
        case SDL_BUTTON_LEFT: return PNTR_APP_MOUSE_BUTTON_LEFT;
        case SDL_BUTTON_MIDDLE: return PNTR_APP_MOUSE_BUTTON_MIDDLE;
        case SDL_BUTTON_RIGHT: return PNTR_APP_MOUSE_BUTTON_RIGHT;
        default:
            return PNTR_APP_MOUSE_BUTTON_UNKNOWN;
    }
}

pntr_app_key pntr_app_sdl_key(SDL_KeyCode key) {
    switch (key) {
        case SDLK_SPACE: return PNTR_APP_KEY_SPACE;
        case SDLK_QUOTE: return PNTR_APP_KEY_APOSTROPHE;
        case SDLK_COMMA: return PNTR_APP_KEY_COMMA;
        case SDLK_MINUS: return PNTR_APP_KEY_MINUS;
        case SDLK_PERIOD: return PNTR_APP_KEY_PERIOD;
        case SDLK_SLASH: return PNTR_APP_KEY_SLASH;
        case SDLK_0: return PNTR_APP_KEY_0;
        case SDLK_1: return PNTR_APP_KEY_1;
        case SDLK_2: return PNTR_APP_KEY_2;
        case SDLK_3: return PNTR_APP_KEY_3;
        case SDLK_4: return PNTR_APP_KEY_4;
        case SDLK_5: return PNTR_APP_KEY_5;
        case SDLK_6: return PNTR_APP_KEY_6;
        case SDLK_7: return PNTR_APP_KEY_7;
        case SDLK_8: return PNTR_APP_KEY_8;
        case SDLK_9: return PNTR_APP_KEY_9;
        case SDLK_SEMICOLON: return PNTR_APP_KEY_SEMICOLON;
        case SDLK_EQUALS: return PNTR_APP_KEY_EQUAL;
        case SDLK_a: return PNTR_APP_KEY_A;
        case SDLK_b: return PNTR_APP_KEY_B;
        case SDLK_c: return PNTR_APP_KEY_C;
        case SDLK_d: return PNTR_APP_KEY_D;
        case SDLK_e: return PNTR_APP_KEY_E;
        case SDLK_f: return PNTR_APP_KEY_F;
        case SDLK_g: return PNTR_APP_KEY_G;
        case SDLK_h: return PNTR_APP_KEY_H;
        case SDLK_i: return PNTR_APP_KEY_I;
        case SDLK_j: return PNTR_APP_KEY_J;
        case SDLK_k: return PNTR_APP_KEY_K;
        case SDLK_l: return PNTR_APP_KEY_L;
        case SDLK_m: return PNTR_APP_KEY_M;
        case SDLK_n: return PNTR_APP_KEY_N;
        case SDLK_o: return PNTR_APP_KEY_O;
        case SDLK_p: return PNTR_APP_KEY_P;
        case SDLK_q: return PNTR_APP_KEY_Q;
        case SDLK_r: return PNTR_APP_KEY_R;
        case SDLK_s: return PNTR_APP_KEY_S;
        case SDLK_t: return PNTR_APP_KEY_T;
        case SDLK_u: return PNTR_APP_KEY_U;
        case SDLK_v: return PNTR_APP_KEY_V;
        case SDLK_w: return PNTR_APP_KEY_W;
        case SDLK_x: return PNTR_APP_KEY_X;
        case SDLK_y: return PNTR_APP_KEY_Y;
        case SDLK_z: return PNTR_APP_KEY_Z;
        case SDLK_LEFTBRACKET: return PNTR_APP_KEY_LEFT_BRACKET;
        case SDLK_BACKSLASH: return PNTR_APP_KEY_BACKSLASH;
        case SDLK_RIGHTBRACKET: return PNTR_APP_KEY_RIGHT_BRACKET;
        case SDLK_UNKNOWN: return PNTR_APP_KEY_GRAVE_ACCENT;
        case SDLK_ESCAPE: return PNTR_APP_KEY_ESCAPE;
        case SDLK_RETURN: return PNTR_APP_KEY_ENTER;
        case SDLK_TAB: return PNTR_APP_KEY_TAB;
        case SDLK_BACKSPACE: return PNTR_APP_KEY_BACKSPACE;
        case SDLK_INSERT: return PNTR_APP_KEY_INSERT;
        case SDLK_DELETE: return PNTR_APP_KEY_DELETE;
        case SDLK_RIGHT: return PNTR_APP_KEY_RIGHT;
        case SDLK_LEFT: return PNTR_APP_KEY_LEFT;
        case SDLK_DOWN: return PNTR_APP_KEY_DOWN;
        case SDLK_UP: return PNTR_APP_KEY_UP;
        case SDLK_PAGEUP: return PNTR_APP_KEY_PAGE_UP;
        case SDLK_PAGEDOWN: return PNTR_APP_KEY_PAGE_DOWN;
        case SDLK_HOME: return PNTR_APP_KEY_HOME;
        case SDLK_END: return PNTR_APP_KEY_END;
        case SDLK_CAPSLOCK: return PNTR_APP_KEY_CAPS_LOCK;
        case SDLK_SCROLLLOCK: return PNTR_APP_KEY_SCROLL_LOCK;
        case SDLK_PRINTSCREEN: return PNTR_APP_KEY_PRINT_SCREEN;
        case SDLK_PAUSE: return PNTR_APP_KEY_PAUSE;
        case SDLK_F1: return PNTR_APP_KEY_F1;
        case SDLK_F2: return PNTR_APP_KEY_F2;
        case SDLK_F3: return PNTR_APP_KEY_F3;
        case SDLK_F4: return PNTR_APP_KEY_F4;
        case SDLK_F5: return PNTR_APP_KEY_F5;
        case SDLK_F6: return PNTR_APP_KEY_F6;
        case SDLK_F7: return PNTR_APP_KEY_F7;
        case SDLK_F8: return PNTR_APP_KEY_F8;
        case SDLK_F9: return PNTR_APP_KEY_F9;
        case SDLK_F10: return PNTR_APP_KEY_F10;
        case SDLK_F11: return PNTR_APP_KEY_F11;
        case SDLK_F12: return PNTR_APP_KEY_F12;
        case SDLK_F13: return PNTR_APP_KEY_F13;
        case SDLK_F14: return PNTR_APP_KEY_F14;
        case SDLK_F15: return PNTR_APP_KEY_F15;
        case SDLK_F16: return PNTR_APP_KEY_F16;
        case SDLK_F17: return PNTR_APP_KEY_F17;
        case SDLK_F18: return PNTR_APP_KEY_F18;
        case SDLK_F19: return PNTR_APP_KEY_F19;
        case SDLK_F20: return PNTR_APP_KEY_F20;
        case SDLK_F21: return PNTR_APP_KEY_F21;
        case SDLK_F22: return PNTR_APP_KEY_F22;
        case SDLK_F23: return PNTR_APP_KEY_F23;
        case SDLK_F24: return PNTR_APP_KEY_F24;
        case SDLK_KP_0: return PNTR_APP_KEY_KP_0;
        case SDLK_KP_1: return PNTR_APP_KEY_KP_1;
        case SDLK_KP_2: return PNTR_APP_KEY_KP_2;
        case SDLK_KP_3: return PNTR_APP_KEY_KP_3;
        case SDLK_KP_4: return PNTR_APP_KEY_KP_4;
        case SDLK_KP_5: return PNTR_APP_KEY_KP_5;
        case SDLK_KP_6: return PNTR_APP_KEY_KP_6;
        case SDLK_KP_7: return PNTR_APP_KEY_KP_7;
        case SDLK_KP_8: return PNTR_APP_KEY_KP_8;
        case SDLK_KP_9: return PNTR_APP_KEY_KP_9;
        case SDLK_KP_DECIMAL: return PNTR_APP_KEY_KP_DECIMAL;
        case SDLK_KP_DIVIDE: return PNTR_APP_KEY_KP_DIVIDE;
        case SDLK_KP_MULTIPLY: return PNTR_APP_KEY_KP_MULTIPLY;
        case SDLK_KP_MINUS: return PNTR_APP_KEY_KP_SUBTRACT;
        case SDLK_KP_PLUS: return PNTR_APP_KEY_KP_ADD;
        case SDLK_KP_ENTER: return PNTR_APP_KEY_KP_ENTER;
        case SDLK_KP_EQUALS: return PNTR_APP_KEY_KP_EQUAL;
        case SDLK_LSHIFT: return PNTR_APP_KEY_LEFT_SHIFT;
        case SDLK_LCTRL: return PNTR_APP_KEY_LEFT_CONTROL;
        case SDLK_LALT: return PNTR_APP_KEY_LEFT_ALT;
        case SDLK_RSHIFT: return PNTR_APP_KEY_RIGHT_SHIFT;
        case SDLK_RCTRL: return PNTR_APP_KEY_RIGHT_CONTROL;
        case SDLK_RALT: return PNTR_APP_KEY_RIGHT_ALT;
        case SDLK_MENU: return PNTR_APP_KEY_MENU;
    }

    return PNTR_APP_KEY_INVALID;
}

bool pntr_app_events(pntr_app* app) {
    if (app == NULL) {
        return false;
    }

    if (app->fps > 0) {
        pntr_app_sdl_start = SDL_GetPerformanceCounter();
    }

    if (app->event == NULL) {
        return true;
    }

    pntr_app_sdl_platform* platform = (pntr_app_sdl_platform*)app->platform;
    pntr_app_event pntrEvent;
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_APP_TERMINATING:
                return false;
            case SDL_MOUSEMOTION: {
                pntrEvent.type = PNTR_APP_EVENTTYPE_MOUSE_MOVE;
                pntrEvent.mouseX = platform->mouseX = event.motion.x;
                pntrEvent.mouseY = platform->mouseX = event.motion.y;
                app->event(&pntrEvent, app->userData);
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP: {
                pntr_app_mouse_button button = pntr_app_sdl_mouse_button(event.button.button);
                if (button != PNTR_APP_MOUSE_BUTTON_UNKNOWN) {
                    pntrEvent.type = (event.type == SDL_MOUSEBUTTONDOWN) ? PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN : PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP;
                    pntrEvent.mouseButton = button;
                    pntrEvent.mouseX = platform->mouseX;
                    pntrEvent.mouseY = platform->mouseY;
                    app->event(&pntrEvent, app->userData);
                }
                break;
            }
            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP: {
                pntrEvent.gamepadButton = pntr_app_sdl_gamepad_button(event.cbutton.button);
                if (pntrEvent.gamepadButton != PNTR_APP_GAMEPAD_BUTTON_UNKNOWN) {
                    pntrEvent.type = (event.cbutton.state == SDL_PRESSED) ? PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_DOWN : PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_UP;
                    pntrEvent.gamepad = event.cbutton.which;
                    app->event(&pntrEvent, app->userData);
                }
                break;
            }
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    return false;
                }

                pntrEvent.key = pntr_app_sdl_key(event.key.keysym.sym);
                if (pntrEvent.key != PNTR_APP_KEY_INVALID) {
                    pntrEvent.type = (event.type == SDL_KEYDOWN) ? PNTR_APP_EVENTTYPE_KEY_DOWN : PNTR_APP_EVENTTYPE_KEY_UP;
                    app->event(&pntrEvent, app->userData);
                }
                break;
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

    SDL_BlitSurface(pntr_app_sdl_surface, NULL, pntr_app_sdl_screen, NULL);
    SDL_UpdateWindowSurface(pntr_app_sdl_window);

    // Limit the FPS
    if (app->fps > 0) {
        uint64_t end = SDL_GetPerformanceCounter();
        float elapsedMS = (end - pntr_app_sdl_start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        SDL_Delay((1.0f / app->fps) * 1000.0f - elapsedMS);
    }

    return true;
}

bool pntr_app_init(pntr_app* app) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER);
    pntr_app_sdl_window = SDL_CreateWindow(app->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, app->width, app->height, SDL_WINDOW_SHOWN);
    pntr_app_sdl_screen = SDL_GetWindowSurface(pntr_app_sdl_window);
    pntr_app_sdl_surface = SDL_CreateRGBSurfaceWithFormatFrom(app->screen->data, app->width, app->height, 8, app->screen->pitch, SDL_PIXELFORMAT_ARGB8888);

    app->platform = pntr_app_malloc(sizeof(pntr_app_sdl_platform));
    pntr_app_sdl_platform* platform = (pntr_app_sdl_platform*)app->platform;

    // GamePads
    for (int i = 0; i < 4; i++) {
        if (SDL_IsGameController(i)) {
            platform->gameControllers[i] = SDL_GameControllerOpen(i);
        }
    }

    return true;
}

void pntr_app_close(pntr_app* app) {

    if (app != NULL) {
        pntr_app_sdl_platform* platform = app->platform;
        if (platform) {

            // Close Gamepads
            for (int i = 0; i < 4; i++) {
                if (platform->gameControllers[i] != NULL) {
                    if (SDL_IsGameController(i)) {
                        SDL_GameControllerClose(platform->gameControllers[i]);
                        platform->gameControllers[i] = NULL;
                    }
                }
            }
        }
    }

    SDL_FreeSurface(pntr_app_sdl_surface);
    SDL_FreeSurface(pntr_app_sdl_screen);
    SDL_DestroyWindow(pntr_app_sdl_window);
}
