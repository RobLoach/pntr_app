#ifdef PNTR_APP_SDL
#ifndef PNTR_APP_SDL_H__
#define PNTR_APP_SDL_H__

// SDL.h
#ifndef PNTR_APP_SDL_H
#define PNTR_APP_SDL_H <SDL3/SDL.h>
#endif
#include PNTR_APP_SDL_H

// pntr configuration
#ifndef PNTR_FREE
    #define PNTR_FREE pntr_app_sdl_free
#endif
#ifndef PNTR_MALLOC
    #define PNTR_MALLOC pntr_app_sdl_malloc
#endif
#ifndef PNTR_LOAD_FILE
    #define PNTR_LOAD_FILE pntr_app_sdl_load_file
#endif
#ifndef PNTR_SAVE_FILE
    #define PNTR_SAVE_FILE(fileName, data, bytesToWrite) pntr_app_sdl_save_file(fileName, data, bytesToWrite)
#endif

typedef struct pntr_app_sdl_platform {
    SDL_Gamepad* gameControllers[PNTR_APP_MAX_GAMEPADS];
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    uint64_t timerLastTime;
    SDL_AudioSpec audioSpec;
} pntr_app_sdl_platform;

void pntr_app_sdl_free(void* ptr);
void* pntr_app_sdl_malloc(size_t size);
unsigned char* pntr_app_sdl_load_file(const char* fileName, unsigned int* bytesRead);
bool pntr_app_sdl_save_file(const char *fileName, const void *data, unsigned int bytesToWrite);

#endif  // PNTR_APP_SDL_H__

#if defined(PNTR_APP_IMPLEMENTATION) && !defined(PNTR_APP_HEADER_ONLY)
#ifndef PNTR_APP_SDL_IMPLEMENTATION_ONCE
#define PNTR_APP_SDL_IMPLEMENTATION_ONCE

// SDL_mixer.h
#ifdef PNTR_APP_SDL_MIXER
    #ifndef PNTR_APP_SDL_MIXER_H
        #define PNTR_APP_SDL_MIXER_H "SDL3_mixer/SDL_mixer.h"
    #endif
    #include PNTR_APP_SDL_MIXER_H
#endif

/**
 * Free the given memory pointer using SDL.
 */
void pntr_app_sdl_free(void* ptr) {
    SDL_free(ptr);
}

/**
 * Allocate memory using SDL's malloc.
 */
void* pntr_app_sdl_malloc(size_t size) {
    return SDL_malloc(size);
}

/**
 * Load a file using SDL.
 */
unsigned char* pntr_app_sdl_load_file(const char* fileName, unsigned int* bytesRead) {
    size_t dataSize;
    void* output = SDL_LoadFile(fileName, &dataSize);

    if (bytesRead != NULL) {
        if (output != NULL) {
            *bytesRead = (unsigned int)dataSize;
        }
        else {
            *bytesRead = 0;
        }
    }

    return (unsigned char*)output;
}

/**
 * Save a file using SDL.
 */
bool pntr_app_sdl_save_file(const char *fileName, const void *data, unsigned int bytesToWrite) {
    return SDL_SaveFile(fileName, data, (size_t)bytesToWrite);
}

#ifndef PNTR_APP_LOG
    void pntr_app_sdl_log(pntr_app_log_type type, const char* message) {
        switch (type) {
            case PNTR_APP_LOG_INFO:
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s", message);
            break;
            case PNTR_APP_LOG_WARNING:
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", message);
            break;
            case PNTR_APP_LOG_ERROR:
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", message);
            break;
            case PNTR_APP_LOG_DEBUG:
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s", message);
            break;
        }
    }
    #define PNTR_APP_LOG pntr_app_sdl_log
#endif

