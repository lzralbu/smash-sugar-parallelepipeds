#include "input.h"

#include "utils.h"

#include "wasm4.h"

static const uint8_t *gamepads[4] = { GAMEPAD1, GAMEPAD2, GAMEPAD3, GAMEPAD4 };

static uint8_t current[4] = { 0 };
static uint8_t previous[4] = { 0 };
static uint8_t pressed[4] = { 0 };

void inputUpdate(void) {
    for (size_t i = 0; i < 4; ++i) {
        current[i] = *gamepads[i];
        pressed[i] = current[i] & (current[i] ^ previous[i]);
        previous[i] = current[i];
    }
}

_Bool isDown(uint8_t gamepad, uint8_t button) {
    MY_ASSERT(1 <= gamepad && gamepad <= 4);
    MY_ASSERT(
        button == BUTTON_1 || button == BUTTON_2 || button == BUTTON_LEFT ||
        button == BUTTON_RIGHT || button == BUTTON_UP || button == BUTTON_DOWN
    );
    return current[gamepad - 1] & button;
}

// detects if button was pressed during the current frame
_Bool isPressed(uint8_t gamepad, uint8_t button) {
    MY_ASSERT(1 <= gamepad && gamepad <= 4);
    MY_ASSERT(
        button == BUTTON_1 || button == BUTTON_2 || button == BUTTON_LEFT ||
        button == BUTTON_RIGHT || button == BUTTON_UP || button == BUTTON_DOWN
    );
    return pressed[gamepad - 1] & button;
}

// detects if any button was pressed during the current frame
_Bool isAnyPressed(uint8_t gamepad) {
    MY_ASSERT(1 <= gamepad && gamepad <= 4);
    return pressed[gamepad - 1] & 0xFF;
}
