#ifndef PNTR_APP_RAYLIB_H
#define PNTR_APP_RAYLIB_H "raylib.h"
#endif

#include PNTR_APP_RAYLIB_H

Image pntr_app_raylib_image;
Texture pntr_app_raylib_texture;

pntr_app_mouse_button pntr_app_raylib_mouse_button(int button) {
    switch (button) {
        case MOUSE_BUTTON_LEFT: return PNTR_APP_MOUSE_BUTTON_LEFT;
        case MOUSE_BUTTON_RIGHT: return PNTR_APP_MOUSE_BUTTON_RIGHT;
        case MOUSE_BUTTON_MIDDLE: return PNTR_APP_MOUSE_BUTTON_MIDDLE;
        default: return PNTR_APP_MOUSE_BUTTON_UNKNOWN;
    }
}

bool pntr_app_events(pntr_app* app) {
    if (app == NULL) {
        return false;
    }

    if (app->event == NULL) {
        return true;
    }

    pntr_app_event event;

    // Keys
    for (event.key = PNTR_APP_KEY_FIRST; event.key < PNTR_APP_KEY_LAST; event.key++) {
        if (IsKeyPressed(event.key)) {
            event.type = PNTR_APP_EVENTTYPE_KEY_DOWN;
            app->event(&event, app->userData);
        }
        else if (IsKeyReleased(event.key)) {
            event.type = PNTR_APP_EVENTTYPE_KEY_UP;
            app->event(&event, app->userData);
        }
    }

    // Mouse
    Vector2 mouseMove = GetMouseDelta();
    if (mouseMove.x != 0.0f || mouseMove.y != 0.0f) {
        event.type = PNTR_APP_EVENTTYPE_MOUSE_MOVE;
        event.mouse_x = GetMouseX();
        event.mouse_y = GetMouseY();
        app->event(&event, app->userData);
    }

    // Mouse Buttons
    for (int i = 0; i < 3; i++) {
        event.type = PNTR_APP_EVENTTYPE_UNKNOWN;
        if (IsMouseButtonPressed(i)) {
            event.type = PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN;
        }
        else if (IsMouseButtonReleased(i)) {
            event.type = PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP;
        }

        if (event.type != PNTR_APP_EVENTTYPE_UNKNOWN) {
            event.mouse_button = pntr_app_raylib_mouse_button(i);
            event.mouse_x = GetMouseX();
            event.mouse_y = GetMouseY();
            app->event(&event, app->userData);
        }
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
    pntr_image* screen = app->screen;

    UpdateTexture(pntr_app_raylib_texture, screen->data);

    BeginDrawing();
        ClearBackground(BLACK);
        // Find the aspect ratio.
        // TODO: Fix aspect workings
        float aspect = GetScreenHeight() / GetScreenWidth() ;
        if (aspect <= 0) {
            aspect = (float)pntr_app_raylib_image.width / (float)pntr_app_raylib_image.height;
        }

        // Calculate the optimal width/height to display in the screen size.
        int height = GetScreenHeight();
        int width = height * aspect;
        if (width > GetScreenWidth()) {
            height = (float)GetScreenWidth() / aspect;
            width = GetScreenWidth();
        }

        // Draw the texture in the middle of the screen.
        int x = (GetScreenWidth() - width) / 2;
        int y = (GetScreenHeight() - height) / 2;
        Rectangle destRec = {x, y, width, height};

        Rectangle source = {0, 0, pntr_app_raylib_image.width, pntr_app_raylib_image.height};
        Vector2 origin = {0, 0};
        DrawTexturePro(pntr_app_raylib_texture, source, destRec, origin, 0, WHITE);
    EndDrawing();

    return !WindowShouldClose();
}

bool pntr_app_init(pntr_app* app) {
    // TODO: Allow resizing the window
    //SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(app->width * 2, app->height * 2, app->title);
    SetMouseScale(0.5f, 0.5f);

    if (app->fps > 0) {
        SetTargetFPS(app->fps);
    }

    pntr_app_raylib_image.data = app->screen->data;
    pntr_app_raylib_image.width = app->width;
    pntr_app_raylib_image.height = app->height;
    pntr_app_raylib_image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    pntr_app_raylib_image.mipmaps = 1;
    pntr_app_raylib_texture = LoadTextureFromImage(pntr_app_raylib_image);

    return true;
}

void pntr_app_close(pntr_app* app) {
    UnloadTexture(pntr_app_raylib_texture);
    CloseWindow();
}