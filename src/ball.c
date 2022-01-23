#include "ball.h"
#include "game.h"
#include "wasm4.h"
#include <stdlib.h>

void Ball_reset(Ball *ball) {

    static int arr[2] = { -1, 1 };
    double speed = randrangef(1.625,  2.375);

    *ball = (Ball) {
        .pos = {
            .x = BALL_INITIAL_POS_X,
            .y = BALL_INITIAL_POS_Y
        },
        .velocity = {
            .x = speed * arr[randrange(0, 2)],
            .y = -speed
        }
    };
}

void Ball_move(Ball *ball) {

    ball->pos.x += ball->velocity.x;
    ball->pos.y += ball->velocity.y;
}

Rect Ball_getAABB(Ball const * ball) {

    return (Rect) {
        .min = { ball->pos.x, ball->pos.y },
        .max = { ball->pos.x + BALL_WIDTH, ball->pos.y + BALL_HEIGHT }
    };
}

void Ball_onCollision(CollisionData const * collisionData) {

    Ball *ball = (Ball *) collisionData->hittingActor;
    Rect const *const intersection = collisionData->intersection;
    Rect const *const hittingActorAABB = collisionData->hittingActorAABB;
    Rect const *const hitActorAABB = collisionData->hitActorAABB;

    if (collisionData->hitActorType == ACTOR_SCREEN) {
        if ((intersection->min.x <= hitActorAABB->min.x) || (intersection->max.x >= hitActorAABB->max.x)) {
            ball->pos.x -= ball->velocity.x + 1;
            ball->velocity.x *= -1;
        }

        if ((intersection->min.y <= hitActorAABB->min.y)) {
            ball->pos.y -= ball->velocity.y + 1;
            ball->velocity.y *= -1;
        }

        // FLOOR IS LAVA
        if (intersection->max.y >= hitActorAABB->max.y) {
            tone(600 | (100 << 16), 15, 100, TONE_PULSE1);
            Ball_reset(ball);
            Game_decrementLives();
        }
    } else if (collisionData->hitActorType == ACTOR_PAD) {
        ball->pos.y -= ball->velocity.y + 0.5;
        ball->velocity.y *= -1;
    } else if (collisionData->hitActorType == ACTOR_BLOCK) {
        tone(100 | (500 << 16), 3, 100, TONE_PULSE2);

        LOG("hittingActorAABB->min.x = %d\nhittingActorAABB->min.y = %d\nhittingActorAABB->max.x = %d\nhittingActorAABB->max.y = %d\n\n", hittingActorAABB->min.x, hittingActorAABB->min.y, hittingActorAABB->max.x, hittingActorAABB->max.y);

        LOG("intersection->min.x = %d\nintersection->min.y = %d\nintersection->max.x = %d\nintersection->max.y = %d\n\n", intersection->min.x, intersection->min.y, intersection->max.x, intersection->max.y);

        LOG("|***********************************************\n\n\n");

        int intersectionWidth = intersection->max.x - intersection->min.x;
        int intersectionHeight = intersection->max.y - intersection->min.y;

        // ball->velocity.y *= -1;
        if (hittingActorAABB->min.x == intersection->min.x) {
            ball->pos.x += intersectionWidth;
            ball->velocity.x *= -1;
        } else if (hittingActorAABB->max.x == intersection->max.x) {
            ball->pos.x -= intersectionWidth;
            ball->velocity.x *= -1;
        } else if (hittingActorAABB->min.y <= intersection->min.y) {
            ball->pos.y += intersectionHeight;
            ball->velocity.y *= -1;
        } else if (hittingActorAABB->max.y >= intersection->max.y) {
            ball->pos.y -= intersectionHeight;
            ball->velocity.y *= -1;
        }

        Block *block = (Block *) collisionData->hitActor;
        if (block->type == BLOCK_NORMAL) {
            Game_incrementScore();
        } else if (block->type == BLOCK_1UP) {
            Game_incrementScore();
            Game_incrementLives();
        }
    }
}

void Ball_draw(Ball const * ball) {

    *DRAW_COLORS = 0x33;
    rect(ball->pos.x, ball->pos.y, BALL_WIDTH, BALL_HEIGHT);
}
