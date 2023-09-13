#define PNTR_APP_IMPLEMENTATION
#define PNTR_ENABLE_DEFAULT_FONT
#define PNTR_DISABLE_MATH
#include "pntr_app.h"

typedef struct AppData {
    pntr_font* font;
    int x;
    pntr_image* logo;
} AppData;

bool Init(pntr_app* app) {
    AppData* appData = (AppData*)pntr_load_memory(sizeof(AppData));
    pntr_app_set_userdata(app, appData);

    appData->font = pntr_load_font_default();
    appData->x = 0;
    appData->logo = pntr_load_image("resources/pntr-32x32.png");

    return true;
}

bool Update(pntr_app* app, pntr_image* screen) {
    AppData* appData = (AppData*)pntr_app_userdata(app);

    // Clear the screen
    pntr_clear_background(screen, PNTR_WHITE);

    // Draw some text
    pntr_draw_text(screen, appData->font, "Hello!", appData->x++, 10, PNTR_BLACK);
    pntr_draw_line(screen, 0, 17, screen->width, 20, PNTR_PURPLE);
    pntr_draw_circle(screen, pntr_app_mouse_x(app), pntr_app_mouse_y(app), 3, PNTR_RED);

    pntr_draw_image(screen, appData->logo, screen->width / 2 - appData->logo->width / 2, screen->height / 2 - appData->logo->height / 2);

    return appData->x < screen->width;
}

void Close(pntr_app* app) {
    AppData* appData = (AppData*)pntr_app_userdata(app);
    pntr_unload_font(appData->font);
    pntr_unload_image(appData->logo);
    pntr_unload_memory(appData);
}

void Event(pntr_app* app, pntr_app_event* event) {
    AppData* appData = (AppData*)pntr_app_userdata(app);

    switch (event->type) {
        case PNTR_APP_EVENTTYPE_KEY_DOWN: {
            if (event->key == PNTR_APP_KEY_SPACE) {
                appData->x = 0;
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
        .fps = 10
    };
}
