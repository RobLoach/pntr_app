#include <Arduino.h>

// Arduino gives loopTask an 8KB stack, which anything with real stack appetite will blow
// through. stb_image's PNG decoder is one such thing, and the crash it causes reports as a
// stack overflow with an unhelpful backtrace, so give it room up front.
SET_LOOP_TASK_STACK_SIZE(16 * 1024);

// Arduino-ESP32 owns app_main() and calls setup()/loop() from its own task. Both have C++
// linkage, so this shim hands control to the C application in pntr_app_example_esp32.c.

extern "C" bool pntr_app_esp32_setup(void);
extern "C" bool pntr_app_esp32_loop(void);

static bool running = false;

void setup() {
    running = pntr_app_esp32_setup();
}

void loop() {
    if (running) {
        running = pntr_app_esp32_loop();
    }
}
