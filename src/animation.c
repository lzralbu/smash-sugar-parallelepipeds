#include "animation.h"

#include "utils.h"

void animationStart(
    Animation *animation, uint8_t rate, uint8_t first, uint8_t last,
    uint8_t type
) {
    MY_ASSERT(first < last);
    MY_ASSERT(
        type == ANIMATION_FORWARD || type == ANIMATION_BACKWARD ||
        type == ANIMATION_PINGPONG
    );
    animation->rate = rate;
    animation->first = first;
    animation->last = last;
    animation->type = type;
    animation->current = first;
    animation->__timer = 0;
    animation->__direction = (type == ANIMATION_BACKWARD ? -1 : 1);
}

void animationUpdate(Animation *animation) {
    ++animation->__timer;
    if (animation->__timer < animation->rate) {
        return;
    }
    uint8_t prev = animation->current;
    // LESSON: DO NOT MIX SIGNED WITH UNSIGNED, UNLESS YOU KNOW EXACTLY WHAT
    // YOU'RE DOING
    animation->__timer = 0;
    if (animation->__direction > 0) {
        ++animation->current;
    } else {
        --animation->current;
    }
    uint8_t len = animation->last - animation->first + 1;
    animation->current =
        animation->first + (animation->current - animation->first + len) % len;

    if (animation->type == ANIMATION_PINGPONG &&
        (animation->current == animation->first ||
         animation->current == animation->last)) {
        animation->__direction *= -1;
    }
    // tracef("\n\tfrom: %d\n\tto: %d\n", prev, animation->current);
}
