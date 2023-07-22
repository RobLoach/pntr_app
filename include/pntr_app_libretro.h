#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
//#include <math.h>

#ifndef PNTR_APP_LIBRETRO_H
#define PNTR_APP_LIBRETRO_H "libretro.h"
#endif
#include PNTR_APP_LIBRETRO_H

#define PNTR_APP_NO_ENTRY

pntr_app* pntr_app_libretro;

static struct retro_log_callback logging;
static retro_log_printf_t log_cb;

static void fallback_log(enum retro_log_level level, const char *fmt, ...) {
    (void)level;
    va_list va;
    va_start(va, fmt);
    vfprintf(stderr, fmt, va);
    va_end(va);
}

void retro_init(void) {
    // Nothing.
}

void retro_deinit(void) {
    // Nothing.
}

unsigned retro_api_version(void) {
    return RETRO_API_VERSION;
}

void retro_set_controller_port_device(unsigned port, unsigned device) {
    log_cb(RETRO_LOG_INFO, "Plugging device %u into port %u.\n", device, port);
}

void retro_get_system_info(struct retro_system_info *info) {
    memset(info, 0, sizeof(*info));

    if (pntr_app_libretro == NULL) {
        info->library_name     = "pntr_app";
    }
    else {
        info->library_name = pntr_app_libretro->title;
    }

    info->library_version  = "0.0.1";
    info->need_fullpath    = false;
    info->valid_extensions = NULL; // Anything is fine, we don't care.
}

static retro_video_refresh_t video_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static retro_environment_t environ_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;

void retro_get_system_av_info(struct retro_system_av_info *info) {
    int fps = 60;
    int width = 640;
    int height = 480;
    if (pntr_app_libretro != NULL) {
        fps = pntr_app_libretro->fps;
        width = pntr_app_libretro->width;
        height = pntr_app_libretro->height;
    }

    info->timing = (struct retro_system_timing) {
        .fps = (double)fps,
        .sample_rate = 0.0,
    };
    info->geometry = (struct retro_game_geometry) {
        .base_width   = width,
        .base_height  = height,
        .max_width    = width,
        .max_height   = height,
        .aspect_ratio = (float)width / (float)height,
    };
}

void retro_set_environment(retro_environment_t cb) {
    environ_cb = cb;

    bool no_content = true;
    cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_content);

    if (cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &logging)) {
        log_cb = logging.log;
    }
    else {
        log_cb = fallback_log;
    }
}

void retro_set_audio_sample(retro_audio_sample_t cb) {
    audio_cb = cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) {
    audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb) {
    input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb) {
    input_state_cb = cb;
}

void retro_set_video_refresh(retro_video_refresh_t cb) {
    video_cb = cb;
}

void retro_reset(void) {
    // Nothing
}

static void update_input(void) {
    input_poll_cb();

     //bool back = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT);
     
}

static void check_variables(void) {
     // Nothing.
}

static void audio_callback(void) {
    audio_cb(0, 0);
}

/**
 * Pushes the given image to the screen.
 */
bool pntr_app_render(pntr_image* screen) {
    if (screen == NULL) {
        return false;
    }

    video_cb((void*)screen->data, screen->width, screen->height, screen->pitch);
}

bool pntr_app_init(pntr_app* app) {
    return true;
}

void pntr_app_close(pntr_app* app) {
    if (app == NULL) {
        return;
    }

    if (app->close != NULL) {
        app->close(app->userData);
    }

    if (app->screen != NULL) {
        pntr_unload_image(app->screen);
    }

    // Clear up any user data.
    if (app->userData != NULL) {
        PNTR_FREE(app->userData);
    }

    PNTR_FREE(app);
}

void retro_run(void) {
    if (pntr_app_libretro == NULL) {
        return;
    }

    update_input();

    if (pntr_app_libretro->update != NULL) {
        if (pntr_app_libretro->update(pntr_app_libretro->screen, pntr_app_libretro->userData) == false) {
            environ_cb(RETRO_ENVIRONMENT_SHUTDOWN, NULL);
            return;
        }
    }

    pntr_app_render(pntr_app_libretro->screen);

    audio_callback();

    bool updated = false;
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated) {
        check_variables();
    }
}

bool retro_load_game(const struct retro_game_info *info) {
    int argc = 1;
    char* argv[2] = {
        "pntr_app",
        info != NULL ? (char*)info->path : NULL
    };
    if (info != NULL && info->path != NULL) {
        argc++;
    }

    pntr_app app = PNTR_APP_MAIN(argc, argv);

    // Call the init callback.
    if (app.init != NULL) {
        // Check if initialization worked.
        if (app.init(app.userData) == false) {
            return false;
        }
    }

    app.screen = pntr_gen_image_color(app.width, app.height, PNTR_BLACK);
    if (app.screen == NULL) {
        return false;
    }

    enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
    if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt)) {
        log_cb(RETRO_LOG_INFO, "XRGB8888 is not supported.\n");
        return false;
    }

    check_variables();

    // Copy the data to the core's app instance.
    pntr_app_libretro = PNTR_MALLOC(sizeof(pntr_app));
    PNTR_MEMCPY(pntr_app_libretro, &app, sizeof(pntr_app));

     (void)info;
     return true;
}

void retro_unload_game(void) {
    pntr_app_close(pntr_app_libretro);
    pntr_app_libretro = NULL;
}

unsigned retro_get_region(void) {
    return RETRO_REGION_NTSC;
}

bool retro_load_game_special(unsigned type, const struct retro_game_info *info, size_t num) {
     (void)type;
     (void)info;
     (void)num;
     return retro_load_game(NULL);
}

size_t retro_serialize_size(void) {
     return 0;
}

bool retro_serialize(void *data, size_t size) {
     (void)data;
     (void)size;
     return true;
}

bool retro_unserialize(const void *data, size_t size) {
     (void)data;
     (void)size;
     return true;
}

void *retro_get_memory_data(unsigned id) {
     (void)id;
     return NULL;
}

size_t retro_get_memory_size(unsigned id) {
     (void)id;
     return 0;
}

void retro_cheat_reset(void) {
     // Nothing.
}

void retro_cheat_set(unsigned index, bool enabled, const char *code) {
     (void)index;
     (void)enabled;
     (void)code;
}