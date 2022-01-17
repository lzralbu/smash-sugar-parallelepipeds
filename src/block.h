#ifndef _BLOCK_H
#define _BLOCK_H

#include "utils.h"

#define BLOCK_MAGIC_N 8
#define BLOCK_MAGIC_D 5
#define BLOCK_MAGIC_K (-19)

#define BLOCK_WIDTH (SCREEN_SIZE - 2 * BLOCK_MAGIC_D + (BLOCK_MAGIC_N - 1) * BLOCK_MAGIC_K)
#define BLOCK_HORIZONTAL_SPACE (-SCREEN_SIZE + 2 * BLOCK_MAGIC_D - BLOCK_MAGIC_N * BLOCK_MAGIC_K)

#define BLOCK_HEIGHT 4
#define BLOCK_VERTICAL_SPACE 2

#define BLOCK_TOTAL_COLUMNS BLOCK_MAGIC_N
#define BLOCK_TOTAL_ROWS 8

#define BLOCK_INITIAL_HEALTH 1

typedef struct Block {
    Point pos;
    int16_t health;
} Block;

void Block_reset(Block *block);

Rect Block_getAABB(Block const * block);
void Block_onCollision(CollisionData const * collisionData);

void Block_draw(Block const * block);

#endif // _BLOCK_H
