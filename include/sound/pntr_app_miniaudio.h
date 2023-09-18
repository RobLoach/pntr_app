#define MA_NO_FLAC
#define MA_NO_MP3
#define MA_API static
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#define SAMPLE_FORMAT   ma_format_f32
#define CHANNEL_COUNT   2
#define SAMPLE_RATE     48000

typedef struct pntr_app_miniaudio {
    ma_engine* pEngine;
    ma_decoder* pDecoder
} pntr_app_miniaudio;

// TODO: Move this to be part of pntr_app.
pntr_app_miniaudio* pntr_app_miniaudio_instance;

void* pntr_app_audio_init() {
    pntr_app_miniaudio_instance = pntr_load_memory(sizeof(pntr_app_miniaudio));
    if (pntr_app_miniaudio_instance == NULL) {
        return NULL;
    }

    pntr_app_miniaudio_instance->pDecoder = pntr_load_memory(sizeof(ma_decoder));
    

    ma_result result = ma_engine_init(NULL, &engine);
}

pntr_sound* pntr_load_sound_from_memory(pntr_app* app, const char* fileName, unsigned char* data, unsigned int dataSize) {

    ma_result result = ma_decoder_init_memory(data, dataSize, NULL)

}

void pntr_unload_sound(pntr_sound* sound) {
    if (sound == NULL) {
        return;
    }

    pntr_unload_memory(sound);
}

void pntr_play_sound(pntr_sound* sound, bool loop) {
    (void)sound;
    (void)loop;
}

void pntr_stop_sound(pntr_sound* sound) {
    (void)sound;
}
