#define RGFW_BUFFER

#ifdef PNTR_APP_RGFW
#ifndef PNTR_APP_RGFW_H__
#define PNTR_APP_RGFW_H__

#ifndef RGFW_HEADER
#define RGFW_IMPLEMENTATION
#include "RGFW.h"
#endif

typedef struct pntr_app_rgfw_platform {
  RGFW_window* window;
} pntr_app_rgfw_platform;

#endif  // PNTR_APP_RGFW_H__

#if defined(PNTR_APP_IMPLEMENTATION) && !defined(PNTR_APP_HEADER_ONLY)
#ifndef PNTR_APP_RGFW_IMPLEMENTATION_ONCE
#define PNTR_APP_RGFW_IMPLEMENTATION_ONCE

// map RGFW_Key to pntr_app_key
pntr_app_key pntr_app_rgfw_key(RGFW_Key key) {
  switch(key) {
    case RGFW_Escape: return PNTR_APP_KEY_ESCAPE;
    case RGFW_F1: return PNTR_APP_KEY_F1;
    case RGFW_F2: return PNTR_APP_KEY_F2;
    case RGFW_F3: return PNTR_APP_KEY_F3;
    case RGFW_F4: return PNTR_APP_KEY_F4;
    case RGFW_F5: return PNTR_APP_KEY_F5;
    case RGFW_F6: return PNTR_APP_KEY_F6;
    case RGFW_F7: return PNTR_APP_KEY_F7;
    case RGFW_F8: return PNTR_APP_KEY_F8;
    case RGFW_F9: return PNTR_APP_KEY_F9;
    case RGFW_F10: return PNTR_APP_KEY_F10;
    case RGFW_F11: return PNTR_APP_KEY_F11;
    case RGFW_F12: return PNTR_APP_KEY_F12;

    case RGFW_Backtick: return PNTR_APP_KEY_GRAVE_ACCENT;

    case RGFW_0: return PNTR_APP_KEY_0;
    case RGFW_1: return PNTR_APP_KEY_1;
    case RGFW_2: return PNTR_APP_KEY_2;
    case RGFW_3: return PNTR_APP_KEY_3;
    case RGFW_4: return PNTR_APP_KEY_4;
    case RGFW_5: return PNTR_APP_KEY_5;
    case RGFW_6: return PNTR_APP_KEY_6;
    case RGFW_7: return PNTR_APP_KEY_7;
    case RGFW_8: return PNTR_APP_KEY_8;
    case RGFW_9: return PNTR_APP_KEY_9;

    case RGFW_Minus: return PNTR_APP_KEY_MINUS;
    case RGFW_Equals: return PNTR_APP_KEY_EQUAL;
    case RGFW_BackSpace: return PNTR_APP_KEY_BACKSPACE;
    case RGFW_Tab: return PNTR_APP_KEY_TAB;
    case RGFW_CapsLock: return PNTR_APP_KEY_CAPS_LOCK;
    case RGFW_ShiftL: return PNTR_APP_KEY_LEFT_SHIFT;
    case RGFW_ControlL: return PNTR_APP_KEY_LEFT_CONTROL;
    case RGFW_AltL: return PNTR_APP_KEY_LEFT_ALT;
    case RGFW_SuperL: return PNTR_APP_KEY_LEFT_SUPER;
    case RGFW_ShiftR: return PNTR_APP_KEY_RIGHT_SHIFT;
    case RGFW_ControlR: return PNTR_APP_KEY_RIGHT_CONTROL;
    case RGFW_AltR: return PNTR_APP_KEY_RIGHT_ALT;
    case RGFW_SuperR: return PNTR_APP_KEY_RIGHT_SUPER;
    case RGFW_Space: return PNTR_APP_KEY_SPACE;

    case RGFW_a: return PNTR_APP_KEY_A;
    case RGFW_b: return PNTR_APP_KEY_B;
    case RGFW_c: return PNTR_APP_KEY_C;
    case RGFW_d: return PNTR_APP_KEY_D;
    case RGFW_e: return PNTR_APP_KEY_E;
    case RGFW_f: return PNTR_APP_KEY_F;
    case RGFW_g: return PNTR_APP_KEY_G;
    case RGFW_h: return PNTR_APP_KEY_H;
    case RGFW_i: return PNTR_APP_KEY_I;
    case RGFW_j: return PNTR_APP_KEY_J;
    case RGFW_k: return PNTR_APP_KEY_K;
    case RGFW_l: return PNTR_APP_KEY_L;
    case RGFW_m: return PNTR_APP_KEY_M;
    case RGFW_n: return PNTR_APP_KEY_N;
    case RGFW_o: return PNTR_APP_KEY_O;
    case RGFW_p: return PNTR_APP_KEY_P;
    case RGFW_q: return PNTR_APP_KEY_Q;
    case RGFW_r: return PNTR_APP_KEY_R;
    case RGFW_s: return PNTR_APP_KEY_S;
    case RGFW_t: return PNTR_APP_KEY_T;
    case RGFW_u: return PNTR_APP_KEY_U;
    case RGFW_v: return PNTR_APP_KEY_V;
    case RGFW_w: return PNTR_APP_KEY_W;
    case RGFW_x: return PNTR_APP_KEY_X;
    case RGFW_y: return PNTR_APP_KEY_Y;
    case RGFW_z: return PNTR_APP_KEY_Z;

    case RGFW_Period: return PNTR_APP_KEY_PERIOD;
    case RGFW_Comma: return PNTR_APP_KEY_COMMA;
    case RGFW_Slash: return PNTR_APP_KEY_SLASH;
    case RGFW_Bracket: return PNTR_APP_KEY_LEFT_BRACKET;
    case RGFW_CloseBracket: return PNTR_APP_KEY_RIGHT_BRACKET;
    case RGFW_Semicolon: return PNTR_APP_KEY_SEMICOLON;
    case RGFW_Return: return PNTR_APP_KEY_ENTER;
    case RGFW_Quote: return PNTR_APP_KEY_APOSTROPHE;
    case RGFW_BackSlash: return PNTR_APP_KEY_BACKSLASH;

    case RGFW_Up: return PNTR_APP_KEY_UP;
    case RGFW_Down: return PNTR_APP_KEY_DOWN;
    case RGFW_Left: return PNTR_APP_KEY_LEFT;
    case RGFW_Right: return PNTR_APP_KEY_RIGHT;

    case RGFW_Delete: return PNTR_APP_KEY_DELETE;
    case RGFW_Insert: return PNTR_APP_KEY_INSERT;
    case RGFW_End: return PNTR_APP_KEY_END;
    case RGFW_Home: return PNTR_APP_KEY_HOME;
    case RGFW_PageUp: return PNTR_APP_KEY_PAGE_UP;
    case RGFW_PageDown: return PNTR_APP_KEY_PAGE_DOWN;

    case RGFW_Numlock: return PNTR_APP_KEY_NUM_LOCK;
    case RGFW_KP_Slash: return PNTR_APP_KEY_KP_DIVIDE;
    case RGFW_Multiply: return PNTR_APP_KEY_KP_MULTIPLY;
    case RGFW_KP_Minus: return PNTR_APP_KEY_KP_SUBTRACT;
    case RGFW_KP_1: return PNTR_APP_KEY_KP_1;
    case RGFW_KP_2: return PNTR_APP_KEY_KP_2;
    case RGFW_KP_3: return PNTR_APP_KEY_KP_3;
    case RGFW_KP_4: return PNTR_APP_KEY_KP_4;
    case RGFW_KP_5: return PNTR_APP_KEY_KP_5;
    case RGFW_KP_6: return PNTR_APP_KEY_KP_6;
    case RGFW_KP_7: return PNTR_APP_KEY_KP_7;
    case RGFW_KP_8: return PNTR_APP_KEY_KP_8;
    case RGFW_KP_9: return PNTR_APP_KEY_KP_9;
    case RGFW_KP_0: return PNTR_APP_KEY_KP_0;
    case RGFW_KP_Period: return PNTR_APP_KEY_KP_DECIMAL;
    case RGFW_KP_Return: return PNTR_APP_KEY_KP_ENTER;
    default: return PNTR_APP_KEY_INVALID;
  }
}

