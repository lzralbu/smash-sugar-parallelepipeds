#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

// #define MAKE_MIN(TYPE) \
//         TYPE min##_TYPE(TYPE a, TYPE b) { return a < b ? a : b; }

// MAKE_MIN(int)

// min_int(a, b)

char* itoa(char* dest, int i);

// chooses a random int "uniformly" from the interval [a, b)
int randrange(int a, int b);

int chooseInt(int *arr, size_t len);

// chooses a random float "uniformly" from the interval [a, b)
double randrangef(double a, double b);

typedef enum Boolean {
    FALSE = 0,
    TRUE = 1
} Boolean;

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct Pointf {
    double x;
    double y;
} Pointf;

typedef struct Rect {
    Point min;
    Point max;
} Rect;

Boolean Rect_hasIntersectionWithRect(Rect const *const a, Rect const *const b);
void Rect_intersectionWithRect(Rect const *const a, Rect const *const b, Rect *intersection);

typedef enum GameActorType {
    ACTOR_SCREEN = 1 << 0,
    ACTOR_PAD    = 1 << 1,
    ACTOR_BALL   = 1 << 2,
    ACTOR_BLOCK  = 1 << 3
} GameActorType;

typedef struct CollisionData {
    void *const hittingActor;
    Rect const *const hittingActorAABB;
    GameActorType const hitActorType;
    Rect const *const hitActorAABB;
    Rect const *const intersection;
} CollisionData;

typedef void (*CollisionCallback)(CollisionData const *const collisionData);

#endif // _UTILS_H
