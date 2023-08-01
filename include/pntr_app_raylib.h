#ifndef PNTR_APP_RAYLIB_H
#define PNTR_APP_RAYLIB_H "raylib.h"
#endif
#include PNTR_APP_RAYLIB_H

// pntr Configuration
// raylib has its own implementation of stb_image_resize, so use that instead of pntr's.
#define PTNR_NO_STB_IMAGE_RESIZE_IMPLEMENTATION
#ifndef PNTR_FREE
    #define PNTR_FREE MemFree
#endif
#ifndef PNTR_MALLOC
    #define PNTR_MALLOC MemAlloc
#endif
#ifndef PNTR_LOAD_FILE
    #define PNTR_LOAD_FILE LoadFileData
#endif
#ifndef PNTR_SAVE_FILE
    #define PNTR_SAVE_FILE(fileName, data, bytesToWrite) SaveFileData(fileName, (void*)data, bytesToWrite)
#endif

typedef struct pntr_app_raylib_platform {
    Image screenImage;
    Texture screenTexture;
} pntr_app_raylib_platform;

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
    if ((int)mouseMove.x != 0 || (int)mouseMove.y != 0) {
        event.type = PNTR_APP_EVENTTYPE_MOUSE_MOVE;
        event.mouseX = GetMouseX();
        event.mouseY = GetMouseY();
        event.mouseDeltaX = (int)mouseMove.x;
        event.mouseDeltaY = (int)mouseMove.y;
        app->event(&event, app->userData);
    }

    // Mouse Buttons
    for (int i = MOUSE_BUTTON_LEFT; i <= MOUSE_BUTTON_MIDDLE; i++) {
        event.type = PNTR_APP_EVENTTYPE_UNKNOWN;
        if (IsMouseButtonPressed(i)) {
            event.type = PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN;
        }
        else if (IsMouseButtonReleased(i)) {
            event.type = PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP;
        }

        if (event.type != PNTR_APP_EVENTTYPE_UNKNOWN) {
            event.mouseButton = pntr_app_raylib_mouse_button(i);
            event.mouseX = GetMouseX();
            event.mouseY = GetMouseY();
            app->event(&event, app->userData);
        }
    }

    // Gamepads
    for (event.gamepad = 0; event.gamepad < 4; event.gamepad++) {
        if (IsGamepadAvailable(event.gamepad)) {
            for (event.gamepadButton = 1; event.gamepadButton <= PNTR_APP_GAMEPAD_BUTTON_RIGHT_THUMB; event.gamepadButton++) {
                if (IsGamepadButtonPressed(event.gamepad, event.gamepadButton)) {
                    event.type = PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_DOWN;
                    app->event(&event, app->userData);
                }
                else if (IsGamepadButtonReleased(event.gamepad, event.gamepadButton)) {
                    event.type = PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_UP;
                    app->event(&event, app->userData);
                }
            }
        }
        else {
            break;
        }
    }

    return true;
}

/**
 * Pushes the given image to the screen.
 */
