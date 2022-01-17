#ifndef _BALL_H
#define _BALL_H

#include "utils.h"
#include "pad.h"

#define BALL_WIDTH 2
#define BALL_HEIGHT BALL_WIDTH
#define BALL_INITIAL_POS_X ((SCREEN_SIZE - 2 * BALL_WIDTH) / 2)
#define BALL_INITIAL_POS_Y (SCREEN_SIZE * 0.75)

typedef struct Ball {
    Pointf pos;
    Pointf velocity;
} Ball;

void Ball_reset(Ball *ball);
void Ball_move(Ball *ball);

Rect Ball_getAABB(Ball const * ball);
void Ball_onCollision(CollisionData const * collisionData);

void Ball_draw(Ball const * ball);

#endif // _BALL_H