pntr_app_gamepad_button pntr_app_sdl_gamepad_button(int button) {
    switch (button) {
        case SDL_GAMEPAD_BUTTON_SOUTH: return PNTR_APP_GAMEPAD_BUTTON_A;
        case SDL_GAMEPAD_BUTTON_EAST: return PNTR_APP_GAMEPAD_BUTTON_B;
        case SDL_GAMEPAD_BUTTON_WEST: return PNTR_APP_GAMEPAD_BUTTON_X;
        case SDL_GAMEPAD_BUTTON_NORTH: return PNTR_APP_GAMEPAD_BUTTON_Y;
        case SDL_GAMEPAD_BUTTON_BACK: return PNTR_APP_GAMEPAD_BUTTON_SELECT;
        case SDL_GAMEPAD_BUTTON_GUIDE: return PNTR_APP_GAMEPAD_BUTTON_MENU;
        case SDL_GAMEPAD_BUTTON_START: return PNTR_APP_GAMEPAD_BUTTON_START;
        case SDL_GAMEPAD_BUTTON_LEFT_STICK: return PNTR_APP_GAMEPAD_BUTTON_LEFT_THUMB;
        case SDL_GAMEPAD_BUTTON_RIGHT_STICK: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_THUMB;
        case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER: return PNTR_APP_GAMEPAD_BUTTON_LEFT_SHOULDER;
        case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_SHOULDER;
        case SDL_GAMEPAD_BUTTON_DPAD_UP: return PNTR_APP_GAMEPAD_BUTTON_UP;
        case SDL_GAMEPAD_BUTTON_DPAD_DOWN: return PNTR_APP_GAMEPAD_BUTTON_DOWN;
        case SDL_GAMEPAD_BUTTON_DPAD_LEFT: return PNTR_APP_GAMEPAD_BUTTON_LEFT;
        case SDL_GAMEPAD_BUTTON_DPAD_RIGHT: return PNTR_APP_GAMEPAD_BUTTON_RIGHT;
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

pntr_app_key pntr_app_sdl_key(SDL_Keycode key) {
    switch (key) {
        case SDLK_SPACE: return PNTR_APP_KEY_SPACE;
        case SDLK_APOSTROPHE: return PNTR_APP_KEY_APOSTROPHE;
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
        case SDLK_A: return PNTR_APP_KEY_A;
        case SDLK_B: return PNTR_APP_KEY_B;
        case SDLK_C: return PNTR_APP_KEY_C;
        case SDLK_D: return PNTR_APP_KEY_D;
        case SDLK_E: return PNTR_APP_KEY_E;
        case SDLK_F: return PNTR_APP_KEY_F;
        case SDLK_G: return PNTR_APP_KEY_G;
        case SDLK_H: return PNTR_APP_KEY_H;
        case SDLK_I: return PNTR_APP_KEY_I;
        case SDLK_J: return PNTR_APP_KEY_J;
        case SDLK_K: return PNTR_APP_KEY_K;
        case SDLK_L: return PNTR_APP_KEY_L;
        case SDLK_M: return PNTR_APP_KEY_M;
        case SDLK_N: return PNTR_APP_KEY_N;
        case SDLK_O: return PNTR_APP_KEY_O;
        case SDLK_P: return PNTR_APP_KEY_P;
        case SDLK_Q: return PNTR_APP_KEY_Q;
        case SDLK_R: return PNTR_APP_KEY_R;
        case SDLK_S: return PNTR_APP_KEY_S;
        case SDLK_T: return PNTR_APP_KEY_T;
        case SDLK_U: return PNTR_APP_KEY_U;
        case SDLK_V: return PNTR_APP_KEY_V;
        case SDLK_W: return PNTR_APP_KEY_W;
        case SDLK_X: return PNTR_APP_KEY_X;
        case SDLK_Y: return PNTR_APP_KEY_Y;
        case SDLK_Z: return PNTR_APP_KEY_Z;
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
        //case SDLK_F11: return PNTR_APP_KEY_F11; // Reserved for fullscreen
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
        default:
            return PNTR_APP_KEY_INVALID;
    }

    return PNTR_APP_KEY_INVALID;
}

void pntr_app_platform_get_destination(pntr_image* screen, pntr_app_sdl_platform* platform, SDL_FRect* outRect) {
    // Find the aspect ratio.
    float aspect = (float)screen->width / (float)screen->height;
    if (aspect <= 0) {
        aspect = (float)screen->height / (float)screen->width;
    }

    int windowWidth;
    int windowHeight;
    SDL_GetRenderOutputSize(platform->renderer, &windowWidth, &windowHeight);

    // Calculate the optimal width/height to display in the screen size.
    float height = (float)windowHeight;
    float width = height * aspect;
    if (width > windowWidth) {
        height = (float)windowWidth / aspect;
        width = windowWidth;
    }

    // Draw the texture in the middle of the screen.
    outRect->x = ((float)windowWidth - width) / 2.0f;
    outRect->y = ((float)windowHeight - height) / 2.0f;
    outRect->w = width;
    outRect->h = height;
}

void pntr_app_platform_render_surface(pntr_app* app, pntr_app_sdl_platform* platform) {
    void* pixels;
    int pitch;
    if (platform->renderer == NULL) {
        return;
    }

    if (app->screen == NULL) {
        SDL_RenderClear(platform->renderer);
        SDL_RenderPresent(platform->renderer);
        return;
    }

    if (platform->texture == NULL) {
        platform->texture = SDL_CreateTexture(platform->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, app->screen->width, app->screen->height);
        if (platform->texture == NULL) {
            SDL_RenderClear(platform->renderer);
            SDL_RenderPresent(platform->renderer);
            return;
        }
    }

    // Update the Texture
    if (!SDL_LockTexture(platform->texture, NULL, &pixels, &pitch)) {
        SDL_RenderClear(platform->renderer);
        SDL_RenderPresent(platform->renderer);
        return;
    }

    uint32_t* destinationPixels = pixels;
    uint32_t* sourcePixels = (uint32_t*)app->screen->data;
    for (int i = 0; i < app->screen->width * app->screen->height; i++) {
        destinationPixels[i] = sourcePixels[i];
    }
    SDL_UnlockTexture(platform->texture);

    SDL_FRect dstRect;
    pntr_app_platform_get_destination(app->screen, platform, &dstRect);

    SDL_RenderClear(platform->renderer);
    SDL_RenderTexture(platform->renderer, platform->texture, NULL, &dstRect);
    SDL_RenderPresent(platform->renderer);
}


void pntr_app_platform_fix_mouse_coordinates(pntr_app* app, pntr_app_event* event, SDL_Window* window, SDL_MouseMotionEvent* mouseMotion) {
    SDL_FRect dstRect;
    pntr_app_platform_get_destination(app->screen, app->platform, &dstRect);

    if (SDL_GetWindowRelativeMouseMode(window)) {
        event->mouseDeltaX = (float)mouseMotion->xrel;
        event->mouseDeltaY = (float)mouseMotion->yrel;
    }
    else {
        if (mouseMotion->x < dstRect.x) {
            mouseMotion->x = dstRect.x;
        }
        else if (mouseMotion->x > dstRect.x + dstRect.w) {
            mouseMotion->x = dstRect.x + dstRect.w;
        }

        if (mouseMotion->y < dstRect.y) {
            mouseMotion->y = dstRect.y;
        }
        else if (mouseMotion->y > dstRect.y + dstRect.h) {
            mouseMotion->y = dstRect.y + dstRect.h;
        }

        event->mouseX = (mouseMotion->x - dstRect.x) * app->screen->width / dstRect.w;
        event->mouseY = (mouseMotion->y - dstRect.y) * app->screen->height / dstRect.h;
    }
}

#ifndef PNTR_APP_SHOW_MOUSE
    bool pntr_app_platform_show_mouse(pntr_app* app, bool show) {
        (void)app;
        if (show) {
            return SDL_ShowCursor();
        }

        return SDL_HideCursor();
    }
    #define PNTR_APP_SHOW_MOUSE pntr_app_platform_show_mouse
#endif

#ifndef PNTR_APP_SET_CLIPBOARD
    void pntr_app_platform_set_clipboard(pntr_app* app, const char* text) {
        (void)app;
        SDL_SetClipboardText(text);
    }
    #define PNTR_APP_SET_CLIPBOARD pntr_app_platform_set_clipboard
#endif

#ifndef PNTR_APP_CLIPBOARD
    const char* pntr_app_platform_clipboard(pntr_app* app) {
        (void)app;
        if (!SDL_HasClipboardText()) {
            return NULL;
        }

        // Don't need to copy the string since SDL creates the memory for us.
        return SDL_GetClipboardText();
    }
    #define PNTR_APP_CLIPBOARD pntr_app_platform_clipboard
#endif

bool pntr_app_platform_events(pntr_app* app) {
    if (app == NULL || app->platform == NULL) {
        return false;
    }

    pntr_app_sdl_platform* platform = (pntr_app_sdl_platform*)app->platform;
    pntr_app_event pntrEvent = {0};
    pntrEvent.app = app;
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
            case SDL_EVENT_TERMINATING:
                return false;

            case SDL_EVENT_MOUSE_MOTION:
                pntrEvent.type = PNTR_APP_EVENTTYPE_MOUSE_MOVE;
                pntr_app_platform_fix_mouse_coordinates(app, &pntrEvent, platform->window, &event.motion);
                pntrEvent.mouseWheel = 0;
                pntr_app_process_event(app, &pntrEvent);
            break;

            case SDL_EVENT_MOUSE_WHEEL:
                pntrEvent.type = PNTR_APP_EVENTTYPE_MOUSE_WHEEL;
                pntrEvent.mouseWheel = event.wheel.y > 0 ? 1 : -1;
                pntr_app_process_event(app, &pntrEvent);
            break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            case SDL_EVENT_MOUSE_BUTTON_UP: {
                pntr_app_mouse_button button = pntr_app_sdl_mouse_button(event.button.button);
                if (button != PNTR_APP_MOUSE_BUTTON_UNKNOWN) {
                    pntrEvent.type = (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) ? PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN : PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP;
                    pntrEvent.mouseButton = button;
                    pntr_app_process_event(app, &pntrEvent);
                }
            }
            break;

            case SDL_EVENT_GAMEPAD_BUTTON_UP:
            case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                pntrEvent.gamepadButton = pntr_app_sdl_gamepad_button(event.gbutton.button);
                if (pntrEvent.gamepadButton != PNTR_APP_GAMEPAD_BUTTON_UNKNOWN) {
                    pntrEvent.type = event.gbutton.down ? PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_DOWN : PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_UP;
                    pntrEvent.gamepad = event.gbutton.which;
                    pntr_app_process_event(app, &pntrEvent);
                }
            break;

            case SDL_EVENT_KEY_DOWN:
            case SDL_EVENT_KEY_UP:
                // Don't process key repeats.
                if (event.type == SDL_EVENT_KEY_DOWN && event.key.repeat) {
                    return true;
                }

                // Escape key quits.
                if (event.key.key == SDLK_ESCAPE) {
                    return false;
                }

                // Fullscreen
                if (event.key.key == SDLK_F11) {
                    if (event.type == SDL_EVENT_KEY_UP) {
                        uint32_t windowFlags = SDL_GetWindowFlags(platform->window);
                        if ((windowFlags & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN) {
                            SDL_SetWindowFullscreen(platform->window, false);
                        }
                        else {
                            SDL_SetWindowFullscreen(platform->window, true);
                        }
                        break;
                    }
                    else {
                        break;
                    }
                }

                pntrEvent.key = pntr_app_sdl_key(event.key.key);
                if (pntrEvent.key != PNTR_APP_KEY_INVALID) {
                    pntrEvent.type = event.key.down ? PNTR_APP_EVENTTYPE_KEY_DOWN : PNTR_APP_EVENTTYPE_KEY_UP;
                    pntr_app_process_event(app, &pntrEvent);
                }
            break;

            case SDL_EVENT_WINDOW_RESIZED:
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
            case SDL_EVENT_WINDOW_EXPOSED:
                if (platform->texture != NULL) {
                    SDL_DestroyTexture(platform->texture);
                    platform->texture = NULL;
                }
            break;

            case SDL_EVENT_DROP_FILE: {
                pntrEvent.type = PNTR_APP_EVENTTYPE_FILE_DROPPED;
                pntrEvent.fileDropped = event.drop.data;
                if (pntrEvent.fileDropped != NULL) {
                    pntr_app_process_event(app, &pntrEvent);
                }
            }
            break;
        }
    }

    return true;
}

/**
 * Pushes the given image to the screen.
 */
bool pntr_app_platform_render(pntr_app* app) {
    if (app == NULL || app->screen == NULL || app->platform == NULL) {
        return false;
    }

    pntr_app_sdl_platform* platform = (pntr_app_sdl_platform*)app->platform;
    pntr_app_platform_render_surface(app, platform);

    return true;
}

bool pntr_app_platform_init(pntr_app* app) {
    if (app == NULL) {
        return false;
    }

    // Prepare platform data.
    app->platform = pntr_load_memory(sizeof(pntr_app_sdl_platform));
    pntr_app_sdl_platform* platform = (pntr_app_sdl_platform*)app->platform;
    if (platform == NULL) {
        return false;
    }

    // SDL_Init
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD | SDL_INIT_AUDIO)) {
        pntr_unload_memory(platform);
        app->platform = NULL;
        return false;
    }

    // Window and Renderer
    if (!SDL_CreateWindowAndRenderer(app->title, app->width * 2, app->height * 2, SDL_WINDOW_RESIZABLE, &platform->window, &platform->renderer)) {
        SDL_Quit();
        pntr_unload_memory(platform);
        app->platform = NULL;
        return false;
    }

    // Texture
    platform->texture = SDL_CreateTexture(platform->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, app->width, app->height);
    if (platform->texture == NULL) {
        SDL_DestroyRenderer(platform->renderer);
        SDL_DestroyWindow(platform->window);
        SDL_Quit();
        pntr_unload_memory(platform);
        app->platform = NULL;
        return false;
    }

    // GamePads
    for (int i = 0; i < 4; i++) {
        if (SDL_IsGamepad(i)) {
            platform->gameControllers[i] = SDL_OpenGamepad(i);
        }
    }

    // Start the tick counter for the delta time.
    platform->timerLastTime = SDL_GetTicksNS();

    // Random Number Generator
    pntr_app_random_set_seed(app, (uint64_t)SDL_GetPerformanceCounter());

    return true;
}

