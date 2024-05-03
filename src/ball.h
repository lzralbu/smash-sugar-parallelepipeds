#ifndef BALL_H
#define BALL_H

#include <stdint.h>

typedef struct {
    int16_t x;
    int16_t y;
    int16_t w; // width
    int16_t h; // height
    float rx;  // signed fractional part of x-position in the range (-1, 1)
    float ry;  // signed fractional part of y-position in the range (-1, 1)
    float vx;  // horizontal velocity in pixels/frame
    float vy;  // vertical velocity in pixels/frame
    uint8_t color;
} Ball;

void ballInit(
    Ball *ball, int16_t x, int16_t y, int16_t w, int16_t h, float rx, float ry,
    float vx, float vy, uint8_t color
);
void ballMove(Ball *ball);
void ballDraw(const Ball *ball);

#endif // BALL_H
