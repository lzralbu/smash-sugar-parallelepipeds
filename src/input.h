#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

void inputUpdate(void);
_Bool isDown(uint8_t gamepad, uint8_t button);

// detects if button was pressed during the current frame
_Bool isPressed(uint8_t gamepad, uint8_t button);

// detects if any button was pressed during the current frame
_Bool isAnyPressed(uint8_t gamepad);

#endif // INPUT_H
