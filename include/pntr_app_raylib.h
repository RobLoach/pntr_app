#ifndef PNTR_APP_RAYLIB_H
#define PNTR_APP_RAYLIB_H "raylib.h"
#endif
#include PNTR_APP_RAYLIB_H

// pntr Configuration
// raylib has its own implementation of stb_image_resize, so use that instead of pntr's.
#define PTNR_NO_STB_IMAGE_RESIZE_IMPLEMENTATION
#define PNTR_NO_STB_IMAGE_IMPLEMENTATION
#define PNTR_NO_STB_IMAGE_WRITE_IMPLEMENTATION

#ifndef PNTR_FREE
    #define PNTR_FREE MemFree
#endif

#ifndef PNTR_MALLOC
    #define PNTR_MALLOC MemAlloc
#endif

#ifndef PNTR_LOAD_FILE
    #define PNTR_LOAD_FILE pntr_app_raylib_load_file
    unsigned char* pntr_app_raylib_load_file(const char* fileName, unsigned int* bytesRead) {
        int dataSize = 0;
        unsigned char* output = LoadFileData(fileName, &dataSize);
        if (bytesRead != NULL) {
            *bytesRead = dataSize;
        }
        return output;
    }
#endif

#ifndef PNTR_SAVE_FILE
    #define PNTR_SAVE_FILE(fileName, data, bytesToWrite) SaveFileData(fileName, (void*)data, (int)bytesToWrite)
#endif

Image pntr_app_raylib_image(pntr_image* image);

#ifndef PNTR_LOAD_IMAGE_FROM_MEMORY
    pntr_image* pntr_app_raylib_load_image_from_memory(pntr_image_type type, const unsigned char *fileData, unsigned int dataSize) {
        if (fileData == NULL) {
            return NULL;
        }

        const char* fileType;
        switch (type) {
            case PNTR_IMAGE_TYPE_BMP:
                type = ".bmp";
                break;
            case PNTR_IMAGE_TYPE_JPG:
                type = ".jpg";
                break;
            case PNTR_IMAGE_TYPE_PNG:
                type = ".png";
                break;
        }

        Image image = LoadImageFromMemory(fileType, fileData, dataSize);
        if (!IsImageReady(image)) {
            return NULL;
        }

        pntr_image* output = pntr_image_from_pixelformat(image.data, image.width, image.height, PNTR_PIXELFORMAT_RGBA8888);
        UnloadImage(image);
        return output;
    }
    #define PNTR_LOAD_IMAGE_FROM_MEMORY pntr_app_raylib_load_image_from_memory
#endif

#ifndef PNTR_SAVE_IMAGE_TO_MEMORY
    unsigned char* pntr_app_raylib_save_image_to_memory(pntr_image* image, pntr_image_type type, unsigned int* dataSize) {
        if (image == NULL) {
            return NULL;
        }
        const char* fileType;
        switch (type) {
            case PNTR_IMAGE_TYPE_BMP:
                type = ".bmp";
                break;
            case PNTR_IMAGE_TYPE_JPG:
                type = ".jpg";
                break;
            case PNTR_IMAGE_TYPE_PNG:
                type = ".png";
                break;
        }

        Image image = pntr_app_raylib_image(image);
        int fileSize;
        unsigned char* output = ExportImageToMemory(image, fileType, &fileSize);
        if (dataSize != NULL) {
            dataSize = (unsigned int)fileSize;
        }
        return output;
    }
    #define PNTR_SAVE_IMAGE_TO_MEMORY pntr_app_raylib_save_image_to_memory
#endif

#ifndef PNTR_APP_LOG
    void pntr_app_raylib_log(pntr_app_log_type type, const char* message) {
        TraceLogLevel logLevel;
        switch (type) {
            case PNTR_APP_LOG_INFO: logLevel = LOG_INFO; break;
            case PNTR_APP_LOG_WARNING: logLevel = LOG_WARNING; break;
            case PNTR_APP_LOG_ERROR: logLevel = LOG_ERROR; break;
            case PNTR_APP_LOG_DEBUG: logLevel = LOG_DEBUG; break;
        }

        TraceLog(logLevel, message);
    }
    #define PNTR_APP_LOG pntr_app_raylib_log
#endif

typedef struct pntr_sound_raylib {
    Sound sound;
    bool loop;
} pntr_sound_raylib;

#define PNTR_APP_RAYLIB_MAX_SOUNDS 100

typedef struct pntr_app_raylib_platform {
    Texture screenTexture;
    pntr_sound_raylib* sounds[PNTR_APP_RAYLIB_MAX_SOUNDS];
} pntr_app_raylib_platform;

