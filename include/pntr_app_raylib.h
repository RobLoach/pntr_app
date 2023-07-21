#ifndef PNTR_APP_RAYLIB_H
#define PNTR_APP_RAYLIB_H "raylib.h"
#endif

#include PNTR_APP_RAYLIB_H

Image pntr_app_raylib_image;
Texture pntr_app_raylib_texture;

/**
 * Pushes the given image to the screen.
 */
bool pntr_app_render(pntr_image* screen) {
    if (screen == NULL) {
        return false;
    }

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
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(app->width, app->height, app->title);
    SetTargetFPS(app->fps);

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