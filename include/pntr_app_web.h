#ifdef EMSCRIPTEN
#include <emscripten.h>
#include <emscripten/html5.h>


// this is cheap/simple but magic-bytes is better
// https://github.com/konsumer/emscripten_browser_sound/blob/main/browser_sound.h#L50-L64

EM_JS(pntr_sound*, pntr_load_sound_from_memory, (const char* fileNamePtr, unsigned char* dataPtr, unsigned int dataSize), {
    const data = HEAPU8.slice(dataPtr, dataPtr + dataSize);
    const filename = UTF8ToString(fileNamePtr);

    let type = "application/octet-stream";

    if (filename.endsWith('.ogg')) {
        type='audio/ogg';
    }
    else if (filename.endsWith('.wav')) {
        type='audio/wav';
    }
    else {
        return 0;
    }

    const audio = new Audio();
    audio.src = URL.createObjectURL(new Blob([data], { type }));
    Module.pntr_sounds = Module.pntr_sounds || [];
    Module.pntr_sounds.push(audio);

    // Return the length instead of length - 1, as 0 or NULL is how to depict a failed load.
    return Module.pntr_sounds.length;
});

EM_JS(void, pntr_play_sound, (pntr_sound* sound, bool loop), {
    const audio = Module.pntr_sounds[sound - 1];
    if (!audio) {
        console.log('play: sound not loaded', {sound, pntr_sounds: Module.pntr_sound});
        return;
    }

    audio.loop = loop;
    let result = audio.play();

    if (result !== undefined) {
        // If it couldn't play the audio because of the autoplay policy, try to play it again after waiting a bit.
        // https://developer.mozilla.org/en-US/docs/Web/Media/Autoplay_guide#example_handling_play_failures
        result.catch((error) => {
            if (error.name === "NotAllowedError") {
                setTimeout(function() {
                    audio.pause();
                    audio.currentTime = 0;
                    pntr_play_sound(sound, loop);
                }, 500);
            }
        });
    }
})

EM_JS(void, pntr_stop_sound, (pntr_sound* sound), {
    const audio = Module.pntr_sounds[sound - 1];
    if (audio) {
        audio.pause();
        audio.currentTime = 0;
    }
})

EM_JS(void, pntr_unload_sound, (pntr_sound* sound), {
    const audio = Module.pntr_sounds[sound - 1];
    if (audio) {
        audio.pause();
        audio.currentTime = 0;
        revokeObjectURL(audio.src);
    }
})

EM_JS(void, pntr_app_init_js, (const char* title, int width, int height), {
    document.title = UTF8ToString(title);
    canvas.width = width;
    canvas.height = height;
    this.ctx = canvas.getContext('2d');
    specialHTMLTargets["!canvas"] = canvas;
});

EM_JS(void, pntr_app_render_js, (void* bufferPtr, int width, int height), {
    const screen = new Uint8ClampedArray(HEAPU8.subarray(bufferPtr, bufferPtr + (width * height * 4)));
    this.ctx.putImageData(new ImageData(screen, width, height), 0, 0);
});


bool pntr_app_events(pntr_app* app) {
    return true;
}

bool pntr_app_render(pntr_app* app) {
    if (app == NULL || app->screen == NULL) {
        return false;
    }

    pntr_image* screen = app->screen;
    pntr_app_render_js((void*)screen->data, screen->width, screen->height);
    return true;
}

int pntr_app_emscripten_key(int eventType, const struct EmscriptenKeyboardEvent *keyEvent, void *userData) {
    pntr_app* app = (pntr_app*)userData;
    if (app == NULL || app->event == NULL) {
        return 0;
    }

    // Build the key event.
    pntr_app_event event;
    event.type = (eventType == EMSCRIPTEN_EVENT_KEYDOWN) ? PNTR_APP_EVENTTYPE_KEY_DOWN : PNTR_APP_EVENTTYPE_KEY_UP;

    // TODO: keyCode is deprecated, so do some string checkings?
    event.key = keyEvent->keyCode;
    if (event.key <= 0) {
        return 0;
    }

    // Invoke the event
    pntr_app_process_event(app, &event);

    // Return false as we're taking over the event.
    return 1;
}

