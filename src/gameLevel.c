#include <math.h>

#include "gameLevel.h"
#include "animation.h"

#include "color.h"
#include "entity.h"
#include "input.h"
#include "aabb.h"
#include "utils.h"

#include "wasm4.h"

#define PAD_MAX_SPEED 2.0f

enum {
    GAME_PLAYING,
    GAME_WON,
    GAME_OVER
};

enum {
    GRID_COLUMNS = 8,
    GRID_ROWS = 8,
    GRID_TOTAL = GRID_COLUMNS * GRID_ROWS,
    ENTITY_BALL = GRID_TOTAL,
    ENTITY_PAD,
    ENTITIES_TOTAL
};

static bool paused = true;
static int score = 0;
static int lives = 3;
static int status = GAME_PLAYING;

static Entity entities[ENTITIES_TOTAL] = { 0 };

static Animation footer;

static void ballReset(void);
static void checkCollisions(void);

void gameLevelStart(void) {
    score = 0;
    lives = 3;

    int16_t blockWidth = 11;
    int16_t horizontalSpace = 8;
    int16_t blockHeight = 5;
    int16_t verticalSpace = 2;
    for (int i = 0; i < GRID_ROWS; ++i) {
        for (int j = 0; j < GRID_COLUMNS; ++j) {
            int blockIndex = i * GRID_COLUMNS + j;
            entities[blockIndex].x =
                (int16_t)(horizontalSpace + (blockWidth + horizontalSpace) * j);
            entities[blockIndex].y =
                15 +
                (int16_t)(horizontalSpace + (blockHeight + verticalSpace) * i);
            entities[blockIndex].rx = 0;
            entities[blockIndex].ry = 0;
            entities[blockIndex].vx = 0;
            entities[blockIndex].vy = 0;
            entities[blockIndex].width = blockWidth;
            entities[blockIndex].height = 5;
            entities[blockIndex].color = COLOR_3;
            entities[blockIndex].alive = true;
        }
    }

    entities[ENTITY_PAD].width = 30;
    entities[ENTITY_PAD].height = 1;
    entities[ENTITY_PAD].x = (160 - entities[ENTITY_PAD].width) / 2;
    entities[ENTITY_PAD].y = 160 - 2 * entities[ENTITY_PAD].height;
    entities[ENTITY_PAD].rx = 0;
    entities[ENTITY_PAD].ry = 0;
    entities[ENTITY_PAD].vx = PAD_MAX_SPEED;
    entities[ENTITY_PAD].vy = 0;
    entities[ENTITY_PAD].color = COLOR_2;
    entities[ENTITY_PAD].alive = true;

    entities[ENTITY_BALL].width = 2;
    entities[ENTITY_BALL].height = 2;
    entities[ENTITY_BALL].x = entities[ENTITY_PAD].x +
                              entities[ENTITY_PAD].width / 2 -
                              entities[ENTITY_BALL].width / 2;
    entities[ENTITY_BALL].y =
        entities[ENTITY_PAD].y - entities[ENTITY_BALL].height;
    entities[ENTITY_BALL].rx = 0;
    entities[ENTITY_BALL].ry = 0;
    entities[ENTITY_BALL].vx = -1.75f;
    entities[ENTITY_BALL].vy = -1.25f;
    entities[ENTITY_BALL].color = COLOR_4;
    entities[ENTITY_BALL].alive = true;

    animationStart(&footer, 4, 1, 2, ANIMATION_FORWARD);
}

void gameLevelUpdate(void) {
    if (status == GAME_PLAYING) {
        if (isDown(1, BUTTON_LEFT)) {
            entities[ENTITY_PAD].vx = -PAD_MAX_SPEED;
        } else if (isDown(1, BUTTON_RIGHT)) {
            entities[ENTITY_PAD].vx = PAD_MAX_SPEED;
        } else {
            entities[ENTITY_PAD].vx = 0;
        }

        for (int i = 0; i < ENTITIES_TOTAL; ++i) {
            entityMove(&entities[i]);
        }

        checkCollisions();

        for (int i = 0; i < ENTITIES_TOTAL; ++i) {
            entityDraw(&entities[i]);
        }

        char textLives[16] = "LIVES:";
        itoa(textLives + 6, lives);
        text(textLives, 10, 10);
    } else {
        if (status == GAME_WON) {
            static char TEXT_MISSION[] = "MISSION";
            static char TEXT_ACCOMPLISHED[] = "ACCOMPLISHED";
            text(TEXT_MISSION, (SCREEN_SIZE - 8 * 7) / 2, 60);
            text(TEXT_ACCOMPLISHED, (SCREEN_SIZE - 8 * 12) / 2, 70);
        } else if (status == GAME_OVER) {
            static char TEXT_YOU_LOST[] = "GAME OVER!";
            text(TEXT_YOU_LOST, (SCREEN_SIZE - 8 * 10) / 2, 60);
        }
    }
}

static void ballReset(void) {}