void pntr_app_platform_close(pntr_app* app) {
    if (app != NULL) {
        pntr_app_sdl_platform* platform = (pntr_app_sdl_platform*)app->platform;
        if (platform != NULL) {
            // Close Gamepads
            for (int i = 0; i < 4; i++) {
                if (platform->gameControllers[i] != NULL) {
                    if (SDL_IsGamepad(i)) {
                        SDL_CloseGamepad(platform->gameControllers[i]);
                        platform->gameControllers[i] = NULL;
                    }
                }
            }

            if (platform->texture != NULL) {
                SDL_DestroyTexture(platform->texture);
                platform->texture = NULL;
            }

            if (platform->renderer != NULL) {
                SDL_DestroyRenderer(platform->renderer);
            }

            // Window
            if (platform->window != NULL) {
                SDL_DestroyWindow(platform->window);
            }

            // Platform
            pntr_unload_memory(platform);
        }
    }

    // SDL
    SDL_Quit();
}

/**
 * Internal structure for handling SDL audio.
 *
 * @internal
 */
typedef struct pntr_sound_sdl {
    #ifdef PNTR_APP_SDL_MIXER
        Mix_Chunk* chunk;
        int channel;
    #else
        Uint8* audio_buf;
        Uint32 audio_len;
        SDL_AudioDeviceID deviceId;
    #endif
} pntr_sound_sdl;

