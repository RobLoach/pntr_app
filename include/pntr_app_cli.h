#include <stdlib.h> // realloc

// Termbox2
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#define tb_malloc  pntr_load_memory
#define tb_realloc realloc
#define tb_free    pntr_unload_memory
#define TB_IMPL
#include "external/termbox2.h"
#pragma GCC diagnostic pop

#include <stdio.h>

typedef struct pntr_app_cli_platform {
    int mouseX;
    int mouseY;
    bool keysEnabled[PNTR_APP_KEY_LAST];
    bool mouseButtonsPressed[PNTR_APP_MOUSE_BUTTON_LAST];
    bool termbox;
} pntr_app_cli_platform;

bool pntr_app_events(pntr_app* app) {
    if (app == NULL || app->platform == NULL) {
        return false;
    }

    if (app->event == NULL) {
        return true;
    }

    pntr_app_cli_platform* platform = (pntr_app_cli_platform*)app->platform;
    pntr_app_event event;

    // Key Up
    event.type = PNTR_APP_EVENTTYPE_KEY_UP;
    for (event.key = PNTR_APP_KEY_FIRST; event.key < PNTR_APP_KEY_LAST; event.key++) {
        if (platform->keysEnabled[event.key]) {
            platform->keysEnabled[event.key] = false;
            app->event(app, &event);
        }
    }

    // Mouse Button Up
    event.type = PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP;
    for (event.mouseButton = PNTR_APP_MOUSE_BUTTON_FIRST; event.mouseButton < PNTR_APP_MOUSE_BUTTON_LAST; event.mouseButton++) {
        if (platform->mouseButtonsPressed[event.mouseButton]) {
            platform->mouseButtonsPressed[event.mouseButton] = false;
            app->event(app, &event);
        }
    }

    struct tb_event ev;

    // Delay for an input event, for maintain the FPS.
    if (app->fps <= 0) {
        tb_poll_event(&ev);
    }
    else {
        int timeout = (int)(1.0f / (float)app->fps * 1000.0f);
        if (tb_peek_event(&ev, timeout) == TB_ERR_NO_EVENT) {
            ev.type = 0;
        }
    }

    // Process the event
    switch (ev.type) {
        case TB_EVENT_KEY: {
            event.type = PNTR_APP_EVENTTYPE_KEY_DOWN;
            event.key = PNTR_APP_KEY_INVALID;
            if (ev.key == TB_KEY_CTRL_C) return false;
            else if (ev.key == TB_KEY_CTRL_X) return false;
            else if (ev.key == TB_KEY_ENTER) event.key = PNTR_APP_KEY_ENTER;
            // else if (ev.key == TB_KEY_CTRL_M)
            // else if (ev.key == TB_KEY_CTRL_N)
            // else if (ev.key == TB_KEY_CTRL_O)
            // else if (ev.key == TB_KEY_CTRL_P)
            // else if (ev.key == TB_KEY_CTRL_Q)
            // else if (ev.key == TB_KEY_CTRL_R)
            // else if (ev.key == TB_KEY_CTRL_S)
            // else if (ev.key == TB_KEY_CTRL_T)
            // else if (ev.key == TB_KEY_CTRL_U)
            // else if (ev.key == TB_KEY_CTRL_V)
            // else if (ev.key == TB_KEY_CTRL_W)
            // else if (ev.key == TB_KEY_CTRL_X)
            // else if (ev.key == TB_KEY_CTRL_Y)
            else if (ev.key == TB_KEY_CTRL_Z) return false;
            else if (ev.key == TB_KEY_ESC) return false;
            // else if (ev.key == TB_KEY_CTRL_LSQ_BRACKET)
            // else if (ev.key == TB_KEY_CTRL_3)
            // else if (ev.key == TB_KEY_CTRL_4)
            // else if (ev.key == TB_KEY_CTRL_BACKSLASH)
            // else if (ev.key == TB_KEY_CTRL_5)
            // else if (ev.key == TB_KEY_CTRL_RSQ_BRACKET)
            // else if (ev.key == TB_KEY_CTRL_6)
            // else if (ev.key == TB_KEY_CTRL_7)
            // else if (ev.key == TB_KEY_CTRL_SLASH)
            else if (ev.key == TB_KEY_CTRL_UNDERSCORE) event.key = PNTR_APP_KEY_MINUS;
            else if (ev.key == TB_KEY_SPACE) event.key = PNTR_APP_KEY_SPACE;
            // else if (ev.key == TB_KEY_BACKSPACE2)
            // else if (ev.key == TB_KEY_CTRL_8)
            else if (ev.key == TB_KEY_F1) event.key = PNTR_APP_KEY_F1;
            else if (ev.key == TB_KEY_F2) event.key = PNTR_APP_KEY_F2;
            else if (ev.key == TB_KEY_F3) event.key = PNTR_APP_KEY_F3;
            else if (ev.key == TB_KEY_F4) event.key = PNTR_APP_KEY_F4;
            else if (ev.key == TB_KEY_F5) event.key = PNTR_APP_KEY_F5;
            else if (ev.key == TB_KEY_F6) event.key = PNTR_APP_KEY_F6;
            else if (ev.key == TB_KEY_F7) event.key = PNTR_APP_KEY_F7;
            else if (ev.key == TB_KEY_F8) event.key = PNTR_APP_KEY_F8;
            else if (ev.key == TB_KEY_F9) event.key = PNTR_APP_KEY_F9;
            else if (ev.key == TB_KEY_F10) event.key = PNTR_APP_KEY_F10;
            else if (ev.key == TB_KEY_F11) event.key = PNTR_APP_KEY_F11;
            else if (ev.key == TB_KEY_F12) event.key = PNTR_APP_KEY_F12;
            else if (ev.key == TB_KEY_INSERT) event.key = PNTR_APP_KEY_INSERT;
            else if (ev.key == TB_KEY_DELETE) event.key = PNTR_APP_KEY_DELETE;
            else if (ev.key == TB_KEY_HOME) event.key = PNTR_APP_KEY_HOME;
            else if (ev.key == TB_KEY_END) event.key = PNTR_APP_KEY_HOME;
            else if (ev.key == TB_KEY_PGUP) event.key = PNTR_APP_KEY_PAGE_UP;
            else if (ev.key == TB_KEY_PGDN) event.key = PNTR_APP_KEY_PAGE_DOWN;
            else if (ev.key == TB_KEY_ARROW_UP) event.key = PNTR_APP_KEY_UP;
            else if (ev.key == TB_KEY_ARROW_DOWN) event.key = PNTR_APP_KEY_DOWN;
            else if (ev.key == TB_KEY_ARROW_LEFT) event.key = PNTR_APP_KEY_LEFT;
            else if (ev.key == TB_KEY_ARROW_RIGHT) event.key = PNTR_APP_KEY_RIGHT;
            else if (ev.key == TB_KEY_BACK_TAB) event.key = PNTR_APP_KEY_TAB;
            // else if (ev.key == TB_KEY_MOUSE_LEFT) event.key = PNTR_APP_KEY_LEFT;
            // else if (ev.key == TB_KEY_MOUSE_RIGHT)
            // else if (ev.key == TB_KEY_MOUSE_MIDDLE)
            // else if (ev.key == TB_KEY_MOUSE_RELEASE)
            // else if (ev.key == TB_KEY_MOUSE_WHEEL_UP)
            // else if (ev.key == TB_KEY_MOUSE_WHEEL_DOWN)

            if (event.key != PNTR_APP_KEY_INVALID) {
                platform->keysEnabled[event.key] = true;
                app->event(app, &event);
            }
            else {
                // Lower vs uppercase
                if (ev.ch >= 97 && ev.ch <= 122) {
                    ev.ch -= 32;
                }
                event.key = ev.ch;
                platform->keysEnabled[event.key] = true;
                app->event(app, &event);
            }
        }
        break;

        case TB_EVENT_MOUSE: {
            // Mouse Move
            // TODO: Mouse Delta
            event.mouseX = ev.x;
            event.mouseY = ev.y;
            event.mouseButton = PNTR_APP_MOUSE_BUTTON_UNKNOWN;

            // Mouse Wheel
            if (ev.key == TB_KEY_MOUSE_WHEEL_UP || ev.key == TB_KEY_MOUSE_WHEEL_DOWN) {
                event.type = PNTR_APP_EVENTTYPE_MOUSE_WHEEL;
                event.mouseWheel = (ev.key == TB_KEY_MOUSE_WHEEL_UP) ? -1 : 1;
                app->event(app, &event);
                break;
            }

            if (ev.key == TB_KEY_MOUSE_LEFT) {
                event.mouseButton = PNTR_APP_MOUSE_BUTTON_LEFT;
            }
            else if (ev.key == TB_KEY_MOUSE_RIGHT) {
                event.mouseButton = PNTR_APP_MOUSE_BUTTON_RIGHT;
            }
            else if (ev.key == TB_KEY_MOUSE_MIDDLE) {
                event.mouseButton = PNTR_APP_MOUSE_BUTTON_MIDDLE;
            }

            if (event.mouseButton != PNTR_APP_MOUSE_BUTTON_UNKNOWN) {
                event.type = PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN;
                platform->mouseButtonsPressed[event.mouseButton] = true;
                app->event(app, &event);
            }
            else {
                event.type = PNTR_APP_EVENTTYPE_MOUSE_MOVE;
                app->event(app, &event);
            }
        }
        break;
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

    pntr_app_cli_platform* platform = (pntr_app_cli_platform*)app->platform;
    pntr_image* screen = app->screen;

    #ifdef PNTR_APP_CLI_REVERSE_CHARACTERS
    const char* characters = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
    #else
    const char* characters = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
    #endif

    int charactersLen = 70;

    // TODO: Scale the image to the size of the terminal?

    // Get the greyscale representation of the screen
    unsigned char* grayscaleImage = (unsigned char*)pntr_image_to_pixelformat(screen, NULL, PNTR_PIXELFORMAT_GRAYSCALE);
    if (grayscaleImage == NULL) {
        return false;
    }

    // Clear the terminal
    if (!platform->termbox) {
        send_clear();
    }

    // Output the characters to the terminal
    for (int y = 0; y < app->height; y++) {
        for (int x = 0; x < app->width; x++) {
            unsigned char pixelIntensity = grayscaleImage[y * app->width + x];
            int char_index = (int)pixelIntensity * (charactersLen - 1) / 255;

            // TODO: Have it set the background/foreground color
            tb_set_cell(x, y, (uint32_t)characters[char_index], TB_WHITE, TB_BLACK); //TB_BLACK, TB_WHITE);

            if (!platform->termbox) {
                printf("%c", characters[char_index]);
            }
        }

        if (!platform->termbox) {
            printf("\n");
        }
    }

    // Present the characters
    tb_present();

    pntr_unload_memory(grayscaleImage);

    return true;
}

bool pntr_app_init(pntr_app* app) {
    if (app == NULL) {
        return false;
    }

    app->platform = pntr_load_memory(sizeof(pntr_app_cli_platform));
    pntr_app_cli_platform* platform = (pntr_app_cli_platform*)app->platform;

    //platform->termbox = tb_init_file("TermTox failure test") == TB_OK;
    platform->termbox = tb_init() == TB_OK;

    tb_set_input_mode(TB_INPUT_ESC | TB_INPUT_MOUSE);

    return true;
}

void pntr_app_close(pntr_app* app) {
    tb_shutdown();

    if (app == NULL) {
        return;
    }

    pntr_unload_memory(app->platform);
    app->platform = NULL;
}

pntr_sound* pntr_load_sound_from_memory(const char* fileName, unsigned char* data, unsigned int dataSize) {
    (void)fileName;
    if (data == NULL || dataSize <= 0) {
        return NULL;
    }

    pntr_sound* sound = (pntr_sound*)pntr_load_memory(sizeof(char));
    if (sound == NULL) {
        pntr_unload_file(data);
        return NULL;
    }

    // TODO: Audio support for the Command line interface?
    // We just free the memory as there is no CLI audio right now.
    pntr_unload_file(data);

    // Save the path within the sound.
    return sound;
}

void pntr_unload_sound(pntr_sound* sound) {
    if (sound == NULL) {
        return;
    }

    pntr_unload_memory(sound);
}

void pntr_play_sound(pntr_sound* sound, bool loop) {
    // Do nothing.
    (void)sound;
    (void)loop;
}

void pntr_stop_sound(pntr_sound* sound) {
    (void)sound;
}
