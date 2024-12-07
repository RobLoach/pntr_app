#ifdef PNTR_APP_NOSOUND
#ifndef PNTR_APP_NOSOUND_H__
#define PNTR_APP_NOSOUND_H__

#endif  // PNTR_APP_NOSOUND_H__

#if defined(PNTR_APP_IMPLEMENTATION) && !defined(PNTR_APP_HEADER_ONLY)
#ifndef PNTR_APP_NOSOUND_IMPLEMENTATION_ONCE
#define PNTR_APP_NOSOUND_IMPLEMENTATION_ONCE

pntr_sound* pntr_load_sound_from_memory(pntr_app_sound_type type, unsigned char* data, unsigned int dataSize) {
    (void)type;
    (void)data;
    (void)dataSize;
    return NULL;
}

void pntr_unload_sound(pntr_sound* sound) {
    (void)sound;
}

void pntr_play_sound(pntr_sound* sound, bool loop) {
    (void)sound;
    (void)loop;
}

void pntr_stop_sound(pntr_sound* sound) {
    (void)sound;
}

#endif  // PNTR_APP_NOSOUND_IMPLEMENTATION_ONCE
#endif  // PNTR_APP_IMPLEMENTATION && !PNTR_APP_HEADER_ONLY
#endif  // PNTR_APP_NOSOUND
