#ifndef PNTR_APP_SDL_H
#define PNTR_APP_SDL_H <SDL2/SDL.h>
#endif

#include PNTR_APP_SDL_H

SDL_Window* pntr_app_sdl_window;
SDL_Surface* pntr_app_sdl_screen;
SDL_Surface* pntr_app_sdl_surface;

/**
 * Pushes the given image to the screen.
 */
bool pntr_app_render(pntr_image* screen) {
    if (screen == NULL) {
        return false;
    }

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:
                return false;
            case SDL_MOUSEBUTTONUP:
                switch (event.button.button) {
                    case SDL_BUTTON_LEFT:
                        //examples_next();
                        break;
                    case SDL_BUTTON_RIGHT:
                        //examples_previous();
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return false;
                    case SDLK_LEFT:
                        //examples_previous();
                        break;
                    case SDLK_RIGHT:
                    case SDLK_SPACE:
                    case SDLK_RETURN:
                        //examples_next();
                        break;
                }
                break;
        }
    }

    SDL_BlitSurface(pntr_app_sdl_surface, NULL, pntr_app_sdl_screen, NULL);
    SDL_UpdateWindowSurface(pntr_app_sdl_window);

    return true;
}

bool pntr_app_init(pntr_app* app) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    pntr_app_sdl_window = SDL_CreateWindow(app->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, app->width, app->height, SDL_WINDOW_SHOWN);
    pntr_app_sdl_screen = SDL_GetWindowSurface(pntr_app_sdl_window);
    pntr_app_sdl_surface = SDL_CreateRGBSurfaceWithFormatFrom(app->screen->data, app->width, app->height, 8, app->screen->pitch, SDL_PIXELFORMAT_ARGB8888);

    return true;
}

void pntr_app_close(pntr_app* app) {
    SDL_FreeSurface(pntr_app_sdl_surface);
    SDL_FreeSurface(pntr_app_sdl_screen);
    SDL_DestroyWindow(pntr_app_sdl_window);
}