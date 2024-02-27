#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>

// DEBUG should be defined to work properly
void LOG(const char* fmt, ...);

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

char* itoa(char* dest, int i);

// selects an "uniformly" random number from the interval [0, 1) (or it might be [0, 1], not sure)
double randomNormalized(void);

double randrangef(double a, double b);

int randrange(int a, int b);

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

    void *const hitActor;
    GameActorType const hitActorType;
    Rect const *const hitActorAABB;
    
    Rect const *const intersection;
} CollisionData;

typedef void (*CollisionCallback)(CollisionData const *const collisionData);

#endif // _UTILS_H
