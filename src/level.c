#include <stdlib.h>
#include "wasm4.h"

#include "level.h"
#include "game.h"
#include "collision.h"
#include "color.h"
#include "input.h"
#include "utils.h"

static void checkCollision(Level *level, Game *game);

void levelStart(Level *level) {
    blockManagerInit(&level->blockManager, 11, 5, 8, 8, 8, 2);
    ballInit(&level->ball, 90, 140, 3, 3, 2, -2, COLOR_4);
    padInit(&level->pad, (160 - 32) / 2, 160 - 5, 32, 3, 3, COLOR_2);
    level->lives = 3;
}

void levelUpdate(Level *level, void *game) {
    if (isDown(1, BUTTON_LEFT)) {
        level->pad.body.velocity[0] =
            -(int16_t)abs(level->pad.body.velocity[0]);
        level->pad.moving = true;
    } else if (isDown(1, BUTTON_RIGHT)) {
        level->pad.body.velocity[0] = (int16_t)abs(level->pad.body.velocity[0]);
        level->pad.moving = true;
    } else {
        level->pad.moving = false;
    }

    ballMove(&level->ball);
    padMove(&level->pad);

    checkCollision(level, game);

    if (level->lives < 0) {
        gameChangeState(game, GAME_STATE_LOST);
        return;
    }

    if (level->blockManager.numAlive == 0) {
        gameChangeState(game, GAME_STATE_WON);
        return;
    }

    blockManagerDraw(&level->blockManager);
    ballDraw(&level->ball);
    padDraw(&level->pad);

    char textLives[10] = "LIVES:";
    itoa(textLives + 6, level->lives);

    setDrawColor(COLOR_PRIMARY, COLOR_2);
    text(textLives, 8, 8);
}

static CollisionLine screenBoundaryLines[4] = {
    // LEFT
    { .normal = { 1, 0 }, .offset = -1 },
    // RIGHT
    { .normal = { -1, 0 }, .offset = -160 },
    // TOP
    { .normal = { 0, 1 }, .offset = -1 },
    // BOTTOM
    { .normal = { 0, -1 }, .offset = -160 }
};

static Contact contacts[10];
static CollisionData collisionData = { .contacts = contacts,
                                       .length = 0,
                                       .capacity = 10 };

static void checkCollision(Level *level, Game *game) {
    collisionData.length = 0;

    // pad vs screen
    for (int i = 0; i < 2; ++i) {
        if (collisionRectangleAndHalfPlane(
                &level->pad.rect, &screenBoundaryLines[i], &collisionData
            ) == 0) {
            continue;
        }

        addMultiple(
            level->pad.body.position,
            collisionData.contacts[collisionData.length - 1].contactNormal,
            collisionData.contacts[collisionData.length - 1].penetration,
            level->pad.body.position
        );
    }

    collisionData.length = 0;

    // ball vs screen
    bool touchedBottom = false;
    for (int i = 0; i < 4; ++i) {
        if (collisionRectangleAndHalfPlane(
                &level->ball.rect, &screenBoundaryLines[i], &collisionData
            ) == 0) {
            continue;
        }

        addMultiple(
            level->ball.body.position,
            collisionData.contacts[collisionData.length - 1].contactNormal,
            collisionData.contacts[collisionData.length - 1].penetration,
            level->ball.body.position
        );

        reflection(
            level->ball.body.velocity,
            collisionData.contacts[collisionData.length - 1].contactNormal,
            level->ball.body.velocity
        );

        touchedBottom = i == 3;
    }
    if (touchedBottom) {
        --level->lives;
        tone(600 | (100 << 16), 15, 100, TONE_PULSE1);
    }

    collisionData.length = 0;

    // ball vs pad
    if (collisionRectangleAndRectangle(
            &level->ball.rect, &level->pad.rect, &collisionData
        ) > 0) {
        int16_t leastPenetration = INT16_MAX;
        int16_t leastNormal[2];
        for (int contactIndex = 0; contactIndex < collisionData.length;
             ++contactIndex) {
            if (collisionData.contacts[contactIndex].penetration <
                    leastPenetration &&
                dot(collisionData.contacts[contactIndex].contactNormal,
                    level->ball.body.velocity) < 0) {
                leastPenetration =
                    collisionData.contacts[contactIndex].penetration;
                leastNormal[0] =
                    collisionData.contacts[contactIndex].contactNormal[0];
                leastNormal[1] =
                    collisionData.contacts[contactIndex].contactNormal[1];
            }
        }

        // ignore collision with pad's bottom to avoid trapping the ball
        if (leastNormal[1] != 1) {
            addMultiple(
                level->ball.body.position,
                leastNormal,
                leastPenetration,
                level->ball.body.position
            );

            reflection(
                level->ball.body.velocity,
                leastNormal,
                level->ball.body.velocity
            );
        }
    }

    collisionData.length = 0;

    // ball vs blocks
    for (int i = 0; i < level->blockManager.numAlive; ++i) {
        if (collisionRectangleAndRectangle(
                &level->ball.rect,
                &level->blockManager.blocks[i].rect,
                &collisionData
            ) == 0) {
            continue;
        }

        int16_t leastPenetration = INT16_MAX;
        int16_t leastNormal[2];
        for (int contactIndex = 0; contactIndex < collisionData.length;
             ++contactIndex) {
            if (collisionData.contacts[contactIndex].penetration <
                    leastPenetration &&
                dot(collisionData.contacts[contactIndex].contactNormal,
                    level->ball.body.velocity) < 0) {
                leastPenetration =
                    collisionData.contacts[contactIndex].penetration;
                leastNormal[0] =
                    collisionData.contacts[contactIndex].contactNormal[0];
                leastNormal[1] =
                    collisionData.contacts[contactIndex].contactNormal[1];
            }
        }

        addMultiple(
            level->ball.body.position,
            leastNormal,
            leastPenetration,
            level->ball.body.position
        );

        reflection(
            level->ball.body.velocity, leastNormal, level->ball.body.velocity
        );

        blockManagerDeactivateBlock(&level->blockManager, (uint8_t)i);
        tone(100 | (500 << 16), 3, 100, TONE_PULSE2);

        collisionData.length = 0;

        break;
    }
}
