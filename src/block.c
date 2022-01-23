#include "block.h"
#include "game.h"
#include "wasm4.h"

void Block_reset(Block *block) {
    block->health = BLOCK_INITIAL_HEALTH;
}

Rect Block_getAABB(Block const * block) {

    return (Rect) {
        .min = { block->pos.x, block->pos.y },
        .max = { block->pos.x + BLOCK_WIDTH, block->pos.y + BLOCK_HEIGHT }
    };
}

void Block_onCollision(CollisionData const * collisionData) {

    Block *block = (Block *) collisionData->hittingActor;
    if (block->health > 0) block->health--;
}

Boolean Block_isAlive(Block const * block) {

    return block->type == BLOCK_INVINCIBLE || block->health > 0;
}

void Block_draw(Block const * block) {

    if (block->type == BLOCK_NORMAL) {
        *DRAW_COLORS = 0x44;
        rect(block->pos.x, block->pos.y, BLOCK_WIDTH, BLOCK_HEIGHT);
    } else if (block->type == BLOCK_1UP) {
        *DRAW_COLORS = 0x34;
        rect(block->pos.x, block->pos.y, BLOCK_WIDTH, BLOCK_HEIGHT);
    } else if (block->type == BLOCK_INVINCIBLE) {
        *DRAW_COLORS = 0x24;
        rect(block->pos.x, block->pos.y, BLOCK_WIDTH, BLOCK_HEIGHT);
    }
}
