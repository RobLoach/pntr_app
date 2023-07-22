/**
 * Render the pixel data onto the canvas.
 */
EM_JS(void, pntr_app_emscripten_render, (void* data, int size, int width, int height), {
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
});

/**
 * Pushes the given image to the screen.
 */
bool pntr_app_render(pntr_image* screen) {
    if (screen == NULL) {
        return true;
    }

    pntr_app_emscripten_render(screen->data, screen->pitch * screen->height, screen->width, screen->height);
    return true;
}

bool pntr_app_init(pntr_app* app) {
    return true;
}

void pntr_app_close(pntr_app* app) {
    // Nothing
}

/**
 * The update callback for web.
 */
void pntr_app_update(void* app) {
    pntr_app* application = (pntr_app*)app;

    // Ensure the application exists.
    if (application == NULL ||
        application->update == NULL ||
        application->update(application->screen, application->userData) == false) {
        emscripten_cancel_main_loop();
        return;
    }
    
    pntr_app_render(application->screen);
}