#define PNTR_APP_IMPLEMENTATION
#define PNTR_ENABLE_DEFAULT_FONT
#define PNTR_APP_DISABLE_TERMBOX
#define PNTR_ENABLE_VARGS
#include "pntr_app.h"
#include "pntr_assert.h"

typedef struct AppData {
    pntr_font* font;
    float timePassed;
} AppData;

#include <stdio.h> // sprintf

#define ASSERT(condition) PNTR_ASSERT(condition)

// State recorded by Event() for the pntr_app_manual_save_load_data() round trip.
static int saveCount = 0;
static size_t saveSize = 0;
static int loadCount = 0;
static size_t loadSize = 0;
static int loadMismatches = 0;

// The byte that the save round trip expects to find at the given offset.
static unsigned char SaveByte(size_t i) {
    return (unsigned char)((i * 7u + 13u) & 0xffu);
}

void Event(pntr_app* app, pntr_app_event* event) {
    (void)app;

    switch (event->type) {
        case PNTR_APP_EVENTTYPE_SAVE: {
            // Fill the entire save buffer with a known pattern.
            unsigned char* save = (unsigned char*)event->save;
            for (size_t i = 0; i < event->save_size; i++) {
                save[i] = SaveByte(i);
            }
            saveSize = event->save_size;
            saveCount++;
        }
        break;
        case PNTR_APP_EVENTTYPE_LOAD: {
            // Make sure every byte came back exactly the way it was saved.
            const unsigned char* save = (const unsigned char*)event->save;
            loadSize = event->save_size;
            loadMismatches = 0;
            for (size_t i = 0; i < event->save_size; i++) {
                if (save[i] != SaveByte(i)) {
                    loadMismatches++;
                }
            }
            loadCount++;
        }
        break;
        default:
            // The input events are asserted on through the state functions instead.
        break;
    }
}