pntr_app_gamepad_button pntr_app_rgfw_gamepad_button(RGFW_joystick_codes button) {
    switch (button) {
        case RGFW_JS_A: return PNTR_APP_GAMEPAD_BUTTON_A;
        case RGFW_JS_B: return PNTR_APP_GAMEPAD_BUTTON_B;
        case RGFW_JS_X: return PNTR_APP_GAMEPAD_BUTTON_X;
        case RGFW_JS_Y: return PNTR_APP_GAMEPAD_BUTTON_Y;
        case RGFW_JS_SELECT: return PNTR_APP_GAMEPAD_BUTTON_SELECT;
        case RGFW_JS_HOME: return PNTR_APP_GAMEPAD_BUTTON_MENU;
        case RGFW_JS_START: return PNTR_APP_GAMEPAD_BUTTON_START;
        case RGFW_JS_L1: return PNTR_APP_GAMEPAD_BUTTON_LEFT_THUMB;
        case RGFW_JS_R1: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_THUMB;
        case RGFW_JS_L2: return PNTR_APP_GAMEPAD_BUTTON_LEFT_SHOULDER;
        case RGFW_JS_R2: return PNTR_APP_GAMEPAD_BUTTON_RIGHT_SHOULDER;
        case RGFW_JS_UP: return PNTR_APP_GAMEPAD_BUTTON_UP;
        case RGFW_JS_DOWN: return PNTR_APP_GAMEPAD_BUTTON_DOWN;
        case RGFW_JS_LEFT: return PNTR_APP_GAMEPAD_BUTTON_LEFT;
        case RGFW_JS_RIGHT: return PNTR_APP_GAMEPAD_BUTTON_RIGHT;
    }
    return PNTR_APP_GAMEPAD_BUTTON_UNKNOWN;
}

