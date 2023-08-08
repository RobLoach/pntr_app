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

typedef struct pntr_sound_raylib {
    Sound sound;
    bool loop;
} pntr_sound_raylib;

#define PNTR_APP_RAYLIB_MAX_SOUNDS 100

typedef struct pntr_app_raylib_platform {
    Image screenImage;
    Texture screenTexture;
    pntr_sound_raylib* sounds[PNTR_APP_RAYLIB_MAX_SOUNDS];
} pntr_app_raylib_platform;

pntr_app_raylib_platform* pntr_app_raylib_platform_instance;

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
        event.mouseWheel = 0;
        app->event(&event, app->userData);
    }

    Vector2 mouseWheel = GetMouseWheelMoveV();
    if (mouseWheel.y != 0) {
        event.type = PNTR_APP_EVENTTYPE_MOUSE_WHEEL;
        event.mouseX = GetMouseX();
        event.mouseY = GetMouseY();
        event.mouseWheel = (mouseWheel.y > 0) ? 1 : -1;
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

    // Update the audio
    if (pntr_app_raylib_platform_instance != NULL) {
        for (int i = 0; i < PNTR_APP_RAYLIB_MAX_SOUNDS; i++) {
            pntr_sound_raylib* sound = pntr_app_raylib_platform_instance->sounds[i];
            if (sound !=  NULL) {
                if (sound->loop) {
                    if (!IsSoundPlaying(sound->sound)) {
                        PlaySound(sound->sound);
                    }
                }
            }
        }
    }

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
    pntr_app_raylib_platform_instance = platform;

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

    pntr_app_raylib_platform_instance = NULL;

    CloseAudioDevice();
    CloseWindow();
}

pntr_sound* pntr_load_sound_from_memory(const char* fileName, unsigned char* data, unsigned int dataSize) {
    if (data == NULL || dataSize <= 0) {
        return NULL;
    }

    const char* fileExtension = GetFileExtension(fileName);
    Wave wave = LoadWaveFromMemory(fileExtension, data, dataSize);
    pntr_unload_file(data);
    if (!IsWaveReady(wave)) {
        return NULL;
    }

    Sound sound = LoadSoundFromWave(wave);
    UnloadWave(wave);
    if (!IsSoundReady(sound)) {
        return NULL;
    }

    // Store the Sound into our own memory.
    pntr_sound_raylib* output = (pntr_sound*)pntr_load_memory(sizeof(pntr_sound_raylib));
    if (output == NULL) {
        UnloadSound(sound);
        return NULL;
    }

    output->sound = sound;
    output->loop = false;

    if (pntr_app_raylib_platform_instance != NULL) {
        for (int i = 0; i < PNTR_APP_RAYLIB_MAX_SOUNDS; i++) {
            if (pntr_app_raylib_platform_instance->sounds[i] ==  NULL) {
                pntr_app_raylib_platform_instance->sounds[i] = output;
                break;
            }
        }
    }

    return output;
}

void pntr_unload_sound(pntr_sound* sound) {
    if (sound == NULL) {
        return;
    }

    pntr_sound_raylib* audio = (pntr_sound_raylib*)sound;
    if (pntr_app_raylib_platform_instance != NULL) {
        for (int i = 0; i < PNTR_APP_RAYLIB_MAX_SOUNDS; i++) {
            if (pntr_app_raylib_platform_instance->sounds[i] ==  audio) {
                pntr_app_raylib_platform_instance->sounds[i] = NULL;
                break;
            }
        }
    }

    UnloadSound(audio->sound);
    pntr_unload_memory(audio);
}

void pntr_play_sound(pntr_sound* sound, bool loop) {
    // TODO: Add volume and panning.
    if (sound == NULL) {
        return;
    }

    pntr_sound_raylib* audio = (pntr_sound_raylib*)sound;
    audio->loop = loop;
    PlaySound(audio->sound);
}

void pntr_stop_sound(pntr_sound* sound) {
    if (sound == NULL) {
        return;
    }

    pntr_sound_raylib* audio = (pntr_sound_raylib*)sound;
    audio->loop = false;
    PlaySound(audio->sound);
}
