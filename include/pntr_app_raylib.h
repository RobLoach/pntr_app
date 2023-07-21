#ifndef PNTR_APP_RAYLIB_H
#define PNTR_APP_RAYLIB_H "raylib.h"
#endif

#include PNTR_APP_RAYLIB_H

Image image;
Texture texture;

/**
 * Pushes the given image to the screen.
 */
bool pntr_app_render(pntr_image* screen) {
    if (screen == NULL) {
        return false;
    }

    UpdateTexture(texture, screen->data);

    BeginDrawing();
        ClearBackground(BLACK);
        // Find the aspect ratio.
        // TODO: Fix aspect workings
        float aspect = GetScreenHeight() / GetScreenWidth() ;
        if (aspect <= 0) {
            aspect = (float)image.width / (float)image.height;
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

        Rectangle source = {0, 0, image.width, image.height};
        Vector2 origin = {0, 0};
        DrawTexturePro(texture, source, destRec, origin, 0, WHITE);
    EndDrawing();

    return !WindowShouldClose();
}

bool pntr_app_init(pntr_app* app) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(app->width * 2, app->height * 2, app->title);
    SetTargetFPS(app->fps);

    image.data = app->screen->data;
    image.width = app->width;
    image.height = app->height;
    image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    image.mipmaps = 1;
    texture = LoadTextureFromImage(image);

    return true;
}

void pntr_app_close(pntr_app* app) {
    UnloadTexture(texture);
    CloseWindow();
}