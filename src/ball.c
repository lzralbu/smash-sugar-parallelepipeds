#include <math.h>
#include "wasm4.h"

#include "ball.h"
#include "color.h"

void ballInit(
    Ball *ball, int16_t x, int16_t y, int16_t w, int16_t h, float rx, float ry,
    float vx, float vy, uint8_t color
) {
    ball->x = x;
    ball->y = y;
    ball->w = w;
    ball->h = h;
    ball->rx = rx;
    ball->ry = ry;
    ball->vx = vx;
    ball->vy = vy;
    ball->color = color;
}

void ballMove(Ball *ball) {
    {
        float displacement = ball->vx + ball->rx;
        float actualDisplacement = truncf(displacement);
        ball->rx = displacement - actualDisplacement;
        ball->x += (int16_t)actualDisplacement;
    }
    {
        float displacement = ball->vy + ball->ry;
        float actualDisplacement = truncf(displacement);
        ball->ry = displacement - actualDisplacement;
        ball->y += (int16_t)actualDisplacement;
    }
}

void ballDraw(const Ball *ball) {
    setDrawColor(COLOR_PRIMARY, ball->color);
    rect(ball->x, ball->y, (uint32_t)ball->w, (uint32_t)ball->h);
}
