#include "audio.h"

#include "wasm4.h"

void playSound1(void) {
    tone(600 | (100 << 16), 15, 100, TONE_PULSE1);
}

void playSound2(void) {
    tone(100 | (500 << 16), 3, 100, TONE_PULSE2);
}
