#include "ball.h"
#include "wasm4.h"
#include <stdlib.h>

void Ball_reset(Ball *ball) {

    static int arr[2] = { -1, 1 };

    *ball = (Ball) {
        .pos = {
            .x = BALL_INITIAL_POS_X,
            .y = BALL_INITIAL_POS_Y
        },
        .velocity = {
            .x = randrangef( 1.25,  2.0) * chooseInt(arr, 2),
            .y = randrangef(-2.0, -1.25)
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
            Ball_reset(ball);
            // ball->pos.y -= ball->velocity.y + 1;
            // ball->velocity.y *= -1;
            
            // Game_onFloorCollision();
            // gameState.lives--;

            // Pad_reset();
            // Ball_reset();
            // trace("FLOOR IS LAVA");
        }
    } else if (collisionData->hitActorType == ACTOR_PAD) {
        if ((intersection->min.x <= hitActorAABB->min.x) || (intersection->max.x >= hitActorAABB->max.x)) {
            ball->pos.x -= ball->velocity.x + 1;
            ball->velocity.x *= -1;
        }

        ball->velocity.y *= -1;
    } else if (collisionData->hitActorType == ACTOR_BLOCK) {
        int intersectionWidth = intersection->max.x - intersection->min.x;
        int intersectionHeight = intersection->max.y - intersection->min.y;

        if (abs(intersectionWidth - intersectionHeight) <= 2) {
            if ((intersection->min.x <= hitActorAABB->min.x) || (intersection->max.x >= hitActorAABB->max.x)) {
                ball->pos.x -= ball->velocity.x + 1;
                ball->velocity.x *= -1;
            }

            if ((intersection->min.y <= hitActorAABB->min.y) || (intersection->max.y >= hitActorAABB->max.y)) {
                ball->pos.y -= ball->velocity.y + 1;
                ball->velocity.y *= -1;
            }
        } if (intersectionWidth > intersectionHeight) {
            if ((intersection->min.x <= hitActorAABB->min.x) || (intersection->max.x >= hitActorAABB->max.x)) {
                ball->pos.x -= ball->velocity.x + 1;
                ball->velocity.x *= -1;
            }
        } else {
            if ((intersection->min.y <= hitActorAABB->min.y) || (intersection->max.y >= hitActorAABB->max.y)) {
                ball->pos.y -= ball->velocity.y + 1;
                ball->velocity.y *= -1;
            }
        }
    }
}

void Ball_draw(Ball const * ball) {

    *DRAW_COLORS = 0x33;
    rect(ball->pos.x, ball->pos.y, BALL_WIDTH, BALL_HEIGHT);
}