#ifndef PNTR_APP_LOAD_SOUND_FROM_MEMORY
#define PNTR_APP_LOAD_SOUND_FROM_MEMORY pntr_app_sdl_load_sound_from_memory
pntr_sound* pntr_app_sdl_load_sound_from_memory(pntr_app_sound_type type, unsigned char* data, unsigned int dataSize) {
    (void)type;
    if (data == NULL || dataSize <= 0) {
        return NULL;
    }

    SDL_IOStream* io = SDL_IOFromMem(data, (size_t)dataSize);
    if (io == NULL) {
        pntr_unload_file(data);
        return NULL;
    }

    pntr_sound_sdl* output = (pntr_sound_sdl*)pntr_load_memory(sizeof(pntr_sound_sdl));
    if (output == NULL) {
        SDL_CloseIO(io);
        return NULL;
    }

    #ifdef PNTR_APP_SDL_MIXER
        Mix_Chunk* chunk = Mix_LoadWAV_IO(io, 1);
        pntr_unload_file(data);
        if (chunk == NULL) {
            pntr_unload_memory(output);
            return NULL;
        }

        output->chunk = chunk;
        output->channel = -1;
    #else
        if (!SDL_LoadWAV_IO(io,
                1, // freesrc
                &platform->audioSpec,
                &output->audio_buf,
                &output->audio_len)) {
            pntr_unload_file(data);
            pntr_unload_memory(output);
            return NULL;
        }
        output->deviceId = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &output->audioSpec);
    #endif

    return (pntr_sound*)output;
}
#endif

