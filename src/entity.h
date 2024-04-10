#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>

typedef struct {
    float x;
    float y;
    float vx;
    float vy;
    uint16_t width;
    uint16_t height;
    uint8_t color;
} Entity;

void entityUpdate(Entity *ent, _Bool move);

#endif // ENTITY_H
