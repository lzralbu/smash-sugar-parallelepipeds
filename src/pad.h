#ifndef _PAD_H
#define _PAD_H

#include "utils.h"
#include <stdint.h>

#define PAD_INITIAL_WIDTH 25
#define PAD_INITIAL_HEIGHT 3
#define PAD_INITIAL_X ((SCREEN_SIZE - PAD_INITIAL_WIDTH) / 2)
#define PAD_INITIAL_Y (SCREEN_SIZE - PAD_INITIAL_HEIGHT - 1)
#define PAD_INITIAL_SPEED 3

typedef struct Pad {
    Point pos;
    Point size;
    int16_t speed;
} Pad;

void Pad_reset(Pad *pad);
void Pad_moveBy(Pad *pad, int16_t dx);

Rect Pad_getAABB(Pad const * pad);
void Pad_onCollision(CollisionData const * collisionData);

void Pad_draw(Pad const * pad);

#endif // _PAD_H
