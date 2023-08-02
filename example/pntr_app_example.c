#include <stdio.h>

#define PNTR_APP_IMPLEMENTATION
#define PNTR_ENABLE_DEFAULT_FONT
#define PNTR_DISABLE_MATH
#include "pntr_app.h"

typedef struct AppData {
    pntr_image* logo;
    pntr_font* font;
    bool spacePressed;
    int x;
    pntr_sound* sound;
    pntr_music* music;
} AppData;

bool Init(void* userData) {
    AppData* appData = (AppData*)userData;

    appData->logo = pntr_load_image("resources/logo.png");
    appData->font = pntr_load_font_default();
    appData->spacePressed = false;
    appData->x = 0;
    appData->sound = pntr_load_sound("resources/sound.wav");
    appData->music = pntr_load_sound("resources/music.ogg");
    pntr_play_sound(appData->music);

    return true;
}

bool Update(pntr_image* screen, void* userData) {
    AppData* appData = (AppData*)userData;

    // Clear the screen
    pntr_clear_background(screen, PNTR_RAYWHITE);

    // Draw some text
    pntr_draw_text(screen, appData->font, "Congrats! You created your first pntr_app!", 35, screen->height - 30, PNTR_DARKGRAY);

    // Draw the logo
    if (appData->logo) {
        pntr_draw_image(screen, appData->logo, appData->x++, screen->height / 2 - appData->logo->height / 2);
    }

    if (appData->spacePressed) {
        pntr_draw_text(screen, appData->font, "Space is Pressed!", 10, 10, PNTR_BLACK);
    }
    else {
        pntr_draw_text(screen, appData->font, "Space is not pressed", 10, 10, PNTR_BLACK);
    }

    return true;
}

void Close(void* userData) {
    AppData* appData = (AppData*)userData;

    pntr_unload_image(appData->logo);
    pntr_unload_font(appData->font);
    pntr_unload_sound(appData->sound);
    pntr_unload_sound(appData->music);
}

void Event(pntr_app_event* event, void* userData) {
    AppData* appData = (AppData*)userData;

    switch (event->type) {
        case PNTR_APP_EVENTTYPE_KEY_DOWN: {
            if (event->key == PNTR_APP_KEY_SPACE) {
                appData->spacePressed = true;
            }

            pntr_play_sound(appData->sound);

            printf("Key Pressed: %c\n", (char)event->key);
        }
        break;

        case PNTR_APP_EVENTTYPE_KEY_UP: {
            if (event->key == PNTR_APP_KEY_SPACE) {
                appData->spacePressed = false;
            }
            printf("Key Released: %c\n", (char)event->key);
        }
        break;

        case PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN:
        case PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP: {
            const char* buttonDown = event->type == PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN ? "Pressed" : "Released";

            const char* button;
            switch (event->mouseButton) {
                case PNTR_APP_MOUSE_BUTTON_LEFT: button = "left"; break;
                case PNTR_APP_MOUSE_BUTTON_RIGHT: button = "right"; break;
                case PNTR_APP_MOUSE_BUTTON_MIDDLE: button = "middle"; break;
                case PNTR_APP_MOUSE_BUTTON_LAST:
                case PNTR_APP_MOUSE_BUTTON_UNKNOWN: button = "unknown"; break;
            }
            printf("Mouse Button %s: %s\n", buttonDown, button);

            if (event->type == PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN) {
                if (event->mouseButton == PNTR_APP_MOUSE_BUTTON_LEFT) {
                    pntr_play_sound(appData->sound);
                }
                else if (event->mouseButton == PNTR_APP_MOUSE_BUTTON_RIGHT) {
                    pntr_stop_sound(appData->music);
                    pntr_play_sound(appData->music);
                }
            }
        }
        break;

        case PNTR_APP_EVENTTYPE_MOUSE_MOVE: {
            printf("Mouse Move: (%d, %d) | (%d, %d)\n", event->mouseX, event->mouseY, event->mouseDeltaX, event->mouseDeltaY);
        }
        break;

        case PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_UP:
        case PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_DOWN: {
            printf("Gamepad: %d. Button: %d %s\n", event->gamepad, event->gamepadButton, event->type == PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_DOWN ? "Pressed" : "Released");
        }
        break;

        default: {
            printf("Unknown event: %d\n", event->type);
        }
        break;
    }
}

pntr_app Main(int argc, char* argv[]) {
    return (pntr_app) {
        .width = 400,
        .height = 225,
        .title = "pntr_app: Example",
        .init = Init,
        .update = Update,
        .close = Close,
        .event = Event,
        .fps = 60,
        .userData = PNTR_MALLOC(sizeof(AppData)),
    };
}
