#ifndef AABB_H
#define AABB_H

#include <stdbool.h>
#include <stdint.h>

#include "entity.h"

typedef struct {
    int16_t l; // left
    int16_t r; // right
    int16_t t; // top
    int16_t b; // bottom
} AABB;

int16_t aabbWidth(const AABB *aabb);
int16_t aabbHeight(const AABB *aabb);

void aabbFromRect(int16_t x, int16_t y, int16_t w, int16_t h, AABB *out);
void aabbFromEntity(const Entity *ent, AABB *out);

bool aabbHasIntersection(const AABB *p, const AABB *q);
bool aabbFindIntersection(const AABB *p, const AABB *q, AABB *out);

#endif // AABB_H
