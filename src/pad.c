#include "pad.h"
#include "wasm4.h"

void Pad_reset(Pad *pad) {

    *pad = (struct Pad) {
        .pos = {
            .x = PAD_INITIAL_X,
            .y = PAD_INITIAL_Y
        },
        .size = {
            .x = PAD_INITIAL_WIDTH,
            .y = PAD_INITIAL_HEIGHT
        },
        .speed = PAD_INITIAL_SPEED
    };
}

void Pad_moveBy(Pad *pad, int16_t dx) {

    pad->pos.x += dx;
}

Rect Pad_getAABB(Pad const * pad) {

    return (Rect) {
        .min = { pad->pos.x, pad->pos.y },
        .max = { pad->pos.x + pad->size.x, pad->pos.y + pad->size.y }
    };
}

void Pad_onCollision(CollisionData const * collisionData) {

    Pad *pad = (Pad *) collisionData->hittingActor;

    if (collisionData->hitActorType == ACTOR_SCREEN) {
        if (pad->pos.x < 0) {
            pad->pos.x = 0;
        } else if (pad->pos.x > SCREEN_SIZE - pad->size.x) {
            pad->pos.x = SCREEN_SIZE - pad->size.x;
        }
    }
}

void Pad_draw(Pad const * pad) {

    *DRAW_COLORS = 0x22;
    rect(pad->pos.x, pad->pos.y, pad->size.x, pad->size.y);
}