#ifndef PNTR_APP_UNLOAD_SOUND
#define PNTR_APP_UNLOAD_SOUND pntr_app_sdl_unload_sound
void pntr_app_sdl_unload_sound(pntr_sound* sound) {
    if (sound == NULL) {
        return;
    }

    pntr_sound_sdl* audio = (pntr_sound_sdl*)sound;
    #ifdef PNTR_APP_SDL_MIXER
        Mix_FreeChunk(audio->chunk);
    #else
        SDL_CloseAudioDevice(audio->deviceId);
        SDL_FreeWAV(audio->audio_buf);
    #endif
    pntr_unload_memory((void*)sound);
}
#endif

#ifndef PNTR_APP_INIT_AUDIO
#define PNTR_APP_INIT_AUDIO pntr_app_sdl_init_audio
void pntr_app_sdl_init_audio(pntr_app* app) {
    pntr_app_sdl_platform* platform = (pntr_app_sdl_platform*)app->platform;
    #ifdef PNTR_APP_SDL_MIXER
        // platform->spec.freq = MIX_DEFAULT_FREQUENCY;
        // platform->spec.format = MIX_DEFAULT_FORMAT;
        // platform->spec.channels = MIX_DEFAULT_CHANNELS;
        Mix_OpenAudio(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &platform->audioSpec);
    #endif
}
#endif

