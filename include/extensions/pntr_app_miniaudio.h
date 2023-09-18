#define MA_NO_FLAC
#define MA_NO_MP3
#define MA_API static
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"


ma_decoder decoder;
ma_device_config deviceConfig;
ma_device device;

pntr_sound* pntr_load_sound_from_memory(const char* fileName, unsigned char* data, unsigned int dataSize) {
    // TODO: MiniFB: Add audio support for MiniFB
    (void)fileName;
    (void)data;
    (void)dataSize;
    // ma_decoder_init_memory
    return NULL;
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
