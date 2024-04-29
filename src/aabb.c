#include "aabb.h"
#include "entity.h"
#include "utils.h"

int16_t aabbWidth(const AABB *aabb) {
    return aabb->r - aabb->l + 1;
}

int16_t aabbHeight(const AABB *aabb) {
    return aabb->b - aabb->t + 1;
}

void aabbFromRect(int16_t x, int16_t y, int16_t w, int16_t h, AABB *out) {
    out->l = x;
    out->r = x + w - 1;
    out->t = y;
    out->b = y + h - 1;
}

void aabbFromEntity(const Entity *ent, AABB *out) {
    aabbFromRect(ent->x, ent->y, ent->width, ent->height, out);
}

bool aabbHasIntersection(const AABB *p, const AABB *q) {
    return MAX(p->l, q->l) <= MIN(p->r, q->r) &&
           MAX(p->t, q->t) <= MIN(p->b, q->b);
}

bool aabbFindIntersection(const AABB *p, const AABB *q, AABB *out) {
    int16_t l = MAX(p->l, q->l);
    int16_t r = MIN(p->r, q->r);
    int16_t t = MAX(p->t, q->t);
    int16_t b = MIN(p->b, q->b);

    if (l <= r && t <= b) {
        if (out) {
            out->l = l;
            out->r = r;
            out->t = t;
            out->b = b;
        }
        return true;
    }
    return false;
}
