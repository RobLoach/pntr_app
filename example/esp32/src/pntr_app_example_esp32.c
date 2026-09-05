/**
 * pntr_app on the ESP32-2432S028R "Cheap Yellow Display".
 *
 * The screen is deliberately small. pntr allocates it as one contiguous RGBA8888 buffer, so
 * 160x120 costs 76800 bytes, which fits on a board with no PSRAM. pntr_app_esp32 upscales it
 * 2x to fill the 320x240 panel.
 */
#define PNTR_APP_IMPLEMENTATION
#include "pntr_app.h"

typedef struct AppData {
    pntr_font* font;
    pntr_image* logo;
    unsigned int boots;
    bool haveCard;
    float x;
    float y;
    float velocityX;
    float velocityY;
    bool touching;
    float touchX;
    float touchY;
} AppData;

#define BALL_RADIUS 10

bool Init(pntr_app* app) {
    AppData* appData = (AppData*)pntr_load_memory(sizeof(AppData));
    if (appData == NULL) {
        return false;
    }

    appData->font = pntr_load_font_default();

    // Count how many times we have booted, which round-trips the SD card file hooks.
    unsigned int size = 0;
    unsigned int* saved = (unsigned int*)pntr_load_file("pntr_app_boots.dat", &size);
    appData->haveCard = (saved != NULL && size == sizeof(unsigned int));
    appData->boots = appData->haveCard ? *saved + 1 : 1;
    pntr_unload_memory(saved);

    if (pntr_save_file("pntr_app_boots.dat", &appData->boots, sizeof(unsigned int))) {
        appData->haveCard = true;
        pntr_app_log(PNTR_APP_LOG_INFO, "pntr_app_example_esp32: saved the boot count to the SD card");
    }
    else {
        pntr_app_log(PNTR_APP_LOG_WARNING, "pntr_app_example_esp32: could not write to the SD card");
    }

    // Optional artwork. Keep it small: stb_image needs roughly 2.5x the decoded size in
    // scratch, so 96x96 decodes on this board but 128x128 runs out of memory.
    appData->logo = pntr_load_image("logo.png");
    if (appData->logo == NULL) {
        pntr_app_log(PNTR_APP_LOG_INFO, "pntr_app_example_esp32: no logo.png on the card, skipping it");
    }
    appData->x = (float)(pntr_app_width(app) / 2);
    appData->y = (float)(pntr_app_height(app) / 2);
    appData->velocityX = 47.0f;
    appData->velocityY = 33.0f;
    appData->touching = false;
    appData->touchX = 0.0f;
    appData->touchY = 0.0f;

    pntr_app_set_userdata(app, appData);
    pntr_app_log(PNTR_APP_LOG_INFO, "pntr_app_example_esp32: running");

    return true;
}

void Event(pntr_app* app, pntr_app_event* event) {
    AppData* appData = (AppData*)pntr_app_userdata(app);

    switch (event->type) {
        case PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN:
            appData->touching = true;
            appData->touchX = event->mouseX;
            appData->touchY = event->mouseY;
            break;

        case PNTR_APP_EVENTTYPE_MOUSE_MOVE:
            appData->touchX = event->mouseX;
            appData->touchY = event->mouseY;
            break;

        case PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP:
            appData->touching = false;
            break;

        default:
            break;
    }
}

bool Update(pntr_app* app, pntr_image* screen) {
    AppData* appData = (AppData*)pntr_app_userdata(app);
    float deltaTime = pntr_app_delta_time(app);
    int width = pntr_app_width(app);
    int height = pntr_app_height(app);

    pntr_clear_background(screen, PNTR_DARKGRAY);

    // Bounce the ball around the screen.
    appData->x += appData->velocityX * deltaTime;
    appData->y += appData->velocityY * deltaTime;

    if (appData->x < BALL_RADIUS || appData->x > (float)(width - BALL_RADIUS)) {
        appData->velocityX = -appData->velocityX;
    }
    if (appData->y < BALL_RADIUS || appData->y > (float)(height - BALL_RADIUS)) {
        appData->velocityY = -appData->velocityY;
    }

    if (appData->logo != NULL) {
        pntr_draw_image(screen, appData->logo,
            (width - appData->logo->width) / 2,
            (height - appData->logo->height) / 2);
    }

    pntr_draw_circle_fill(screen, (int)appData->x, (int)appData->y, BALL_RADIUS, PNTR_SKYBLUE);
    pntr_draw_rectangle(screen, 0, 0, width, height, PNTR_WHITE);

    // Show where the resistive touch panel thinks the finger is.
    if (appData->touching) {
        pntr_draw_circle(screen, (int)appData->touchX, (int)appData->touchY, 6, PNTR_YELLOW);
    }

    char message[48];
    snprintf(message, sizeof(message), "%dx%d @ %d fps", width, height, pntr_app_fps(app));
    pntr_draw_text(screen, appData->font, message, 4, 4, PNTR_WHITE);

    if (appData->haveCard) {
        snprintf(message, sizeof(message), "SD ok, boot #%u", appData->boots);
    }
    else {
        snprintf(message, sizeof(message), "no SD card");
    }
    pntr_draw_text(screen, appData->font, message, 4, 14, PNTR_WHITE);

    return true;
}

void Close(pntr_app* app) {
    AppData* appData = (AppData*)pntr_app_userdata(app);
    if (appData == NULL) {
        return;
    }

    pntr_unload_image(appData->logo);
    pntr_unload_font(appData->font);
    pntr_unload_memory(appData);
}

pntr_app Main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    return (pntr_app) {
        .width = 160,
        .height = 120,
        .title = "pntr_app",
        .init = Init,
        .update = Update,
        .close = Close,
        .event = Event,
        .fps = 30
    };
}
