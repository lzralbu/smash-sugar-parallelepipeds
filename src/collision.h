#ifndef COLLISION_H
#define COLLISION_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int16_t position[2]; // top-left
    int16_t velocity[2];
} RigidBody;

// pad, ball(s) and blocks
typedef struct {
    RigidBody *body;
    int16_t d[2]; // width and height
} CollisionRectangle;

// will be used for the screen boundaries
typedef struct {
    int16_t normal[2];
    int16_t offset; // signed distance from origin
} CollisionLine;

typedef struct {
    RigidBody *firstBody;
    RigidBody *secondBody;
    int16_t contactPoint[2];
    int16_t contactNormal[2];
    int16_t penetration;
} Contact;

typedef struct {
    Contact *contacts;
    uint8_t length;
    uint8_t capacity;
} CollisionData;

bool intersectionTestRectangleAndRectangle(
    const CollisionRectangle *rect1, const CollisionRectangle *rect2
);

bool intersectionTestRectangleAndHalfPlane(
    const CollisionRectangle *rect, const CollisionLine *line
);

uint8_t collisionRectangleAndRectangle(
    const CollisionRectangle *rect1, const CollisionRectangle *rect2,
    CollisionData *data
);

uint8_t collisionRectangleAndHalfPlane(
    const CollisionRectangle *a, const CollisionLine *line, CollisionData *data
);

#endif // COLLISION_H
