#include <emscripten/html5.h> // emscripten_set_keydown_callback, emscripten_set_keyup_callback

typedef struct pntr_app_emscripten_platform {
    int mouseX;
    int mouseY;

    EmscriptenGamepadEvent gamepadState[4];
} pntr_app_emscripten_platform;

/**
 * Render the pixel data onto the canvas.
 */
EM_JS(bool, pntr_app_emscripten_render, (void* data, int size, int width, int height), {
    // Make sure the canvas is set up.
    const canvas = document.getElementById('canvas');
    const context = canvas.getContext('2d');
    canvas.setAttribute('width', width);
    canvas.setAttribute('height', height);

    // Set the canvas' image data to the pntr pixel data
    const image = context.getImageData(0, 0, width, height);
    image.data.set(HEAPU8.subarray(data, data + size));

    // Display the new image data on the canvas
    context.putImageData(image, 0, 0);
    return true;
});

/**
 * Pushes the given image to the screen.
 */
bool pntr_app_render(pntr_app* app) {
    if (app == NULL || app->screen == NULL) {
        return true;
    }

    pntr_image* screen = app->screen;
    return pntr_app_emscripten_render(screen->data, screen->pitch * screen->height, screen->width, screen->height);
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
    app->event(&event, app->userData);

    // Return false as we're taking over the event.
    return 1;
}

int pntr_app_emscripten_mouse_button_from_emscripten(unsigned short button) {
    switch (button) {
        case 0: return PNTR_APP_MOUSE_BUTTON_LEFT;
        case 1: return PNTR_APP_MOUSE_BUTTON_MIDDLE;
        case 2: return PNTR_APP_MOUSE_BUTTON_RIGHT;
    }
    return PNTR_APP_MOUSE_BUTTON_UNKNOWN;
}

int pntr_app_emscripten_mouse(int eventType, const struct EmscriptenMouseEvent *mouseEvent, void *userData) {
    pntr_app* app = (pntr_app*)userData;
    if (app == NULL || app->event == NULL) {
        return 0;
    }

    // Build the key event.
    pntr_app_event event;
    switch (eventType) {
        case EMSCRIPTEN_EVENT_MOUSEDOWN: event.type = PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN; break;
        case EMSCRIPTEN_EVENT_MOUSEUP: event.type = PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP; break;
        case EMSCRIPTEN_EVENT_MOUSEMOVE: event.type = PNTR_APP_EVENTTYPE_MOUSE_MOVE; break;
        default:
            return 0;
    }

    pntr_app_emscripten_platform* platform = app->platform;

    // Mouse Button
    event.mouseButton = pntr_app_emscripten_mouse_button_from_emscripten(mouseEvent->button);

    // TODO: Fix mouse position resolution.
    if (eventType == EMSCRIPTEN_EVENT_MOUSEMOVE) {
        platform->mouseX += mouseEvent->movementX;
        platform->mouseY += mouseEvent->movementY;
        if (platform->mouseX < 0) {
            platform->mouseX = 0;
        }
        else if (platform->mouseX > app->width) {
            platform->mouseX = app->width;
        }

        if (platform->mouseY < 0) {
            platform->mouseY = 0;
        }
        else if (platform->mouseY > app->height) {
            platform->mouseY = app->height;
        }
    }

    // TODO: Convert to pixel scale of the application screen.
    event.mouseX = platform->mouseX;
    event.mouseY = platform->mouseY;

    // Invoke the event
    app->event(&event, app->userData);

    return 1;
}

bool pntr_app_init(pntr_app* app) {
    if (app == NULL) {
        return false;
    }

    app->platform = pntr_load_memory(sizeof(pntr_app_emscripten_platform));

    // Title
    emscripten_set_window_title(app->title);

    // Keyboard
    emscripten_set_keydown_callback("#canvas", app, true, pntr_app_emscripten_key);
    emscripten_set_keyup_callback("#canvas", app, true, pntr_app_emscripten_key);

    // Mouse
    emscripten_set_mousedown_callback("#canvas", app, true, pntr_app_emscripten_mouse);
    emscripten_set_mouseup_callback("#canvas", app, true, pntr_app_emscripten_mouse);
    emscripten_set_mousemove_callback("#canvas", app, true, pntr_app_emscripten_mouse);

    return true;
}

void pntr_app_close(pntr_app* app) {
    // Nothing
}

//#include <stdio.h>

pntr_app_gamepad_button pntr_app_emscripten_gamepad_button(int button) {
    // TODO: emscripten: Determine the correct gamepad mappings
    //printf("Emscripten: Button: %d\n", buton);

    return button;
}

bool pntr_app_events(pntr_app* app) {
    if (app == NULL || app->platform == NULL) {
        return false;
    }

    pntr_app_event event;
    pntr_app_emscripten_platform* platform = app->platform;

    // Gamepad Buttons
    // TODO: Is emscripten gamepad working?
    EMSCRIPTEN_RESULT res = emscripten_sample_gamepad_data();
    if (res == EMSCRIPTEN_RESULT_SUCCESS) {
        int numGamepads =  emscripten_get_num_gamepads();
        for (event.gamepad = 0; event.gamepad < numGamepads && event.gamepad < 4; event.gamepad++) {
            EmscriptenGamepadEvent ge;
            if (emscripten_get_gamepad_status(event.gamepad, &ge) != EMSCRIPTEN_RESULT_SUCCESS) {
                continue;
            }

            for (int j = 0; j < ge.numButtons; ++j) {
                if (ge.digitalButton[j] != platform->gamepadState[event.gamepad].digitalButton[j]) {
                    event.gamepadButton = pntr_app_emscripten_gamepad_button(j);
                    if (event.gamepadButton != PNTR_APP_GAMEPAD_BUTTON_UNKNOWN) {
                        event.type = ge.digitalButton[j] ? PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_DOWN : PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_UP;
                        app->event(&event, app->userData);
                    }

                    platform->gamepadState[event.gamepad].digitalButton[j] = ge.digitalButton[j];
                }

                if (ge.analogButton[j] != platform->gamepadState[event.gamepad].analogButton[j]) {
                    event.gamepadButton = pntr_app_emscripten_gamepad_button(j);
                    if (event.gamepadButton != PNTR_APP_GAMEPAD_BUTTON_UNKNOWN) {
                        bool currentPushed = ge.analogButton[j] >= 0.5;
                        bool lastPushed = platform->gamepadState[event.gamepad].analogButton[j] > 0.5;

                        if (currentPushed != lastPushed) {
                            event.type = currentPushed ? PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_DOWN : PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_UP;
                            app->event(&event, app->userData);
                        }
                    }

                    platform->gamepadState[event.gamepad].analogButton[j] = ge.analogButton[j];
                }
            }
        }
    }

    return true;
}

/**
 * The update callback for web.
 */
void pntr_app_update_loop(void* app) {
    if (app == NULL) {
        return;
    }

    pntr_app* application = (pntr_app*)app;

    if (!pntr_app_events(application)) {
        emscripten_cancel_main_loop();
        return;
    }

    // Ensure the application exists.
    if (application->update == NULL ||
        application->update(application->screen, application->userData) == false) {
        emscripten_cancel_main_loop();
        return;
    }

    if (!pntr_app_render(application)) {
        emscripten_cancel_main_loop();
        return;
    }
}