pntr_app_raylib_platform* pntr_app_raylib_platform_instance;

Image pntr_app_raylib_image(pntr_image* image) {
    Image output = { 0 };
    if (image == NULL) {
        return output;
    }

    // Set up the raylib image to match the screen.
    output.data = image->data;
    output.width = image->width;
    output.height = image->height;
    output.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    output.mipmaps = 1;

    return output;
}

pntr_app_mouse_button pntr_app_raylib_mouse_button(int button) {
    switch (button) {
        case MOUSE_BUTTON_LEFT: return PNTR_APP_MOUSE_BUTTON_LEFT;
        case MOUSE_BUTTON_RIGHT: return PNTR_APP_MOUSE_BUTTON_RIGHT;
        case MOUSE_BUTTON_MIDDLE: return PNTR_APP_MOUSE_BUTTON_MIDDLE;
        default: return PNTR_APP_MOUSE_BUTTON_UNKNOWN;
    }
}

void pntr_app_raylib_destination_rect(pntr_app* app, Rectangle* outRect) {
    // Find the aspect ratio.
    float aspect = (float)app->screen->width / (float)app->screen->height;
    if (aspect <= 0) {
        aspect = (float)app->screen->height / (float)app->screen->width;
    }

    // Calculate the optimal width/height to display in the screen size.
    float height = GetScreenHeight();
    float width = height * aspect;
    if (width > GetScreenWidth()) {
        height = (float)GetScreenWidth() / aspect;
        width = GetScreenWidth();
    }

    // Draw the texture in the middle of the screen.
    outRect->x = (GetScreenWidth() - width) / 2;
    outRect->y = (GetScreenHeight() - height) / 2;
    outRect->width = width;
    outRect->height = height;
}

void pntr_app_raylib_fix_mouse_coordinates(pntr_app* app, pntr_app_event* event) {
    Rectangle dstRect;
    pntr_app_raylib_destination_rect(app, &dstRect);

    event->mouseX = (GetMouseX() - dstRect.x) * app->screen->width / dstRect.width;
    event->mouseY = (GetMouseY() - dstRect.y) * app->screen->height / dstRect.height;
}