bool pntr_app_platform_events(pntr_app* app) {
  if (app == NULL || app->platform == NULL) {
    return false;
  }

  pntr_app_rgfw_platform* platform = (pntr_app_rgfw_platform*)app->platform;
  while(RGFW_window_checkEvent(platform->window)) {
    if (platform->window->event.type == RGFW_quit) {
      return false;
    }

    if (platform->window->event.type == RGFW_keyPressed && platform->window->event.keyCode == RGFW_F11){
      // TODO: toggle fullscreen
    }

    pntr_app_event event = {0};
    event.app = app;

    if (platform->window->event.type == RGFW_keyPressed){
      event.type = PNTR_APP_EVENTTYPE_KEY_DOWN;
      event.key = pntr_app_rgfw_key(platform->window->event.keyCode);
      pntr_app_process_event(app, &event);
    }

    if (platform->window->event.type == RGFW_keyReleased){
      event.type = PNTR_APP_EVENTTYPE_KEY_UP;
      event.key = pntr_app_rgfw_key(platform->window->event.keyCode);
      pntr_app_process_event(app, &event);
    }

    // wheel and button events are tied together
    if (platform->window->event.type == RGFW_mouseButtonPressed) {
      event.type = PNTR_APP_EVENTTYPE_MOUSE_BUTTON_DOWN;

      if (platform->window->event.button == RGFW_mouseScrollUp) {
        event.type = PNTR_APP_EVENTTYPE_MOUSE_WHEEL;
        event.mouseWheel = -1;
      }
      if (platform->window->event.button == RGFW_mouseScrollDown) {
        event.type = PNTR_APP_EVENTTYPE_MOUSE_WHEEL;
        event.mouseWheel = 1;
      }

      if (platform->window->event.button == RGFW_mouseLeft){
        event.mouseButton = PNTR_APP_MOUSE_BUTTON_LEFT;
      }
      if (platform->window->event.button == RGFW_mouseMiddle){
        event.mouseButton = PNTR_APP_MOUSE_BUTTON_MIDDLE;
      }
      if (platform->window->event.button == RGFW_mouseRight){
        event.mouseButton = PNTR_APP_MOUSE_BUTTON_RIGHT;
      }
      pntr_app_process_event(app, &event);
    }

    if (platform->window->event.type == RGFW_mouseButtonReleased) {
      event.type = PNTR_APP_EVENTTYPE_MOUSE_BUTTON_UP;

      // TODO: do i want to handle mouse wheel up/down both?

      if (platform->window->event.button == RGFW_mouseScrollUp) {
        event.type = PNTR_APP_EVENTTYPE_MOUSE_WHEEL;
        event.mouseWheel = -1;
      }
      if (platform->window->event.button == RGFW_mouseScrollDown) {
        event.type = PNTR_APP_EVENTTYPE_MOUSE_WHEEL;
        event.mouseWheel = 1;
      }

      if (platform->window->event.button == RGFW_mouseLeft){
        event.mouseButton = PNTR_APP_MOUSE_BUTTON_LEFT;
      }
      if (platform->window->event.button == RGFW_mouseMiddle){
        event.mouseButton = PNTR_APP_MOUSE_BUTTON_MIDDLE;
      }
      if (platform->window->event.button == RGFW_mouseRight){
        event.mouseButton = PNTR_APP_MOUSE_BUTTON_RIGHT;
      }
      pntr_app_process_event(app, &event);
    }

    if (platform->window->event.type == RGFW_mousePosChanged) {
      event.type = PNTR_APP_EVENTTYPE_MOUSE_MOVE;
      event.mouseX = platform->window->event.point.x;
      event.mouseY = platform->window->event.point.y;
      pntr_app_process_event(app, &event);
    }

    if (platform->window->event.type == RGFW_jsButtonPressed) {
      event.type = PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_DOWN;
      event.gamepad = platform->window->event.joystick;
      event.gamepadButton = pntr_app_rgfw_gamepad_button(platform->window->event.button);
      pntr_app_process_event(app, &event);
    }

    if (platform->window->event.type == RGFW_jsButtonReleased) {
      event.type = PNTR_APP_EVENTTYPE_GAMEPAD_BUTTON_UP;
      event.gamepad = platform->window->event.joystick;
      event.gamepadButton = pntr_app_rgfw_gamepad_button(platform->window->event.button);
      pntr_app_process_event(app, &event);
    }

    // TODO: check this
    if (platform->window->event.type == RGFW_dnd_init) {
      event.type = PNTR_APP_EVENTTYPE_FILE_DROPPED;
      for(int i=0;i<platform->window->event.droppedFilesCount;i++) {
        event.fileDropped = platform->window->event.droppedFiles[i];
        pntr_app_process_event(app, &event);
      }
    }
  }

  return true;
}

