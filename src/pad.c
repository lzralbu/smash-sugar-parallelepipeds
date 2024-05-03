#include <math.h>
#include "wasm4.h"

#include "pad.h"
#include "color.h"

void padInit(
    Pad *pad, int16_t x, int16_t y, int16_t w, int16_t h, float rx, float speed,
    int8_t direction, uint8_t color
) {
    pad->x = x;
    pad->y = y;
    pad->w = w;
    pad->h = h;
    pad->rx = rx;
    pad->speed = speed;
    pad->direction = direction;
    pad->color = color;
}

void padMove(Pad *pad) {
    float velocity = pad->speed * pad->direction;
    float displacement = velocity + pad->rx;
    float actualDisplacement = truncf(displacement);
    pad->rx = displacement - actualDisplacement;
    pad->x += (int16_t)actualDisplacement;
}

void padDraw(const Pad *pad) {
    setDrawColor(COLOR_PRIMARY, pad->color);
    rect(pad->x, pad->y, (uint32_t)pad->w, (uint32_t)pad->h);
}
