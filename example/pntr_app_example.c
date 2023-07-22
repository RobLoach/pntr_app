#define PNTR_APP_IMPLEMENTATION
#define PNTR_DISABLE_MATH
#include "../include/pntr_app.h"

bool Init(void* userData) {
    // Nothing.

    return true;
}

bool Update(pntr_image* screen, void* userData) {
    pntr_clear_background(screen, PNTR_RAYWHITE);
    pntr_draw_circle_fill(screen, screen->width / 2, screen->height / 2, 100, PNTR_BLUE);

    return true;
}

void Close(void* userData) {
    // Close the thing
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
        .userData = NULL
    };
}