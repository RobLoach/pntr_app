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

// TODO: Have app be passed in as a pointer?
EM_JS(void, pntr_app_emscripten_register_key_events, (pntr_app* app), {
    const canvas = document.getElementById('canvas');
    const pntr_app_emscripten_key_event = Module.cwrap('pntr_app_emscripten_key_event',
        'void',
        ['number', 'boolean', 'number']
    );

    canvas.addEventListener("keydown", (event) => {
        console.log(event);
        pntr_app_emscripten_key_event(app, true, event.keyCode);
        return false;
    });

    canvas.addEventListener("keyup", (event) => {
        pntr_app_emscripten_key_event(app, false, event.keyCode);
        return false;
    });
});

#include <stdio.h>

// TODO: Have the active app be passed as a pointer?
void EMSCRIPTEN_KEEPALIVE pntr_app_emscripten_key_event(pntr_app* app, bool down, int keyCode) {
    if (app == NULL){
        printf("Nope1");
        return;
    }
    if (app->event == NULL) {
        printf("Nope2");
        return;
    }
    printf("ASDFFDAS: %d\n", keyCode);

    if (keyCode > 0) {
    printf("Called event! %d\n", down);
        pntr_app_event event;
        event.type = down ? PNTR_APP_EVENTTYPE_KEY_DOWN : PNTR_APP_EVENTTYPE_KEY_UP;
        event.type = keyCode;
        app->event(&event, app->userData);
    }
}

bool pntr_app_init(pntr_app* app) {
    if (app == NULL) {
        return false;
    }

    emscripten_set_window_title(app->title);

    pntr_app_emscripten_register_key_events(app);
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