bool pntr_app_events(pntr_app* app) {
    if (app == NULL) {
        return false;
    }

    pntr_app_event event;
    event.app = app;

    // Keys
    for (event.key = PNTR_APP_KEY_FIRST; event.key < PNTR_APP_KEY_LAST; event.key++) {
        // Toggle Fullscreen
        if (event.key == PNTR_APP_KEY_F11) {
            if (IsKeyReleased(event.key)) {
                ToggleFullscreen();
            }
            continue;
        }

        if (IsKeyPressed(event.key)) {
            event.type = PNTR_APP_EVENTTYPE_KEY_DOWN;
            pntr_app_process_event(app, &event);
        }
        else if (IsKeyReleased(event.key)) {
            event.type = PNTR_APP_EVENTTYPE_KEY_UP;
            pntr_app_process_event(app, &event);
        }
    }

    // Mouse
    Vector2 mouseMove = GetMouseDelta();
    if ((int)mouseMove.x != 0 || (int)mouseMove.y != 0) {
        event.type = PNTR_APP_EVENTTYPE_MOUSE_MOVE;
        pntr_app_raylib_fix_mouse_coordinates(app, &event);
        event.mouseWheel = 0;
        pntr_app_process_event(app, &event);
    }

    Vector2 mouseWheel = GetMouseWheelMoveV();
    if (mouseWheel.y != 0) {
        event.type = PNTR_APP_EVENTTYPE_MOUSE_WHEEL;
        event.mouseWheel = (mouseWheel.y > 0) ? 1 : -1;
        pntr_app_process_event(app, &event);
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
            pntr_app_raylib_fix_mouse_coordinates(app, &event);
            pntr_app_process_event(app, &event);
        }
    }

    // Gamepads
    for (event.gamepad = 0; event.gamepad < PNTR_APP_MAX_GAMEPADS; event.gamepad++) {
        if (IsGamepadAvailable(event.gamepad)) {
            for (event.gamepadButton = 1; event.gamepadButton <= PNTR_APP_GAMEPAD_BUTTON_RIGHT_THUMB; event.gamepadButton++) {
                if (IsGamepadButtonPressed(event.gamepad, event.gamepadButton)) {
                    event.type = PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_DOWN;
                    pntr_app_process_event(app, &event);
                }
                else if (IsGamepadButtonReleased(event.gamepad, event.gamepadButton)) {
                    event.type = PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_UP;
                    pntr_app_process_event(app, &event);
                }
            }
        }
        else {
            break;
        }
    }

    // File Dropped
    if (IsFileDropped()) {
        event.type = PNTR_APP_EVENTTYPE_FILE_DROPPED;
        FilePathList droppedFiles = LoadDroppedFiles();
        for (unsigned int i = 0; i < droppedFiles.count; i++) {
            event.fileDropped = droppedFiles.paths[i];
            pntr_app_process_event(app, &event);
        }
        UnloadDroppedFiles(droppedFiles);
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
    if (!IsTextureReady(platform->screenTexture)) {
        platform->screenTexture = LoadTextureFromImage(pntr_app_raylib_image(app->screen));
        if (!IsTextureReady(platform->screenTexture)) {
            TraceLog(LOG_ERROR, "pntr: Failed to resize screen texture");
            return false;
        }
    }
    else {
        UpdateTexture(platform->screenTexture, screen->data);
    }

    BeginDrawing();
        ClearBackground(BLACK);

        // Find the aspect ratio.
        float aspect = (float)screen->width / (float)screen->height;
        if (aspect <= 0) {
            aspect = (float)screen->height / (float)screen->width;
        }

        // Calculate the optimal width/height to display in the screen size.
        float height = GetScreenHeight();
        float width = height * aspect;
        if (width > GetScreenWidth()) {
            height = (float)GetScreenWidth() / aspect;
            width = GetScreenWidth();
        }

        // Draw the texture in the middle of the screen.
        Rectangle destRect;
        pntr_app_raylib_destination_rect(app, &destRect);

        Rectangle source = {0, 0, screen->width, screen->height};
        Vector2 origin = {0, 0};
        DrawTexturePro(platform->screenTexture, source, destRect, origin, 0, WHITE);
    EndDrawing();

    // Make sure to sounds that are looping are playing still
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

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    float scale = 2.0f;
    InitWindow((int)((float)app->width * scale), (int)((float)app->height * scale), app->title);
    if (!IsWindowReady()) {
        pntr_unload_memory(app->platform);
        app->platform = NULL;
        return false;
    }

    if (app->fps > 0) {
        SetTargetFPS(app->fps);
    }

    pntr_app_raylib_platform_instance = platform;

    // Audio
    InitAudioDevice();

    // Random Number Generator
    pntr_app_random_seed(app, (unsigned int)GetRandomValue(0, PRAND_RAND_MAX));

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

pntr_sound* pntr_load_sound_from_memory(pntr_app_sound_type type, unsigned char* data, unsigned int dataSize) {
    if (data == NULL || dataSize <= 0) {
        return NULL;
    }

    const char* fileExtension;
    switch (type) {
        case PNTR_APP_SOUND_TYPE_WAV:
            fileExtension = ".wav";
            break;
        case PNTR_APP_SOUND_TYPE_OGG:
            fileExtension = ".ogg";
            break;
        case PNTR_APP_SOUND_TYPE_UNKNOWN:
            return pntr_set_error(PNTR_ERROR_NOT_SUPPORTED);
            break;
    }

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

    // Find the first available Sound object
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

    // Make sure to clean up the sound from the sound library
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

bool pntr_app_platform_update_delta_time(pntr_app* app) {
    app->deltaTime = GetFrameTime();
    return true;
}

PNTR_APP_API void pntr_app_set_title(pntr_app* app, const char* title) {
    if (app != NULL) {
        app->title = title;
    }

    SetWindowTitle(title);
}

PNTR_APP_API void pntr_app_set_icon(pntr_app* app, pntr_image* icon) {
    if (app == NULL || icon == NULL) {
        return;
    }

    Image image;
    image.data = icon->data;
    image.width = icon->width;
    image.height = icon->height;
    image.mipmaps = 1;
    image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

    SetWindowIcon(image);
}

bool _pntr_app_platform_set_size(pntr_app* app, int width, int height) {
    if (app == NULL || app->platform == NULL) {
        return false;
    }

    pntr_app_raylib_platform* platform = (pntr_app_raylib_platform*)app->platform;

    SetWindowSize(width, height);

    // Unload the screen texture, it'll get rebuilt at next render
    if (IsTextureReady(platform->screenTexture)) {
        UnloadTexture(platform->screenTexture);
    }

    platform->screenTexture.id = 0;
    platform->screenTexture.width = 0;
    platform->screenTexture.height = 0;
    platform->screenTexture.mipmaps = 0;
    platform->screenTexture.format = 0;

    return true;
}
