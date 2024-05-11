#ifndef PAD_H
#define PAD_H

#include <stdint.h>
#include "collision.h"

typedef struct {
    RigidBody body;
    CollisionRectangle rect;
    uint8_t color;
    bool moving;
} Pad;

void padInit(
    Pad *pad, int16_t x, int16_t y, int16_t w, int16_t h, int16_t speed,
    uint8_t color
);
void padMove(Pad *pad);
void padDraw(const Pad *pad);

#endif // PAD_H