#ifndef PNTR_APP_CLOSE_AUDIO
#define PNTR_APP_CLOSE_AUDIO pntr_app_sdl_close_audio
void pntr_app_sdl_close_audio(pntr_app* app) {
    (void)app;
    #ifdef PNTR_APP_SDL_MIXER
        Mix_HaltChannel(-1);
        Mix_CloseAudio();
    #else
        SDL_PauseAudio(1);
        SDL_CloseAudio();
    #endif
}
#endif

#ifndef PNTR_APP_PLAY_SOUND
#define PNTR_APP_PLAY_SOUND pntr_app_sdl_play_sound
void pntr_app_sdl_play_sound(pntr_sound* sound, bool loop) {
    if (sound == NULL) {
        return;
    }

    pntr_sound_sdl* audio = (pntr_sound_sdl*)sound;
    #ifdef PNTR_APP_SDL_MIXER
        audio->channel = Mix_PlayChannel(-1, audio->chunk, loop ? -1 : 0);
    #else
        // TODO: Add sound looping to SDL Queue Audio.
        pntr_stop_sound(sound);
        /*int success =*/ SDL_QueueAudio(audio->deviceId, audio->audio_buf, audio->audio_len);
        SDL_PauseAudioDevice(audio->deviceId, loop ? 0 : 0);
    #endif
}
#endif

