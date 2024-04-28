#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int16_t x;
    int16_t y;
    float rx; // (-1, 1)
    float ry; // (-1, 1)
    float vx; // pixels/frame
    float vy; // pixels/frame
    int16_t width;
    int16_t height;
    uint8_t color;
    bool alive;
} Entity;

void entityMove(Entity *ent);
void entityDraw(const Entity *ent);

#endif // ENTITY_H
