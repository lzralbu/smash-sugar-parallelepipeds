#ifndef PAD_H
#define PAD_H

#include <stdint.h>

typedef struct {
    int16_t x;
    int16_t y;
    int16_t w;   // width
    int16_t h;   // height
    float rx;    // signed fractional part of x-position in the range (-1, 1)
    float speed; // always non-negative
    int8_t direction; // -1, 0, 1
    uint8_t color;
} Pad;

void padInit(
    Pad *pad, int16_t x, int16_t y, int16_t w, int16_t h, float rx, float speed,
    int8_t direction, uint8_t color
);
void padMove(Pad *pad);
void padDraw(const Pad *pad);

#endif // PAD_H