#ifndef PNTR_APP_STOP_SOUND
#define PNTR_APP_STOP_SOUND pntr_app_sdl_stop_sound
void pntr_app_sdl_stop_sound(pntr_sound* sound) {
    if (sound == NULL) {
        return;
    }

    pntr_sound_sdl* audio = (pntr_sound_sdl*)sound;
    #ifdef PNTR_APP_SDL_MIXER
        if (audio->channel >= 0) {
            Mix_Pause(audio->channel);
            audio->channel = -1;
        }
    #else
        SDL_ClearQueuedAudio(audio->deviceId);
    #endif
}
#endif

bool pntr_app_platform_update_delta_time(pntr_app* app) {
    if (app == NULL || app->platform == NULL) {
        return false;
    }

    pntr_app_sdl_platform* platform = app->platform;

    uint64_t now = SDL_GetTicksNS();
    uint64_t delta = now - platform->timerLastTime;

    // Calculate if it's time to update
    if (app->fps <= 0 || (delta > (1000.0f / (float)app->fps))) {
        platform->timerLastTime = now;
        app->deltaTime = (float)delta / 1000.0f;
        return true;
    }

    return false;
}

PNTR_APP_API void pntr_app_set_icon(pntr_app* app, pntr_image* icon) {
    if (app == NULL || icon == NULL || app->platform == NULL) {
        return;
    }

    pntr_app_sdl_platform* platform = app->platform;
    if (platform->window == NULL) {
        return;
    }

    SDL_Surface* sdlIcon = SDL_CreateSurfaceFrom(icon->width, icon->height, SDL_PIXELFORMAT_ARGB8888, icon->data, icon->pitch);
    if (sdlIcon != NULL) {
        SDL_SetWindowIcon(platform->window, sdlIcon);
        SDL_DestroySurface(sdlIcon);
    }
}

PNTR_APP_API void pntr_app_set_title(pntr_app* app, const char* title) {
    if (app == NULL || title == NULL || app->platform == NULL) {
        return;
    }

    app->title = title;
    pntr_app_sdl_platform* platform = app->platform;
    if (platform->window == NULL) {
        return;
    }

    SDL_SetWindowTitle(platform->window, title);
}

bool pntr_app_platform_set_size(pntr_app* app, int width, int height) {
    if (app == NULL || app->platform == NULL) {
        return false;
    }

    pntr_app_sdl_platform* platform = (pntr_app_sdl_platform*)app->platform;
    if (platform->window == NULL) {
        return false;
    }

    SDL_SetWindowSize(platform->window, width, height);

    // Recreate the screen texture
    if (platform->texture != NULL) {
        SDL_DestroyTexture(platform->texture);
    }
    platform->texture = NULL;

    return true;
}

#endif  // PNTR_APP_SDL_IMPLEMENTATION_ONCE
#endif  // PNTR_APP_IMPLEMENTATION && !PNTR_APP_HEADER_ONLY
#endif  // PNTR_APP_SDL
