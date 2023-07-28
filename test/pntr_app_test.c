#define PNTR_APP_IMPLEMENTATION
#define PNTR_ENABLE_DEFAULT_FONT
#define PNTR_DISABLE_MATH
#include "pntr_app.h"

typedef struct AppData {
    int iterations;
    pntr_font* font;
} AppData;

bool Init(void* userData) {
    AppData* appData = (AppData*)userData;

    appData->iterations = 0;
    appData->font = pntr_load_font_default();

    return true;
}

bool Update(pntr_image* screen, void* userData) {
    AppData* appData = (AppData*)userData;

    // Clear the screen
    pntr_clear_background(screen, PNTR_WHITE);

    // Draw some text
    pntr_draw_text(screen, appData->font, "Hello!", 10, 10, PNTR_BLACK);
    pntr_draw_line(screen, 0, 18, screen->width, 20, PNTR_PURPLE);

    return appData->iterations++ < 1;
}

void Close(void* userData) {
    AppData* appData = (AppData*)userData;

    pntr_unload_font(appData->font);
}

void Event(pntr_app_event* event, void* userData) {
    (void)userData;
    //AppData* appData = (AppData*)userData;

    switch (event->type) {
        case PNTR_APP_EVENTTYPE_KEY_DOWN: {
            if (event->key == PNTR_APP_KEY_SPACE) {
            }
        }
        break;

        default:
            // Nothing
    }
}

pntr_app Main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    return (pntr_app) {
        .width = 80,
        .height = 24,
        .title = "pntr_app: Test",
        .init = Init,
        .update = Update,
        .close = Close,
        .event = Event,
        .fps = 30,
        .userData = PNTR_MALLOC(sizeof(AppData)),
    };
}
