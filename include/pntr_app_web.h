#ifdef EMSCRIPTEN
#include <emscripten.h>

EM_JS(pntr_sound*, pntr_load_sound_from_memory, (const char* fileName, unsigned char* data, unsigned int dataSize), {
    const bytes = HEAPU8.slice(data, data + dataSize);
    console.log('load', bytes);
    const sound = new Audio();
    this.pntr_sounds.push(sound);
    sound.src = URL.createObjectURL(new Blob([bytes]));
    return this.pntr_sounds.length - 1;
});

EM_JS(void, pntr_play_sound, (pntr_sound* sound, bool loop), {
    if (sound && this.pntr_sounds[sound]) {
        this.pntr_sounds[sound].loop = loop;
        this.pntr_sounds[sound].play();
    } else {
        console.log('play: no sound', sound);
    }

})

EM_JS(void, pntr_stop_sound, (pntr_sound* sound), {
    this.pntr_sounds[sound].pause();
    this.pntr_sounds[sound].currentTime = 0;
})

EM_JS(void, pntr_unload_sound, (pntr_sound* sound), {
    this.pntr_sounds[sound].pause();
    this.pntr_sounds[sound].currentTime = 0;
    revokeObjectURL(this.pntr_sounds[sound].src);
})

EM_JS(void, pntr_app_init_js, (const char* title, int width, int height), {
    document.title = UTF8ToString(title);
    canvas.width = width;
    canvas.height = height;
    this.pntr_sounds = [];
});


bool pntr_app_events(pntr_app* app) {
    return true;
}

bool pntr_app_render(pntr_app* app) {
    if (app == NULL || app->screen == NULL) {
        return false;
    }
}

bool pntr_app_init(pntr_app* app) {
    if (app == NULL) {
        return false;
    }
    pntr_app_init_js(app->title, app->width, app->height);
    return true;
}

void pntr_app_close(pntr_app* app) {
}

bool pntr_app_platform_update_delta_time(pntr_app* app) {
    return true;
}

void pntr_app_set_title(pntr_app* app, const char* title) {
    pntr_app_init_js(app->title, app->width, app->height);
}

bool _pntr_app_platform_set_size(pntr_app* app, int width, int height) {
    pntr_app_init_js(app->title, app->width, app->height);
    return true;
}

#endif
