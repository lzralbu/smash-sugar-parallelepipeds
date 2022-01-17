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

    Game_incrementScore();
}

void Block_draw(Block const * block) {

    *DRAW_COLORS = 0x44;
    rect(block->pos.x, block->pos.y, BLOCK_WIDTH, BLOCK_HEIGHT);
}
