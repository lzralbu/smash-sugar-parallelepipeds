#include "entity.h"

#include "color.h"
#include "utils.h"

#include "wasm4.h"

#include <math.h>

void entityUpdate(Entity *ent, _Bool move) {
    if (move) {
        ent->x += ent->vx;
        ent->y += ent->vy;
    }

    setDrawColor(COLOR_PRIMARY, ent->color);
    rect((int32_t)ent->x, (int32_t)ent->y, ent->width, ent->height);
    // rect(
    //     (int32_t)round(ent->x), (int32_t)round(ent->y), ent->width,
    //     ent->height
    // );
}
