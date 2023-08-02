#define PNTR_APP_IMPLEMENTATION
#define PNTR_ENABLE_DEFAULT_FONT
#define PNTR_DISABLE_MATH
#include "pntr_app.h"

typedef struct AppData {
    pntr_font* font;
    int x;
    int mouseX;
    int mouseY;
    pntr_image* logo;
} AppData;

bool Init(void* userData) {
    AppData* app = (AppData*)userData;

    app->font = pntr_load_font_default();
    app->x = 0;
    app->logo = pntr_load_image("resources/pntr-32x32.png");

    return true;
}

bool Update(pntr_image* screen, void* userData) {
    AppData* app = (AppData*)userData;

    // Clear the screen
    pntr_clear_background(screen, PNTR_WHITE);

    // Draw some text
    pntr_draw_text(screen, app->font, "Hello!", app->x++, 10, PNTR_BLACK);
    pntr_draw_line(screen, 0, 17, screen->width, 20, PNTR_PURPLE);
    pntr_draw_circle(screen, app->mouseX, app->mouseY, 3, PNTR_RED);

    pntr_draw_image(screen, app->logo, screen->width / 2 - app->logo->width / 2, screen->height / 2 - app->logo->height / 2);

    return app->x < screen->width;
}

void Close(void* userData) {
    AppData* app = (AppData*)userData;

    pntr_unload_font(app->font);
    pntr_unload_image(app->logo);
}

void Event(pntr_app_event* event, void* userData) {
    AppData* app = (AppData*)userData;

    switch (event->type) {
        case PNTR_APP_EVENTTYPE_KEY_DOWN: {
            if (event->key == PNTR_APP_KEY_SPACE) {
                app->x = 0;
            }
        }
        break;
        case PNTR_APP_EVENTTYPE_MOUSE_MOVE: {
            app->mouseX = event->mouseX;
            app->mouseY = event->mouseY;
        }

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
        .fps = 10,
        .userData = PNTR_MALLOC(sizeof(AppData)),
    };
}
