#include "entity.h"

#include "color.h"
#include "utils.h"

#include "wasm4.h"

#include <math.h>

static void moveHelper(int16_t *pos, float *remainder, float velocity) {
    float displacement = velocity + *remainder;
    float actualDisplacement = truncf(displacement); // round towards zero
    *remainder = displacement - actualDisplacement;
    *pos += (int16_t)actualDisplacement;
}

void entityMove(Entity *ent) {
    if (!ent->alive) {
        return;
    }
    moveHelper(&ent->x, &ent->rx, ent->vx);
    moveHelper(&ent->y, &ent->ry, ent->vy);
}

void entityDraw(const Entity *ent) {
    if (!ent->alive) {
        return;
    }
    setDrawColor(COLOR_PRIMARY, ent->color);
    rect(ent->x, ent->y, (uint32_t)ent->width, (uint32_t)ent->height);
}

