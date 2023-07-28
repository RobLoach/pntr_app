// TODO: Switch to termbox2? https://github.com/termbox/termbox2
#include <stdio.h>

bool pntr_app_events(pntr_app* app) {
    if (app == NULL) {
        return false;
    }

    if (app->event == NULL) {
        return true;
    }

    //pntr_app_event event;

    return true;
}

/**
 * Pushes the given image to the screen.
 */
bool pntr_app_render(pntr_app* app) {
    if (app == NULL || app->screen == NULL) {
        return false;
    }

    pntr_image* screen = app->screen;

    #ifdef PNTR_APP_CLI_REVERSE_CHARACTERS
    const char* characters = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
    #else
    const char* characters = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
    #endif

    int charactersLen = 70;

    unsigned char* grayscaleImage = (unsigned char*)pntr_image_to_pixelformat(screen, NULL, PNTR_PIXELFORMAT_GRAYSCALE);

    for (int y = 0; y < app->height; y++) {
        for (int x = 0; x < app->width; x++) {
            unsigned char pixelIntensity = grayscaleImage[y * app->width + x];

            int char_index = (int)pixelIntensity * (charactersLen - 1) / 255;
            printf("%c", characters[char_index]);
        }
        printf("\n");
    }

    pntr_unload_memory(grayscaleImage);

    return true;
}

bool pntr_app_init(pntr_app* app) {
    (void)app;
    return true;
}

void pntr_app_close(pntr_app* app) {
    if (app != NULL) {
        return;
    }
}