bool Init(pntr_app* app) {
    // pntr_app_userdata
    {
        pntr_app_set_userdata(app, NULL);
        void* userData = pntr_app_userdata(app);
        ASSERT(userData == NULL);
        void* userData2 = pntr_load_memory(20);
        pntr_app_set_userdata(app, userData2);
        userData = pntr_app_userdata(app);
        ASSERT(userData == userData2);
        pntr_unload_memory(userData2);
    }

    // pntr_app_width
    {
        int width = pntr_app_width(app);
        ASSERT(width == 80);
    }

    // pntr_app_height
    {
        int height = pntr_app_height(app);
        ASSERT(height == 24);
    }

    // pntr_app_set_size
    {
        ASSERT(pntr_app_set_size(app, 50, 10));
        ASSERT(pntr_app_width(app) == 50);
        ASSERT(pntr_app_height(app) == 10);

        ASSERT(pntr_app_set_size(app, 80, 24));
        ASSERT(pntr_app_width(app) == 80);
        ASSERT(pntr_app_height(app) == 24);
    }

    // pntr_app_set_title
    {
        pntr_app_set_title(app, "Hello World!");
        const char* title = pntr_app_title(app);
        ASSERT(title[0] == 'H');
        ASSERT(title[1] == 'e');
        ASSERT(title[2] == 'l');
    }

    // pntr_app_key_pressed()
    {
        ASSERT(pntr_app_key_up(app, PNTR_APP_KEY_B));
        ASSERT(!pntr_app_key_down(app, PNTR_APP_KEY_B));
        ASSERT(!pntr_app_key_released(app, PNTR_APP_KEY_B));
        ASSERT(!pntr_app_key_pressed(app, PNTR_APP_KEY_B));

        // Force push a button.
        pntr_app_event e = {0};
        e.app = app;
        e.key = PNTR_APP_KEY_A;
        e.type = PNTR_APP_EVENTTYPE_KEY_DOWN;
        pntr_app_process_event(app, &e);

        ASSERT(pntr_app_key_down(app, PNTR_APP_KEY_A));
        ASSERT(pntr_app_key_pressed(app, PNTR_APP_KEY_A));
    }

    // pntr_app_random_seed(), pntr_app_random_set_seed()
    {
        // Make sure resetting the random seed works.
        pntr_app_random_set_seed(app, 0);
        int random = pntr_app_random(app, 0, 100);
        ASSERT(random == 39);

        pntr_app_random_set_seed(app, 0);
        random = pntr_app_random(app, 0, 100);
        ASSERT(random == 39);

        pntr_app_random_set_seed(app, 5);
        random = pntr_app_random(app, 0, 100);
        ASSERT(random != 39);
    }

    // pntr_app_random(), pntr_app_random_float()
    {
        for (int i = 0; i < 1000; i++) {
            int randomValue = pntr_app_random(app, 5000, 10000);
            ASSERT(randomValue >= 5000);
            ASSERT(randomValue <= 10000);
        }
        for (int x = 0; x < 1000; x++) {
            float randomValue = pntr_app_random_float(app, 0.5f, 2.0f);
            ASSERT(randomValue >= 0.5f);
            ASSERT(randomValue <= 2.0f);
        }
    }

    // pntr_app_clipboard(), pntr_app_set_clipboard()
    {
        pntr_app_set_clipboard(app, "Hello World!", 0);
        const char* clipboard = pntr_app_clipboard(app);
        ASSERT(clipboard[0] == 'H');
        ASSERT(clipboard[1] == 'e');
        ASSERT(clipboard[2] == 'l');
        ASSERT(clipboard[3] == 'l');
        ASSERT(clipboard[4] == 'o');

        pntr_app_set_clipboard(app, "Goodbye World!", 4);
        clipboard = pntr_app_clipboard(app);
        ASSERT(clipboard[0] == 'G');
        ASSERT(clipboard[1] == 'o');
        ASSERT(clipboard[2] == 'o');
        ASSERT(clipboard[3] == 'd');
        ASSERT(clipboard[4] == '\0');
    }

    // pntr_app_mouse_x(), pntr_app_mouse_y(), pntr_app_mouse_delta_x(), pntr_app_mouse_delta_y()
    {
        pntr_app_event e = {0};
        e.app = app;
        e.type = PNTR_APP_EVENTTYPE_MOUSE_MOVE;

        // The mouse starts at the origin, having never moved.
        ASSERT(pntr_app_mouse_x(app) == 0.0f);
        ASSERT(pntr_app_mouse_y(app) == 0.0f);
        ASSERT(pntr_app_mouse_delta_x(app) == 0.0f);
        ASSERT(pntr_app_mouse_delta_y(app) == 0.0f);

        // Leaving the deltas at zero reports an absolute position, so the delta is calculated.
        e.mouseX = 10.0f;
        e.mouseY = 12.0f;
        e.mouseDeltaX = 0.0f;
        e.mouseDeltaY = 0.0f;
        pntr_app_process_event(app, &e);
        ASSERT(pntr_app_mouse_x(app) == 10.0f);
        ASSERT(pntr_app_mouse_y(app) == 12.0f);
        ASSERT(pntr_app_mouse_delta_x(app) == 10.0f);
        ASSERT(pntr_app_mouse_delta_y(app) == 12.0f);

        // The calculated delta is written back into the event.
        ASSERT(e.mouseDeltaX == 10.0f);
        ASSERT(e.mouseDeltaY == 12.0f);

        // A new frame clears the delta, but keeps the position.
        pntr_app_pre_events(app);
        ASSERT(pntr_app_mouse_delta_x(app) == 0.0f);
        ASSERT(pntr_app_mouse_delta_y(app) == 0.0f);
        ASSERT(pntr_app_mouse_x(app) == 10.0f);
        ASSERT(pntr_app_mouse_y(app) == 12.0f);

        // Another absolute position gets its delta from the previous position.
        e.mouseX = 15.0f;
        e.mouseY = 10.0f;
        e.mouseDeltaX = 0.0f;
        e.mouseDeltaY = 0.0f;
        pntr_app_process_event(app, &e);
        ASSERT(pntr_app_mouse_x(app) == 15.0f);
        ASSERT(pntr_app_mouse_y(app) == 10.0f);
        ASSERT(pntr_app_mouse_delta_x(app) == 5.0f);
        ASSERT(pntr_app_mouse_delta_y(app) == -2.0f);

        // An absolute position that didn't change isn't a move, so it's dropped.
        pntr_app_pre_events(app);
        e.mouseDeltaX = 0.0f;
        e.mouseDeltaY = 0.0f;
        pntr_app_process_event(app, &e);
        ASSERT(pntr_app_mouse_x(app) == 15.0f);
        ASSERT(pntr_app_mouse_y(app) == 10.0f);
        ASSERT(pntr_app_mouse_delta_x(app) == 0.0f);
        ASSERT(pntr_app_mouse_delta_y(app) == 0.0f);

        // Passing in a delta reports relative movement, where the position accumulates.
        pntr_app_pre_events(app);
        e.mouseX = 0.0f;
        e.mouseY = 0.0f;
        e.mouseDeltaX = -5.0f;
        e.mouseDeltaY = 3.0f;
        pntr_app_process_event(app, &e);
        ASSERT(pntr_app_mouse_x(app) == 10.0f);
        ASSERT(pntr_app_mouse_y(app) == 13.0f);
        ASSERT(pntr_app_mouse_delta_x(app) == -5.0f);
        ASSERT(pntr_app_mouse_delta_y(app) == 3.0f);

        // Relative movement is clamped to the top left of the screen.
        pntr_app_pre_events(app);
        e.mouseDeltaX = -1000.0f;
        e.mouseDeltaY = -1000.0f;
        pntr_app_process_event(app, &e);
        ASSERT(pntr_app_mouse_x(app) == 0.0f);
        ASSERT(pntr_app_mouse_y(app) == 0.0f);

        // Relative movement is clamped to the bottom right of the screen.
        pntr_app_pre_events(app);
        e.mouseDeltaX = 1000.0f;
        e.mouseDeltaY = 1000.0f;
        pntr_app_process_event(app, &e);
        ASSERT(pntr_app_mouse_x(app) == (float)pntr_app_width(app));
        ASSERT(pntr_app_mouse_y(app) == (float)pntr_app_height(app));

        // Absolute positions are clamped the same way.
        pntr_app_pre_events(app);
        e.mouseX = -50.0f;
        e.mouseY = -50.0f;
        e.mouseDeltaX = 0.0f;
        e.mouseDeltaY = 0.0f;
        pntr_app_process_event(app, &e);
        ASSERT(pntr_app_mouse_x(app) == 0.0f);
        ASSERT(pntr_app_mouse_y(app) == 0.0f);

        pntr_app_pre_events(app);
    }

    // pntr_app_mouse_wheel()
    {
        pntr_app_event e = {0};
        e.app = app;
        e.type = PNTR_APP_EVENTTYPE_MOUSE_WHEEL;

        ASSERT(pntr_app_mouse_wheel(app) == 0);

        // Scroll up.
        e.mouseWheel = 1;
        pntr_app_process_event(app, &e);
        ASSERT(pntr_app_mouse_wheel(app) == 1);

        // The wheel event is handed the current mouse position.
        ASSERT(e.mouseX == pntr_app_mouse_x(app));
        ASSERT(e.mouseY == pntr_app_mouse_y(app));

        // The wheel only lasts for the frame it was scrolled in.
        pntr_app_pre_events(app);
        ASSERT(pntr_app_mouse_wheel(app) == 0);

        // Scroll down.
        e.mouseWheel = -1;
        pntr_app_process_event(app, &e);
        ASSERT(pntr_app_mouse_wheel(app) == -1);

        pntr_app_pre_events(app);
        ASSERT(pntr_app_mouse_wheel(app) == 0);
    }

    // pntr_app_mouse_button_pressed(), pntr_app_mouse_button_down(), pntr_app_mouse_button_released(), pntr_app_mouse_button_up()
    {
        pntr_app_event e = {0};
        e.app = app;
        e.mouseButton = PNTR_APP_MOUSE_BUTTON_LEFT;

        // Nothing is held down to begin with.
        ASSERT(pntr_app_mouse_button_up(app, PNTR_APP_MOUSE_BUTTON_LEFT));
        ASSERT(!pntr_app_mouse_button_down(app, PNTR_APP_MOUSE_BUTTON_LEFT));
        ASSERT(!pntr_app_mouse_button_pressed(app, PNTR_APP_MOUSE_BUTTON_LEFT));
        ASSERT(!pntr_app_mouse_button_released(app, PNTR_APP_MOUSE_BUTTON_LEFT));

        // Force push a mouse button.
        e.type = PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN;
        pntr_app_process_event(app, &e);
        ASSERT(pntr_app_mouse_button_down(app, PNTR_APP_MOUSE_BUTTON_LEFT));
        ASSERT(pntr_app_mouse_button_pressed(app, PNTR_APP_MOUSE_BUTTON_LEFT));
        ASSERT(!pntr_app_mouse_button_up(app, PNTR_APP_MOUSE_BUTTON_LEFT));
        ASSERT(!pntr_app_mouse_button_released(app, PNTR_APP_MOUSE_BUTTON_LEFT));

        // The button event is handed the current mouse position.
        ASSERT(e.mouseX == pntr_app_mouse_x(app));
        ASSERT(e.mouseY == pntr_app_mouse_y(app));

        // The other buttons are unaffected.
        ASSERT(!pntr_app_mouse_button_down(app, PNTR_APP_MOUSE_BUTTON_RIGHT));
        ASSERT(pntr_app_mouse_button_up(app, PNTR_APP_MOUSE_BUTTON_RIGHT));

        // Holding the button across a frame is still down, but no longer pressed.
        pntr_app_pre_events(app);
        ASSERT(pntr_app_mouse_button_down(app, PNTR_APP_MOUSE_BUTTON_LEFT));
        ASSERT(!pntr_app_mouse_button_pressed(app, PNTR_APP_MOUSE_BUTTON_LEFT));

        // Let the button go.
        e.type = PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP;
        pntr_app_process_event(app, &e);
        ASSERT(pntr_app_mouse_button_released(app, PNTR_APP_MOUSE_BUTTON_LEFT));
        ASSERT(pntr_app_mouse_button_up(app, PNTR_APP_MOUSE_BUTTON_LEFT));
        ASSERT(!pntr_app_mouse_button_down(app, PNTR_APP_MOUSE_BUTTON_LEFT));

        // The release only lasts for the frame it happened in.
        pntr_app_pre_events(app);
        ASSERT(!pntr_app_mouse_button_released(app, PNTR_APP_MOUSE_BUTTON_LEFT));
        ASSERT(pntr_app_mouse_button_up(app, PNTR_APP_MOUSE_BUTTON_LEFT));
    }

    // pntr_app_gamepad_button_pressed(), pntr_app_gamepad_button_down(), pntr_app_gamepad_button_released()
    {
        pntr_app_event e = {0};
        e.app = app;
        e.gamepad = 2;
        e.gamepadButton = PNTR_APP_GAMEPAD_BUTTON_A;

        // No gamepad is holding anything down to begin with.
        ASSERT(!pntr_app_gamepad_button_down(app, -1, PNTR_APP_GAMEPAD_BUTTON_A));
        ASSERT(!pntr_app_gamepad_button_pressed(app, -1, PNTR_APP_GAMEPAD_BUTTON_A));
        ASSERT(!pntr_app_gamepad_button_released(app, -1, PNTR_APP_GAMEPAD_BUTTON_A));

        // Force push a button on the third gamepad.
        e.type = PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_DOWN;
        pntr_app_process_event(app, &e);
        ASSERT(pntr_app_gamepad_button_down(app, 2, PNTR_APP_GAMEPAD_BUTTON_A));
        ASSERT(pntr_app_gamepad_button_pressed(app, 2, PNTR_APP_GAMEPAD_BUTTON_A));
        ASSERT(!pntr_app_gamepad_button_released(app, 2, PNTR_APP_GAMEPAD_BUTTON_A));

        // A gamepad of -1 sweeps across every gamepad.
        ASSERT(pntr_app_gamepad_button_down(app, -1, PNTR_APP_GAMEPAD_BUTTON_A));
        ASSERT(pntr_app_gamepad_button_pressed(app, -1, PNTR_APP_GAMEPAD_BUTTON_A));

        // The other gamepads and the other buttons are unaffected.
        ASSERT(!pntr_app_gamepad_button_down(app, 0, PNTR_APP_GAMEPAD_BUTTON_A));
        ASSERT(!pntr_app_gamepad_button_pressed(app, 0, PNTR_APP_GAMEPAD_BUTTON_A));
        ASSERT(!pntr_app_gamepad_button_down(app, 2, PNTR_APP_GAMEPAD_BUTTON_B));
        ASSERT(!pntr_app_gamepad_button_down(app, -1, PNTR_APP_GAMEPAD_BUTTON_B));

        // An out of range gamepad is never down.
        ASSERT(!pntr_app_gamepad_button_down(app, PNTR_APP_MAX_GAMEPADS, PNTR_APP_GAMEPAD_BUTTON_A));
        ASSERT(!pntr_app_gamepad_button_pressed(app, PNTR_APP_MAX_GAMEPADS, PNTR_APP_GAMEPAD_BUTTON_A));
        ASSERT(!pntr_app_gamepad_button_released(app, PNTR_APP_MAX_GAMEPADS, PNTR_APP_GAMEPAD_BUTTON_A));

        // Holding the button across a frame is still down, but no longer pressed.
        pntr_app_pre_events(app);
        ASSERT(pntr_app_gamepad_button_down(app, 2, PNTR_APP_GAMEPAD_BUTTON_A));
        ASSERT(!pntr_app_gamepad_button_pressed(app, 2, PNTR_APP_GAMEPAD_BUTTON_A));
        ASSERT(!pntr_app_gamepad_button_pressed(app, -1, PNTR_APP_GAMEPAD_BUTTON_A));

        // Let the button go.
        e.type = PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_UP;
        pntr_app_process_event(app, &e);
        ASSERT(!pntr_app_gamepad_button_down(app, 2, PNTR_APP_GAMEPAD_BUTTON_A));
        ASSERT(!pntr_app_gamepad_button_down(app, -1, PNTR_APP_GAMEPAD_BUTTON_A));
        ASSERT(pntr_app_gamepad_button_released(app, 2, PNTR_APP_GAMEPAD_BUTTON_A));
        ASSERT(pntr_app_gamepad_button_released(app, -1, PNTR_APP_GAMEPAD_BUTTON_A));

        // The release only lasts for the frame it happened in.
        pntr_app_pre_events(app);
        ASSERT(!pntr_app_gamepad_button_released(app, 2, PNTR_APP_GAMEPAD_BUTTON_A));
        ASSERT(!pntr_app_gamepad_button_released(app, -1, PNTR_APP_GAMEPAD_BUTTON_A));
    }

    // pntr_app_process_event() input bounds checking
    {
        pntr_app_event e = {0};
        e.app = app;

        // Keys outside of the key range are dropped before they index the key state.
        pntr_app_pre_events(app);
        ASSERT(!app->keysChanged);
        e.type = PNTR_APP_EVENTTYPE_KEY_DOWN;
        e.key = PNTR_APP_KEY_INVALID;
        pntr_app_process_event(app, &e);
        e.key = (pntr_app_key)((int)PNTR_APP_KEY_FIRST - 1);
        pntr_app_process_event(app, &e);
        e.key = PNTR_APP_KEY_LAST;
        pntr_app_process_event(app, &e);
        e.key = (pntr_app_key)((int)PNTR_APP_KEY_LAST + 1000);
        pntr_app_process_event(app, &e);
        e.type = PNTR_APP_EVENTTYPE_KEY_UP;
        e.key = PNTR_APP_KEY_LAST;
        pntr_app_process_event(app, &e);
        ASSERT(!app->keysChanged);

        // A key at either end of the valid range still gets through.
        e.type = PNTR_APP_EVENTTYPE_KEY_DOWN;
        e.key = PNTR_APP_KEY_FIRST;
        pntr_app_process_event(app, &e);
        e.key = (pntr_app_key)((int)PNTR_APP_KEY_LAST - 1);
        pntr_app_process_event(app, &e);
        ASSERT(app->keysChanged);
        ASSERT(pntr_app_key_down(app, PNTR_APP_KEY_FIRST));
        ASSERT(pntr_app_key_down(app, PNTR_APP_KEY_MENU));

        // Mouse buttons outside of the button range are dropped.
        pntr_app_pre_events(app);
        ASSERT(!app->mouseButtonsChanged);
        e.type = PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN;
        e.mouseButton = PNTR_APP_MOUSE_BUTTON_UNKNOWN;
        pntr_app_process_event(app, &e);
        e.mouseButton = PNTR_APP_MOUSE_BUTTON_LAST;
        pntr_app_process_event(app, &e);
        e.mouseButton = (pntr_app_mouse_button)((int)PNTR_APP_MOUSE_BUTTON_LAST + 500);
        pntr_app_process_event(app, &e);
        e.type = PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP;
        e.mouseButton = PNTR_APP_MOUSE_BUTTON_LAST;
        pntr_app_process_event(app, &e);
        ASSERT(!app->mouseButtonsChanged);

        // The last valid mouse button still gets through.
        e.type = PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN;
        e.mouseButton = (pntr_app_mouse_button)((int)PNTR_APP_MOUSE_BUTTON_LAST - 1);
        pntr_app_process_event(app, &e);
        ASSERT(app->mouseButtonsChanged);
        ASSERT(pntr_app_mouse_button_down(app, PNTR_APP_MOUSE_BUTTON_X2));
        e.type = PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP;
        pntr_app_process_event(app, &e);
        ASSERT(pntr_app_mouse_button_up(app, PNTR_APP_MOUSE_BUTTON_X2));

        // Gamepads outside of the gamepad range are dropped. A gamepad of -1 only means
        // "any gamepad" when querying the state, never when reporting an event.
        pntr_app_pre_events(app);
        e.type = PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_DOWN;
        e.gamepadButton = PNTR_APP_GAMEPAD_BUTTON_START;
        e.gamepad = -1;
        pntr_app_process_event(app, &e);
        e.gamepad = PNTR_APP_MAX_GAMEPADS;
        pntr_app_process_event(app, &e);
        e.gamepad = PNTR_APP_MAX_GAMEPADS + 100;
        pntr_app_process_event(app, &e);
        ASSERT(!pntr_app_gamepad_button_down(app, -1, PNTR_APP_GAMEPAD_BUTTON_START));
        ASSERT(!pntr_app_gamepad_button_pressed(app, -1, PNTR_APP_GAMEPAD_BUTTON_START));

        // Gamepad buttons outside of the button range are dropped, as they're a bit shift.
        e.gamepad = 0;
        e.gamepadButton = PNTR_APP_GAMEPAD_BUTTON_UNKNOWN;
        pntr_app_process_event(app, &e);
        e.gamepadButton = PNTR_APP_GAMEPAD_BUTTON_LAST;
        pntr_app_process_event(app, &e);
        e.gamepadButton = (pntr_app_gamepad_button)((int)PNTR_APP_GAMEPAD_BUTTON_LAST + 100);
        pntr_app_process_event(app, &e);
        ASSERT(!pntr_app_gamepad_button_down(app, -1, PNTR_APP_GAMEPAD_BUTTON_UNKNOWN));
        ASSERT(!pntr_app_gamepad_button_down(app, -1, PNTR_APP_GAMEPAD_BUTTON_LAST));
        ASSERT(app->gamepadButtonState[0] == 0);

        // A valid gamepad event still gets through.
        e.gamepadButton = PNTR_APP_GAMEPAD_BUTTON_START;
        pntr_app_process_event(app, &e);
        ASSERT(pntr_app_gamepad_button_down(app, 0, PNTR_APP_GAMEPAD_BUTTON_START));
        e.type = PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_UP;
        pntr_app_process_event(app, &e);
        ASSERT(!pntr_app_gamepad_button_down(app, 0, PNTR_APP_GAMEPAD_BUTTON_START));

        pntr_app_pre_events(app);
    }

    // pntr_app_get_file_sound_type()
    {
        ASSERT(pntr_app_get_file_sound_type("sound.wav") == PNTR_APP_SOUND_TYPE_WAV);
        ASSERT(pntr_app_get_file_sound_type("SOUND.WAV") == PNTR_APP_SOUND_TYPE_WAV);
        ASSERT(pntr_app_get_file_sound_type("resources/some.dir/sound.wav") == PNTR_APP_SOUND_TYPE_WAV);
        ASSERT(pntr_app_get_file_sound_type("sound.ogg") == PNTR_APP_SOUND_TYPE_OGG);
        ASSERT(pntr_app_get_file_sound_type("SOUND.OGG") == PNTR_APP_SOUND_TYPE_OGG);
        ASSERT(pntr_app_get_file_sound_type("sound.mp3") == PNTR_APP_SOUND_TYPE_UNKNOWN);
        ASSERT(pntr_app_get_file_sound_type("sound") == PNTR_APP_SOUND_TYPE_UNKNOWN);
        ASSERT(pntr_app_get_file_sound_type("") == PNTR_APP_SOUND_TYPE_UNKNOWN);
    }

    // pntr_load_sound(), pntr_load_sound_from_memory(), pntr_sound_playing()
    {
        // An unsupported extension is refused before the file is ever read.
        ASSERT(pntr_load_sound("sound.mp3") == NULL);

        // A supported extension that isn't on disk fails too.
        ASSERT(pntr_load_sound("this-file-does-not-exist.wav") == NULL);

        // Loading from memory needs actual data.
        unsigned char soundData[4] = {1, 2, 3, 4};
        ASSERT(pntr_load_sound_from_memory(PNTR_APP_SOUND_TYPE_WAV, NULL, 4) == NULL);
        ASSERT(pntr_load_sound_from_memory(PNTR_APP_SOUND_TYPE_WAV, soundData, 0) == NULL);

        #ifndef PNTR_APP_LOAD_SOUND_FROM_MEMORY
            // This platform has no sound device, so there's nothing to load the sound onto.
            ASSERT(pntr_load_sound_from_memory(PNTR_APP_SOUND_TYPE_WAV, soundData, 4) == NULL);
        #endif

        // Every sound function tolerates a NULL sound.
        ASSERT(!pntr_sound_playing(NULL));
        pntr_play_sound(NULL, true);
        pntr_stop_sound(NULL);
        pntr_unload_sound(NULL);
        ASSERT(!pntr_sound_playing(NULL));
    }

    // pntr_set_volume()
    {
        // A NULL sound returns before the volume is clamped.
        pntr_set_volume(NULL, 2.0f);

        #ifndef PNTR_APP_SET_VOLUME
            // This platform defines no PNTR_APP_SET_VOLUME, so the clamp is only reachable as
            // a no-op. Walk both sides of it to show that the handle is never written through.
            int notASound = 1234;
            pntr_sound* sound = (pntr_sound*)&notASound;
            pntr_set_volume(sound, -100.0f);
            pntr_set_volume(sound, 0.0f);
            pntr_set_volume(sound, 0.5f);
            pntr_set_volume(sound, 1.0f);
            pntr_set_volume(sound, 100.0f);
            ASSERT(notASound == 1234);
        #endif
    }

    // pntr_app_manual_save_load_data()
    {
        const char* saveFileName = "pntr_app_test.save";

        pntr_app_event e = {0};
        e.app = app;

        // Saving asks the event callback to fill the save buffer, then base64 encodes it.
        saveCount = 0;
        loadCount = 0;
        e.type = PNTR_APP_EVENTTYPE_SAVE;
        pntr_app_manual_save_load_data(app, &e, saveFileName);
        ASSERT(saveCount == 1);
        ASSERT(saveSize == (size_t)PNTR_APP_SAVE_SIZE);

        // Loading decodes the file and hands the exact same bytes back to the callback.
        e.type = PNTR_APP_EVENTTYPE_LOAD;
        e.save = NULL;
        e.save_size = 0;
        pntr_app_manual_save_load_data(app, &e, saveFileName);
        ASSERT(loadCount == 1);
        ASSERT(loadSize == (size_t)PNTR_APP_SAVE_SIZE);
        ASSERT(loadMismatches == 0);

        // The save buffer is released by the time the call returns.
        ASSERT(e.save == NULL);
        ASSERT(e.save_size == 0);

        // Loading a file that isn't there leaves the callback alone.
        ASSERT(remove(saveFileName) == 0);
        pntr_app_manual_save_load_data(app, &e, saveFileName);
        ASSERT(loadCount == 1);
    }

    // pntr_app_update_fps(), pntr_app_fps()
    {
        // The FPS counter only depends on deltaTime, so it can be stepped by hand.
        app->deltaTime = 0.25f;
        app->fpsTimer = 0.0f;
        app->fpsFrameCount = 0;
        app->actualFPS = 0;

        // A second hasn't passed yet, so there's no count to report.
        for (int i = 0; i < 3; i++) {
            pntr_app_update_fps(app);
            ASSERT(pntr_app_fps(app) == 0);
        }

        // The fourth quarter second frame completes the second.
        pntr_app_update_fps(app);
        ASSERT(pntr_app_fps(app) == 4);

        // The timer rolls over, so the next second is counted from scratch.
        for (int i = 0; i < 3; i++) {
            pntr_app_update_fps(app);
            ASSERT(pntr_app_fps(app) == 4);
        }
        pntr_app_update_fps(app);
        ASSERT(pntr_app_fps(app) == 4);

        // Put the counter back the way it was found.
        app->deltaTime = 0.0f;
        app->fpsTimer = 0.0f;
        app->fpsFrameCount = 0;
        app->actualFPS = 0;
    }

    // Set up the application data.
    AppData* appData = pntr_load_memory(sizeof(AppData));
    pntr_app_set_userdata(app, appData);
    appData->timePassed = 0.0f;

    // Load the font
    appData->font = pntr_load_font_default();
    ASSERT(appData->font != NULL);

    pntr_app_log(PNTR_APP_LOG_INFO, "[pntr_app] Tests passed");

    return true;
}

bool Update(pntr_app* app, pntr_image* screen) {
    AppData* appData = (AppData*)pntr_app_userdata(app);

    // Clear the screen
    pntr_clear_background(screen, PNTR_BLACK);

    // Draw some text
    pntr_draw_text(screen, appData->font, "pntr_app", 7, 4, PNTR_WHITE);
    pntr_draw_text(screen, appData->font, "Tests Passed", 1, 13, PNTR_WHITE);

    // Finish running after one loop
    if (appData->timePassed != 0) {
        return false;
    }

    float dt = pntr_app_delta_time(app);
    appData->timePassed += dt;

    return true;
}

void Close(pntr_app* app) {
    AppData* appData = (AppData*)pntr_app_userdata(app);
    pntr_unload_font(appData->font);
    pntr_unload_memory(appData);
}

pntr_app Main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    return (pntr_app) {
        .width = 80,
        .height = 24,
        .title = "pntr_app: Test",
        .init = Init,
        .update = Update,
        .close = Close,
        .event = Event,
        .fps = 5
    };
}
