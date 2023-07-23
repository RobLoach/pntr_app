#include <emscripten/html5.h> // emscripten_set_keydown_callback, emscripten_set_keyup_callback

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
        return true;
    }

    // Build the key event.
    pntr_app_event event; 
    event.type = (eventType == EMSCRIPTEN_EVENT_KEYDOWN) ? PNTR_APP_EVENTTYPE_KEY_DOWN : PNTR_APP_EVENTTYPE_KEY_UP;

    // TODO: keyCode is deprecated, so do some string checkings?
    event.key = keyEvent->keyCode;
    if (event.key <= 0) {
        return true;
    }

    // Invoke the event
    app->event(&event, app->userData);

    // Return false as we're taking over the event.
    return false;
}

bool pntr_app_init(pntr_app* app) {
    if (app == NULL) {
        return false;
    }

    // Title
    emscripten_set_window_title(app->title);

    // Keyboard
    emscripten_set_keydown_callback("#canvas", app, true, pntr_app_emscripten_key);
    emscripten_set_keyup_callback("#canvas", app, true, pntr_app_emscripten_key);
    return true;
}

void pntr_app_close(pntr_app* app) {
    // Nothing
}

bool pntr_app_events(pntr_app* app) {
    // Nothing.
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