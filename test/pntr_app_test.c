#define PNTR_APP_IMPLEMENTATION
#define PNTR_ENABLE_DEFAULT_FONT
#define PNTR_DISABLE_MATH
#define PNTR_APP_DISABLE_TERMBOX
#include "pntr_app.h"

typedef struct AppData {
    pntr_font* font;
    float timePassed;
} AppData;

#include <stdio.h>

#define ASSERT(condition) do { \
    if ((bool)(condition) == false) { \
        char message[512]; \
        sprintf(message, "%s:%d: error: expected %s", __FILE__, __LINE__, #condition); \
        pntr_app_log(PNTR_APP_LOG_ERROR, message); \
        return false; \
    } \
} while(0)

bool Init(pntr_app* app) {
    // pntr_app_userdata
    {
        pntr_app_set_userdata(app, NULL);
        void* userData = pntr_app_userdata(app);
        ASSERT(userData == NULL);
        void* userData2 = pntr_load_memory(20);
        pntr_app_set_userdata(app, userData2);
        userData = pntr_app_userdata(app);
        ASSERT(userData == userData2);
        pntr_unload_memory(userData2);
    }

    // pntr_app_width
    {
        int width = pntr_app_width(app);
        ASSERT(width == 80);
    }

    // pntr_app_height
    {
        int height = pntr_app_height(app);
        ASSERT(height == 24);
    }

    // pntr_app_set_size
    {
        ASSERT(pntr_app_set_size(app, 50, 10));
        pntr_app_set_size(app, 80, 24);
        ASSERT(pntr_app_width(app) == 80);
        ASSERT(pntr_app_height(app) == 24);
    }

    // pntr_app_set_title
    {
        pntr_app_set_title(app, "Hello World!");
        const char* title = pntr_app_title(app);
        ASSERT(title[0] == 'H');
        ASSERT(title[1] == 'e');
    }

    // Set up the application data.
    AppData* appData = pntr_load_memory(sizeof(AppData));
    pntr_app_set_userdata(app, appData);
    appData->timePassed = 0.0f;

    // Load the font
    appData->font = pntr_load_font_default();
    ASSERT(appData->font != NULL);

    pntr_app_log(PNTR_APP_LOG_INFO, "Tests passed");

    return true;
}

bool Update(pntr_app* app, pntr_image* screen) {
    AppData* appData = (AppData*)pntr_app_userdata(app);

    // Clear the screen
    pntr_clear_background(screen, PNTR_WHITE);

    // Draw some text
    pntr_draw_text(screen, appData->font, "Hello!", (int)appData->timePassed, 10, PNTR_BLACK);

    float dt = pntr_app_delta_time(app);
    ASSERT(dt >= 0.0f);
    appData->timePassed += dt;

    if (appData->timePassed >= 5) {
        return false;
    }

    return true;
}

void Close(pntr_app* app) {
    AppData* appData = (AppData*)pntr_app_userdata(app);
    pntr_unload_font(appData->font);
    pntr_unload_memory(appData);
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
        .fps = 5
    };
}
