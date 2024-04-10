#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdint.h>

enum AnimationType {
    ANIMATION_FORWARD,
    ANIMATION_BACKWARD,
    ANIMATION_PINGPONG
};

typedef struct {
    int8_t __direction;
    uint8_t __timer;
    uint8_t rate;    // playback rate in frames
    uint8_t first;   // first index of the animation
    uint8_t last;    // last index of the animation
    uint8_t type;    // animation type
    uint8_t current; // current index of the animation
} Animation;

void animationStart(
    Animation *animation, uint8_t rate, uint8_t first, uint8_t last, uint8_t type
);
void animationUpdate(Animation *animation);

#endif // ANIMATION_H
