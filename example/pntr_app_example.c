#define PNTR_APP_IMPLEMENTATION
#define PNTR_ENABLE_DEFAULT_FONT
#define PNTR_DISABLE_MATH
#include "../include/pntr_app.h"

typedef struct AppData {
    pntr_image* logo;
    pntr_font* font;
} AppData;

bool Init(void* userData) {
    AppData* appData = (AppData*)userData;

    appData->logo = pntr_load_image("resources/logo.png");
    appData->font = pntr_load_font_default();

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
        pntr_draw_image(screen, appData->logo, screen->width / 2 - appData->logo->width / 2, screen->height / 2 - appData->logo->height / 2);
    }

    return true;
}

void Close(void* userData) {
    AppData* appData = (AppData*)userData;

    pntr_unload_image(appData->logo);
    pntr_unload_font(appData->font);
}

pntr_app Main(int argc, char* argv[]) {
    return (pntr_app) {
        .width = 400,
        .height = 225,
        .title = "pntr_app",
        .init = Init,
        .update = Update,
        .close = Close,
        .fps = 60,
        .userData = PNTR_MALLOC(sizeof(AppData)),
    };
}