/**
 * Pushes the given image to the screen.
 */
bool pntr_app_platform_render(pntr_app* app) {
  if (app == NULL || app->screen == NULL || app->platform == NULL) {
    return false;
  }

  pntr_app_rgfw_platform* platform = (pntr_app_rgfw_platform*)app->platform;

  unsigned char* bitmap = (unsigned char*)app->screen->data;

  // this is 19.2 numbers like 400x225 = 7680x2160
  RGFW_area screenSize = RGFW_getScreenSize();

  for (int y = 0; y < app->height; y++) {
    int index = y * (4 * screenSize.w);
    memcpy(platform->window->buffer + index, bitmap + (4 * app->width * y), app->width * 4);
  }

  RGFW_window_setGPURender(platform->window, 0);
  RGFW_window_swapBuffers(platform->window);
  // RGFW_window_checkFPS(platform->window, 60);

  return true;
}


bool pntr_app_platform_init(pntr_app* app) {
  if (app == NULL) {
    return false;
  }
  app->platform = pntr_load_memory(sizeof(pntr_app_rgfw_platform));
  if (app->platform == NULL) {
    return false;
  }
  pntr_app_rgfw_platform* platform = (pntr_app_rgfw_platform*)app->platform;
  platform->window = RGFW_createWindow(app->title, RGFW_RECT(0, 0, app->width, app->height), RGFW_CENTER);

  return true;
}


void pntr_app_platform_close(pntr_app* app) {
  if (app == NULL || app->platform == NULL) {
    return;
  }

  pntr_app_rgfw_platform* platform = (pntr_app_rgfw_platform*)app->platform;
  RGFW_window_close(platform->window);

  pntr_unload_memory(app->platform);
  app->platform = NULL;
}

bool pntr_app_platform_update_delta_time(pntr_app* app) {
  // TODO: Make delta time get the actual delta time.
  if (app->fps <= 0) {
    app->deltaTime = 0.1f;
    return true;
  }

  app->deltaTime = 1.0f / (float)app->fps;

  return true;
}

PNTR_APP_API void pntr_app_set_title(pntr_app* app, const char* title) {
  if (app == NULL || app->platform == NULL) {
    return;
  }

  pntr_app_rgfw_platform* platform = (pntr_app_rgfw_platform*)app->platform;
  RGFW_window_setName(platform->window, (char*)title);
  app->title = title;
}


bool pntr_app_platform_set_size(pntr_app* app, int width, int height) {
  if (app == NULL || app->platform == NULL) {
    return false;
  }

  pntr_app_rgfw_platform* platform = (pntr_app_rgfw_platform*)app->platform;
  RGFW_window_resize(platform->window, RGFW_AREA(width, height));

  return true;
}


PNTR_APP_API void pntr_app_set_icon(pntr_app* app, pntr_image* icon) {
  if (app == NULL || app->platform == NULL) {
    return;
  }

  pntr_app_rgfw_platform* platform = (pntr_app_rgfw_platform*)app->platform;
  RGFW_window_setIcon(platform->window, (unsigned char*)icon->data, RGFW_AREA(icon->width, icon->height), 4);
}

#endif  // PNTR_APP_RGFW_IMPLEMENTATION_ONCE
#endif  // PNTR_APP_IMPLEMENTATION && !PNTR_APP_HEADER_ONLY
#endif  // PNTR_APP_RGFW
