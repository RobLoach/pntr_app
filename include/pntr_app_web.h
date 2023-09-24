#ifdef EMSCRIPTEN
#include <emscripten.h>
#include <emscripten/html5.h>

#ifndef PNTR_APP_LOG
    #include <emscripten/console.h> // emscripten_console_log(), emscripten_console_warn(), etc.

    /**
     * Outputs to the console using emscripten's console.h.
     */
    void pntr_app_emscripten_log(pntr_app_log_type type, const char* message) {
        switch (type) {
            case PNTR_APP_LOG_INFO:
                emscripten_console_log(message);
            break;
            case PNTR_APP_LOG_WARNING:
                emscripten_console_warn(message);
            break;
            case PNTR_APP_LOG_ERROR:
                emscripten_console_error(message);
            break;
            case PNTR_APP_LOG_DEBUG:
                emscripten_dbg(message);
            break;
        }
    }
    #define PNTR_APP_LOG pntr_app_emscripten_log
#endif

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
    audio.currentTime = 0;
    let result = audio.play();

    if (result !== undefined) {
        // If it couldn't play the audio because of the autoplay policy, try to play it again after waiting a bit.
        // https://developer.mozilla.org/en-US/docs/Web/Media/Autoplay_guide#example_handling_play_failures
        result.catch((error) => {
            if (error.name === "NotAllowedError") {
                setTimeout(function() {
                    // TODO: Figure out a clean way to handle delayed sounds with currentTime.
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

/**
 * pntr_app_init_js: Initializes the canvas context.
 *
 * @param width The desired width of the context.
 * @param height The desired height of the context.
 */
EM_JS(void, pntr_app_init_js, (int width, int height), {
    canvas.width = width;
    canvas.height = height;
    this.ctx = canvas.getContext('2d');
    specialHTMLTargets["!canvas"] = canvas;
});

/**
 * pntr_app_render_js: Renders the given pixel data onto the emscripten canvas context.
 *
 * @param data Pointer to the pixel data.
 * @param dataSize The size of the pixel data.
 * @param width The width of the image.
 * @param height The height of the image.
 */
EM_JS(void, pntr_app_render_js, (void* data, int dataSize, int width, int height), {
    // Grab the image context.
    const image = this.ctx.getImageData(0, 0, width, height);

    // Set the pixel data with in the image.
    image.data.set(HEAPU8.subarray(data, data + dataSize));

    // Put the image onto the canvas context.
    this.ctx.putImageData(image, 0, 0);
});

/**
 * Ports an emscripten gamepad button mapping to pntr.
 *
 * @see https://w3c.github.io/gamepad/#remapping
 */
pntr_app_gamepad_button pntr_app_emscripten_gamepad_button(int button) {
    // TODO: emscripten: Determine the correct gamepad mappings? This is an Xbox controller.
    // TODO: Emscripten: Adopt a "standard" mapping? https://w3c.github.io/gamepad/#remapping
    switch (button) {
        case 0: return PNTR_APP_GAMEPAD_BUTTON_A;
        case 1: return PNTR_APP_GAMEPAD_BUTTON_B;
        case 2: return PNTR_APP_GAMEPAD_BUTTON_X;
        case 3: return PNTR_APP_GAMEPAD_BUTTON_Y;
        case 4: return PNTR_APP_GAMEPAD_BUTTON_LEFT_SHOULDER;
        case 5: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_SHOULDER;
        case 6: return PNTR_APP_GAMEPAD_BUTTON_SELECT;
        case 7: return PNTR_APP_GAMEPAD_BUTTON_START;
        case 8: return PNTR_APP_GAMEPAD_BUTTON_MENU;
        case 9: return PNTR_APP_GAMEPAD_BUTTON_LEFT_THUMB;
        case 10: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_THUMB;
        case 11: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_THUMB;
        case 12: return PNTR_APP_GAMEPAD_BUTTON_UP;
        case 13: return PNTR_APP_GAMEPAD_BUTTON_DOWN;
        case 14: return PNTR_APP_GAMEPAD_BUTTON_RIGHT;
        case 15: return PNTR_APP_GAMEPAD_BUTTON_LEFT;
    }

    return PNTR_APP_GAMEPAD_BUTTON_UNKNOWN;
}

/**
 * Process the given gamepad event.
 */
void pntr_app_emscripten_gamepad_event(pntr_app* app, EmscriptenGamepadEvent* ge, pntr_app_event* event) {
    // Buttons
    for (int i = 0; i < ge->numButtons; i++) {
        event->gamepadButton = pntr_app_emscripten_gamepad_button(i);
        if (event->gamepadButton != PNTR_APP_GAMEPAD_BUTTON_UNKNOWN) {
            event->type = (ge->digitalButton[i] == EM_TRUE) ? PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_DOWN : PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_UP;
            pntr_app_process_event(app, event);
        }
    }

    // Axis
    #define PNTR_APP_EMSCRIPTEN_GAMEPAD_AXIS_THRESHOLD 0.6
    if (ge->numAxes >= 2) {
        // Left
        event->gamepadButton = PNTR_APP_GAMEPAD_BUTTON_LEFT;
        event->type = (ge->axis[0] <= -PNTR_APP_EMSCRIPTEN_GAMEPAD_AXIS_THRESHOLD) ? PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_DOWN : PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_UP;
        pntr_app_process_event(app, event);

        // Right
        event->gamepadButton = PNTR_APP_GAMEPAD_BUTTON_RIGHT;
        event->type = (ge->axis[0] >= PNTR_APP_EMSCRIPTEN_GAMEPAD_AXIS_THRESHOLD) ? PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_DOWN : PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_UP;
        pntr_app_process_event(app, event);

        // Up
        event->gamepadButton = PNTR_APP_GAMEPAD_BUTTON_UP;
        event->type = (ge->axis[1] <= -PNTR_APP_EMSCRIPTEN_GAMEPAD_AXIS_THRESHOLD) ? PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_DOWN : PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_UP;
        pntr_app_process_event(app, event);

        // Down
        event->gamepadButton = PNTR_APP_GAMEPAD_BUTTON_DOWN;
        event->type = (ge->axis[1] >= PNTR_APP_EMSCRIPTEN_GAMEPAD_AXIS_THRESHOLD) ? PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_DOWN : PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_UP;
        pntr_app_process_event(app, event);
    }

    // TODO: Emscripten: Support Triggers on Axis 2/3, D-Pad on Axis 6/7
}

/**
 * Input: Process Gamepad events.
 *
 * @see https://emscripten.org/docs/api_reference/html5.h.html#gamepad
 */
void pntr_app_emscripten_gamepad(pntr_app* app) {
    // Ask the browser if Gamepad API is supported.
    EMSCRIPTEN_RESULT res = emscripten_sample_gamepad_data();
    if (res != EMSCRIPTEN_RESULT_SUCCESS) {
        return;
    }

    // Loop through every available gamepad.
    pntr_app_event event;
    int numGamepads =  emscripten_get_num_gamepads();
    for (event.gamepad = 0; event.gamepad < numGamepads && event.gamepad < PNTR_APP_MAX_GAMEPADS; event.gamepad++) {
        EmscriptenGamepadEvent ge;

        // Get the active state from the gamepad.
        if (emscripten_get_gamepad_status(event.gamepad, &ge) != EMSCRIPTEN_RESULT_SUCCESS) {
            continue;
        }

        // Process the gamepad events for the given gamepad.
        pntr_app_emscripten_gamepad_event(app, &ge, &event);
    }
}

bool pntr_app_events(pntr_app* app) {
    // Most emscripten events are handled through callbacks, except for Gamepads.
    pntr_app_emscripten_gamepad(app);

    return true;
}

bool pntr_app_render(pntr_app* app) {
    if (app == NULL || app->screen == NULL) {
        return false;
    }

    pntr_app_render_js((void*)app->screen->data, app->screen->pitch * app->screen->height, app->screen->width, app->screen->height);
    return true;
}

EM_BOOL pntr_app_emscripten_key(int eventType, const struct EmscriptenKeyboardEvent *keyEvent, void *userData) {
    pntr_app* app = (pntr_app*)userData;
    if (app == NULL || app->event == NULL) {
        return EM_FALSE;
    }

    // Build the key event.
    pntr_app_event event;
    event.type = (eventType == EMSCRIPTEN_EVENT_KEYDOWN) ? PNTR_APP_EVENTTYPE_KEY_DOWN : PNTR_APP_EVENTTYPE_KEY_UP;

    // TODO: keyCode is deprecated, so do some string checkings?
    event.key = keyEvent->keyCode;
    if (event.key <= 0) {
        return EM_FALSE;
    }

    // Invoke the event
    pntr_app_process_event(app, &event);

    // Return false as we're taking over the event.
    return EM_TRUE;
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

/**
 * @see https://stackoverflow.com/questions/69935188/open-a-file-in-emscripten-using-browser-file-selector-dialogue
 */
EMSCRIPTEN_KEEPALIVE int pntr_app_emscripten_file_dropped(void* app, const char* fileName, void *buffer, size_t size) {
  if (!pntr_save_file(fileName, buffer, size)) {
    pntr_app_log(PNTR_APP_LOG_ERROR, "[pntr_app] Failed to save file");
    return 0;
  }

  pntr_app_event event;
  event.type = PNTR_APP_EVENTTYPE_FILE_DROPPED;
  event.fileDropped = fileName;
  pntr_app_process_event(app, &event);

  return 1;
}

EMSCRIPTEN_KEEPALIVE void* pntr_app_emscripten_load_memory(size_t size) {
    return pntr_load_memory(size);
}

EMSCRIPTEN_KEEPALIVE void pntr_app_emscripten_unload_memory(void* ptr) {
    pntr_unload_memory(ptr);
}

EM_JS(void, pntr_app_emscripten_init_filedropped, (void* app), {
    const stringToNewUTF8Local = s => {
        const buff_ptr = Module._pntr_app_emscripten_load_memory(s.length+1);
        Module.HEAPU8.set((new TextEncoder()).encode(s + '\0'), buff_ptr);
        return buff_ptr;
    };
    canvas.addEventListener('dragover', e => e.preventDefault());
    canvas.addEventListener("drop", e => {
        e.preventDefault();
        for (const file of e.dataTransfer.files) {
            const reader = new FileReader();
            reader.addEventListener("load", e => {
                const bytes = new Uint8Array(event.target.result);
                const data_ptr = Module._pntr_app_emscripten_load_memory(bytes.byteLength);
                Module.HEAPU8.set(bytes, data_ptr);
                Module._pntr_app_emscripten_file_dropped(app, stringToNewUTF8Local(file.name), data_ptr, bytes.byteLength);
            });
            reader.readAsArrayBuffer(file);
        }
    });
});

bool pntr_app_init(pntr_app* app) {
    if (app == NULL) {
        return false;
    }

    // Initialize the context
    pntr_app_init_js(app->width, app->height);

    // Window title
    pntr_app_set_title(app, app->title);

    // Keyboard
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, app, true, pntr_app_emscripten_key);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, app, true, pntr_app_emscripten_key);

    // Mouse
    emscripten_set_mousedown_callback("!canvas", app, true, pntr_app_emscripten_mouse);
    emscripten_set_mouseup_callback("!canvas", app, true, pntr_app_emscripten_mouse);
    emscripten_set_mousemove_callback("!canvas", app, true, pntr_app_emscripten_mouse);
    emscripten_set_wheel_callback("!canvas", app, true, pntr_app_emscripten_mouse_wheel);

    // TODO: emscripten: Register on file drop events for PNTR_APP_EVENT_TYPE_FILE_DROPPED support.
    pntr_app_emscripten_init_filedropped((void*)app);

    return true;
}

void pntr_app_close(pntr_app* app) {
    // TODO: Close the context, and delete the canvas.
}

PNTR_APP_API int pntr_app_random(int min, int max) {
    return (int)((emscripten_random() * (float)(max - min)) + (float)min);
}

PNTR_APP_API void pntr_app_random_seed(unsigned int seed) {
    if (seed == 0) {
        int randomNumberSeed = pntr_app_random(1, 100);
        for (int i = 0; i < randomNumberSeed; i++) {
            pntr_app_random(0, 100);
        }
    }
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

PNTR_APP_API void pntr_app_set_icon(pntr_app* app, pntr_image* icon) {
    (void)app;
    (void)icon;
    // TODO: Add base64 version of the icon.
    // <link href="data:image/x-icon;base64,BASE64STRINGHERE" rel="icon" type="image/x-icon" />
}

void pntr_app_set_title(pntr_app* app, const char* title) {
    if (app == NULL) {
        app->title = title;
    }

    emscripten_set_window_title(title);
}

bool _pntr_app_platform_set_size(pntr_app* app, int width, int height) {
    pntr_app_init_js(width, height);

    return true;
}

#endif
