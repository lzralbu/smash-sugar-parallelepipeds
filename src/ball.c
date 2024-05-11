#include "wasm4.h"

#include "ball.h"
#include "color.h"

void ballInit(
    Ball *ball, int16_t x, int16_t y, int16_t w, int16_t h, int16_t vx,
    int16_t vy, uint8_t color
) {
    ball->body.position[0] = x;
    ball->body.position[1] = y;
    ball->body.velocity[0] = vx;
    ball->body.velocity[1] = vy;
    ball->rect.body = &ball->body;
    ball->rect.d[0] = w;
    ball->rect.d[1] = h;
    ball->color = color;
}

void ballMove(Ball *ball) {
    ball->body.position[0] += ball->body.velocity[0];
    ball->body.position[1] += ball->body.velocity[1];
}

void ballDraw(const Ball *ball) {
    setDrawColor(COLOR_PRIMARY, ball->color);
    rect(
        ball->body.position[0],
        ball->body.position[1],
        (uint32_t)ball->rect.d[0],
        (uint32_t)ball->rect.d[1]
    );
}