int pntr_app_emscripten_mouse_button_from_emscripten(unsigned short button) {
    // https://developer.mozilla.org/en-US/docs/Web/API/MouseEvent/button
    switch (button) {
        case 0: return PNTR_APP_MOUSE_BUTTON_LEFT;
        case 1: return PNTR_APP_MOUSE_BUTTON_MIDDLE;
        case 2: return PNTR_APP_MOUSE_BUTTON_RIGHT;
    }
    return PNTR_APP_MOUSE_BUTTON_UNKNOWN;
}

EM_BOOL pntr_app_emscripten_mouse_wheel(int eventType, const struct EmscriptenWheelEvent *mouseEvent, void *userData) {
    pntr_app* app = (pntr_app*)userData;
    if (app == NULL || mouseEvent->deltaY == 0) {
        return EM_FALSE;
    }

    pntr_app_event event;
    event.type = PNTR_APP_EVENTTYPE_MOUSE_WHEEL;
    event.mouseWheel = mouseEvent->deltaY > 0 ? 1 : -1;
    pntr_app_process_event(app, &event);
    return EM_TRUE;
}

EM_BOOL pntr_app_emscripten_mouse(int eventType, const struct EmscriptenMouseEvent *mouseEvent, void *userData) {
    pntr_app* app = (pntr_app*)userData;
    if (app == NULL) {
        return EM_FALSE;
    }

    // Build the key event.
    pntr_app_event event;
    switch (eventType) {
        case EMSCRIPTEN_EVENT_MOUSEDOWN: event.type = PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN; break;
        case EMSCRIPTEN_EVENT_MOUSEUP: event.type = PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP; break;
        case EMSCRIPTEN_EVENT_MOUSEMOVE: event.type = PNTR_APP_EVENTTYPE_MOUSE_MOVE; break;
        default: {
            return EM_FALSE;
        }
    }

    switch (event.type) {
        case PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN:
        case PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP: {
            event.mouseButton = pntr_app_emscripten_mouse_button_from_emscripten(mouseEvent->button);
            if (event.mouseButton != PNTR_APP_MOUSE_BUTTON_UNKNOWN) {
                pntr_app_process_event(app, &event);
            }
        }
        break;
        case PNTR_APP_EVENTTYPE_MOUSE_MOVE: {
            int canvasWidth = EM_ASM_INT({return canvas.clientWidth;});
            int canvasHeight = EM_ASM_INT({return canvas.clientHeight;});
            event.mouseX = (float)mouseEvent->targetX / (float)canvasWidth * (float)app->width;
            event.mouseY = (float)mouseEvent->targetY / (float)canvasHeight * (float)app->height;
            pntr_app_process_event(app, &event);
        }
        break;
        default: {
            return EM_FALSE;
        }
    }

    return EM_TRUE;
}

bool pntr_app_init(pntr_app* app) {
    if (app == NULL) {
        return false;
    }
    pntr_app_init_js(app->title, app->width, app->height);

    // Keyboard
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, app, true, pntr_app_emscripten_key);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, app, true, pntr_app_emscripten_key);

    // Mouse
    emscripten_set_mousedown_callback("!canvas", app, true, pntr_app_emscripten_mouse);
    emscripten_set_mouseup_callback("!canvas", app, true, pntr_app_emscripten_mouse);
    emscripten_set_mousemove_callback("!canvas", app, true, pntr_app_emscripten_mouse);
    emscripten_set_wheel_callback("!canvas", app, true, pntr_app_emscripten_mouse_wheel);

    return true;
}

void pntr_app_close(pntr_app* app) {
    // TODO: Close the context, and delete the canvas.
}

EM_JS(int, pntr_app_emscripten_get_delta_time, (), {
    if (!this.lastUpdate) {
        this.lastUpdate = performance.now();
        this.now = this.lastUpdate;
    }
    now = performance.now();
    const dt = now - this.lastUpdate;
    this.lastUpdate = now;
    return dt;
});

bool pntr_app_platform_update_delta_time(pntr_app* app) {
    app->deltaTime = pntr_app_emscripten_get_delta_time() / 1000.0f;
    return true;
}

void pntr_app_set_title(pntr_app* app, const char* title) {
    pntr_app_init_js(title, app->width, app->height);
}

bool _pntr_app_platform_set_size(pntr_app* app, int width, int height) {
    pntr_app_init_js(app->title, width, height);
    return true;
}

#endif