bool pntr_app_render(pntr_app* app) {
    if (app == NULL || app->screen == NULL || app->platform == NULL) {
        return false;
    }

    pntr_image* screen = app->screen;
    pntr_app_raylib_platform* platform = (pntr_app_raylib_platform*)app->platform;

    // Update the texture with the latest from the pntr screen.
    UpdateTexture(platform->screenTexture, screen->data);

    BeginDrawing();
        ClearBackground(BLACK);

        // Find the aspect ratio.
        // TODO: Fix aspect workings
        float aspect = GetScreenHeight() / GetScreenWidth() ;
        if (aspect <= 0) {
            aspect = (float)platform->screenImage.width / (float)platform->screenImage.height;
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
        Rectangle source = {0, 0, platform->screenImage.width, platform->screenImage.height};
        Vector2 origin = {0, 0};
        DrawTexturePro(platform->screenTexture, source, destRec, origin, 0, WHITE);
    EndDrawing();

    return !WindowShouldClose();
}

bool pntr_app_init(pntr_app* app) {
    app->platform = pntr_load_memory(sizeof(pntr_app_raylib_platform));
    if (app->platform == NULL) {
        return false;
    }

    pntr_app_raylib_platform* platform = (pntr_app_raylib_platform*)app->platform;

    // TODO: Allow resizing the window
    //SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    float scale = 2.0f;
    InitWindow((int)((float)app->width * scale), (int)((float)app->height * scale), app->title);
    if (!IsWindowReady()) {
        pntr_unload_memory(app->platform);
        app->platform = NULL;
        return false;
    }
    SetMouseScale(1.0f / scale, 1.0f / scale);

    if (app->fps > 0) {
        SetTargetFPS(app->fps);
    }

    // Set up the raylib image to match the screen.
    platform->screenImage.data = app->screen->data;
    platform->screenImage.width = app->width;
    platform->screenImage.height = app->height;
    platform->screenImage.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    platform->screenImage.mipmaps = 1;

    // Build a Texture off of the screen image.
    platform->screenTexture = LoadTextureFromImage(platform->screenImage);

    // Audio
    InitAudioDevice();

    return true;
}

void pntr_app_close(pntr_app* app) {
    if (app != NULL) {
        return;
    }

    if (app->platform != NULL) {
        pntr_app_raylib_platform* platform = (pntr_app_raylib_platform*)app->platform;
        UnloadTexture(platform->screenTexture);
    }

    CloseAudioDevice();
    CloseWindow();
}

pntr_sound* pntr_load_sound(const char* fileName) {
    unsigned int bytesRead;
    unsigned char* data = pntr_load_file(fileName, &bytesRead);
    if (data == NULL) {
        return NULL;
    }

    const char* fileExtension = GetFileExtension(fileName);
    Wave wave = LoadWaveFromMemory(fileExtension, data, bytesRead);
    pntr_unload_file(data);
    if (!IsWaveReady(wave)) {
        pntr_unload_file(data);
        return NULL;
    }

    Sound sound = LoadSoundFromWave(wave);
    UnloadWave(wave);
    if (!IsSoundReady(sound)) {
        return NULL;
    }

    pntr_sound* output = (pntr_sound*)pntr_load_memory(sizeof(Sound));
    if (output == NULL) {
        UnloadSound(sound);
        return NULL;
    }
    pntr_memory_copy(output, &sound, sizeof(Sound));

    return output;
}

void pntr_unload_sound(pntr_sound* sound) {
    if (sound == NULL) {
        return;
    }

    UnloadSound(*((Sound*)sound));
    pntr_unload_memory(sound);
}

void pntr_play_sound(pntr_sound* sound) {
    // TODO: Add volume and panning.
    if (sound == NULL) {
        return;
    }

    PlaySound(*((Sound*)sound));
}

//Music musd;

pntr_music* pntr_load_music(const char* fileName) {

    //musd = LoadMusicStream(fileName);

    // if (!IsMusicReady(musd)) {
    //     printf("FASDASDF\n");
    // }
    unsigned int bytesRead;
    unsigned char* data = pntr_load_file(fileName, &bytesRead);
    if (data == NULL) {
        printf("Nope\n");
        return NULL;
    }

    const char* fileExtension = GetFileExtension(fileName);
    Music music = LoadMusicStreamFromMemory(fileExtension, data, bytesRead);
    //pntr_unload_file(data);
    if (!IsMusicReady(music)) {
        printf("Nope\n");
        return NULL;
    }

    pntr_music* output = (pntr_music*)pntr_load_memory(sizeof(Music));
    if (output == NULL) {
        printf("Nope\n");
        pntr_unload_file(data);
        UnloadMusicStream(music);
        return NULL;
    }
    pntr_memory_copy(output, &music, sizeof(Music));

    return output;
}

void pntr_play_music(pntr_music* music) {

    //PlayMusicStream(musd);
    if (music == NULL) {
        printf("Nope\n");
        return;
    }

    PlayMusicStream(*((Music*)music));
}

void pntr_unload_music(pntr_music* music) {
    if (music == NULL) {
        return;
    }

    UnloadMusicStream(*((Music*)music));
    pntr_unload_memory(music);
}

void pntr_update_music(pntr_music* music) {
    if (music == NULL) {
        return;
    }

    UpdateMusicStream(*((Music*)music));
}
