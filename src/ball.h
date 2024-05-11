#ifndef BALL_H
#define BALL_H

#include <stdint.h>
#include "collision.h"

typedef struct {
    RigidBody body;
    CollisionRectangle rect;
    uint8_t color;
} Ball;

void ballInit(
    Ball *ball, int16_t x, int16_t y, int16_t w, int16_t h, int16_t vx,
    int16_t vy, uint8_t color
);
void ballMove(Ball *ball);
void ballDraw(const Ball *ball);

#endif // BALL_H
