#define RL_MALLOC pntr_load_memory
#define RL_FREE pntr_unload_memory
#define RL_CALLOC(n, sz) pntr_load_memory((n) * (sz))
#define SUPPORT_MODULE_RAUDIO
#define RAUDIO_STANDALONE
#define SUPPORT_FILEFORMAT_WAV
#define SUPPORT_FILEFORMAT_OGG
#include "raudio.c"

typedef struct pntr_sound_raudio {
    Sound sound;
    bool loop;
} pntr_sound_raudio;

#ifndef PNTR_APP_RAUDIO_MAX_SOUNDS
#define PNTR_APP_RAUDIO_MAX_SOUNDS 100
#endif

typedef struct pntr_app_raudio_platform {
    pntr_sound_raudio* sounds[PNTR_APP_RAUDIO_MAX_SOUNDS];
} pntr_app_raudio_platform;

pntr_app_raudio_platform* pntr_app_raudio_platform_instance;


#include <stdio.h>
void pntr_app_raudio_init() {
    if (IsAudioDeviceReady()) {
        return;
    }

    InitAudioDevice();
    printf("InitAudioDevice()\n");
    if (IsAudioDeviceReady()) {
        pntr_app_raudio_platform_instance = pntr_load_memory(sizeof(pntr_app_raudio_platform));
        printf("IsAudioDeviceReady\n");
    }
}

void pntr_app_raudio_close() {
    printf("pntr_app_raudio_close\n");
    // Unload all sounds
    if (pntr_app_raudio_platform_instance != NULL) {
        for (int i = 0; i < PNTR_APP_RAUDIO_MAX_SOUNDS; i++) {
            if (pntr_app_raudio_platform_instance->sounds[i] != NULL) {
                pntr_sound_raudio* audio = pntr_app_raudio_platform_instance->sounds[i];
                UnloadSound(audio->sound);
                pntr_unload_memory(audio);
                pntr_app_raudio_platform_instance->sounds[i] = NULL;
            }
        }
    }
printf("CloseAudioDevice start\n");
    CloseAudioDevice();
    printf("CloseAudioDevice\n");
}

pntr_sound* pntr_load_sound_from_memory(const char* fileName, unsigned char* data, unsigned int dataSize) {
    //return NULL;
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
    pntr_sound_raudio* output = (pntr_sound*)pntr_load_memory(sizeof(pntr_sound_raudio));
    if (output == NULL) {
        UnloadSound(sound);
        return NULL;
    }

    printf("pntr_load_sound_from_memory1\n");
    output->sound = sound;
    output->loop = false;

    printf("pntr_load_sound_from_memory2\n");
    // Find the first available Sound object
    if (pntr_app_raudio_platform_instance != NULL) {
        for (int i = 0; i < PNTR_APP_RAUDIO_MAX_SOUNDS; i++) {
            if (pntr_app_raudio_platform_instance->sounds[i] ==  NULL) {
                pntr_app_raudio_platform_instance->sounds[i] = output;
                break;
            }
        }
    }
    printf("pntr_load_sound_from_memory3\n");

    return output;
}

void pntr_unload_sound(pntr_sound* sound) {
    printf("pntr_unload_sound\n");
    if (sound == NULL) {
        return;
    }
    printf("pntr_unload_sound2\n");

    // Make sure to clean up the sound from the sound library
    pntr_sound_raudio* audio = (pntr_sound_raudio*)sound;
    if (pntr_app_raudio_platform_instance != NULL) {
        for (int i = 0; i < PNTR_APP_RAUDIO_MAX_SOUNDS; i++) {
            if (pntr_app_raudio_platform_instance->sounds[i] == audio) {
                UnloadSound(audio->sound);
                pntr_unload_memory(audio);
                pntr_app_raudio_platform_instance->sounds[i] = NULL;
                break;
            }
        }
    }
    printf("pntr_unload_sound3\n");
}

void pntr_play_sound(pntr_sound* sound, bool loop) {
    printf("pntr_play_sound\n");
    // TODO: Add volume and panning.
    if (sound == NULL) {
        return;
    }

    pntr_sound_raudio* audio = (pntr_sound_raudio*)sound;
    printf("pntr_play_sound2\n");
    audio->loop = loop;
    printf("pntr_play_sound3\n");
    PlaySound(audio->sound);
    printf("pntr_play_sound4\n");
}

void pntr_stop_sound(pntr_sound* sound) {
    printf("pntr_stop_sound\n");
    if (sound == NULL) {
        return;
    }

    pntr_sound_raudio* audio = (pntr_sound_raudio*)sound;
    audio->loop = false;
    printf("pntr_stop_sound2\n");
    PlaySound(audio->sound);
}