static void checkCollisions(void) {
    AABB screenLeft = { 0 };
    aabbFromRect(-10, 0, 10, 160, &screenLeft);

    AABB screenRight = { 0 };
    aabbFromRect(160, 0, 10, 160, &screenRight);

    AABB screenTop = { 0 };
    aabbFromRect(0, -10, 160, 10, &screenTop);

    AABB screenBottom = { 0 };
    aabbFromRect(0, 160, 160, 10, &screenBottom);

    AABB ballAABB;
    aabbFromEntity(&entities[ENTITY_BALL], &ballAABB);

    AABB padAABB;
    aabbFromEntity(&entities[ENTITY_PAD], &padAABB);

    AABB intersection = { 0 };

    // pad vs screen left
    if (aabbFindIntersection(&padAABB, &screenLeft, &intersection)) {
        entities[ENTITY_PAD].x += aabbWidth(&intersection);
        entities[ENTITY_PAD].vx *= -1;
        aabbFromEntity(&entities[ENTITY_PAD], &padAABB);
    }

    // pad vs screen right
    if (aabbFindIntersection(&padAABB, &screenRight, &intersection)) {
        entities[ENTITY_PAD].x -= aabbWidth(&intersection);
        entities[ENTITY_PAD].vx *= -1;
        aabbFromEntity(&entities[ENTITY_PAD], &padAABB);
    }

    // ball vs screen left
    if (aabbFindIntersection(&ballAABB, &screenLeft, &intersection)) {
        entities[ENTITY_BALL].x += aabbWidth(&intersection);
        entities[ENTITY_BALL].vx *= -1;
        aabbFromEntity(&entities[ENTITY_BALL], &ballAABB);
    }

    // ball vs screen right
    if (aabbFindIntersection(&ballAABB, &screenRight, &intersection)) {
        entities[ENTITY_BALL].x -= aabbWidth(&intersection);
        entities[ENTITY_BALL].vx *= -1;
        aabbFromEntity(&entities[ENTITY_BALL], &ballAABB);
    }

    // ball vs screen top
    if (aabbFindIntersection(&ballAABB, &screenTop, &intersection)) {
        entities[ENTITY_BALL].y += aabbHeight(&intersection);
        entities[ENTITY_BALL].vy *= -1;
        aabbFromEntity(&entities[ENTITY_BALL], &ballAABB);
    }

    // ball vs screen bottom
    if (aabbFindIntersection(&ballAABB, &screenBottom, &intersection)) {
        entities[ENTITY_BALL].y -= aabbHeight(&intersection);
        entities[ENTITY_BALL].vy *= -1;
        aabbFromEntity(&entities[ENTITY_BALL], &ballAABB);

        lives--;
        if (lives == 0) {
            status = GAME_OVER;
        }

        tone(600 | (100 << 16), 15, 100, TONE_PULSE1);
    }

    // ball vs pad
    if (aabbFindIntersection(&ballAABB, &padAABB, &intersection)) {
        entities[ENTITY_BALL].y -= aabbHeight(&intersection);
        entities[ENTITY_BALL].vy *= -1;
        aabbFromEntity(&entities[ENTITY_BALL], &ballAABB);
    }

    // ball vs blocks
    for (int i = 0; i < GRID_TOTAL; ++i) {
        if (!entities[i].alive) {
            continue;
        }

        AABB blockAABB = { 0 };
        aabbFromEntity(&entities[i], &blockAABB);

        if (!aabbFindIntersection(&ballAABB, &blockAABB, &intersection)) {
            continue;
        }

        entities[i].alive = false;
        score++;
        if (score == GRID_TOTAL) {
            status = GAME_WON;
        }

        tone(100 | (500 << 16), 3, 100, TONE_PULSE2);

        AABB blockLeftAABB = { 0 };
        aabbFromRect(
            entities[i].x, entities[i].y, 1, entities[i].height, &blockLeftAABB
        );

        AABB blockRightAABB = { 0 };
        aabbFromRect(
            entities[i].x + entities[i].width - 1,
            entities[i].y,
            1,
            entities[i].height,
            &blockRightAABB
        );

        AABB blockTopAABB = { 0 };
        aabbFromRect(
            entities[i].x, entities[i].y, entities[i].width, 1, &blockTopAABB
        );

        AABB blockBottomAABB = { 0 };
        aabbFromRect(
            entities[i].x,
            entities[i].y + entities[i].height - 1,
            entities[i].width,
            1,
            &blockBottomAABB
        );

        // ball vs block left
        if (aabbFindIntersection(&ballAABB, &blockLeftAABB, &intersection) &&
            entities[ENTITY_BALL].vx > 0) {
            entities[ENTITY_BALL].x -= aabbWidth(&intersection);
            entities[ENTITY_BALL].vx *= -1;
            aabbFromEntity(&entities[ENTITY_BALL], &ballAABB);
        }

        // ball vs block right
        if (aabbFindIntersection(&ballAABB, &blockRightAABB, &intersection) &&
            entities[ENTITY_BALL].vx < 0) {
            entities[ENTITY_BALL].x += aabbWidth(&intersection);
            entities[ENTITY_BALL].vx *= -1;
            aabbFromEntity(&entities[ENTITY_BALL], &ballAABB);
        }

        // ball vs block top
        if (aabbFindIntersection(&ballAABB, &blockTopAABB, &intersection) &&
            entities[ENTITY_BALL].vy > 0) {
            entities[ENTITY_BALL].y -= aabbHeight(&intersection);
            entities[ENTITY_BALL].vy *= -1;
            aabbFromEntity(&entities[ENTITY_BALL], &ballAABB);
        }

        // ball vs block bottom
        if (aabbFindIntersection(&ballAABB, &blockBottomAABB, &intersection) &&
            entities[ENTITY_BALL].vy < 0) {
            entities[ENTITY_BALL].y += aabbHeight(&intersection);
            entities[ENTITY_BALL].vy *= -1;
            aabbFromEntity(&entities[ENTITY_BALL], &ballAABB);
        }

        break;
    }
}
