#include "gameLevel.h"

#include "color.h"
#include "entity.h"
#include "input.h"
#include "utils.h"

#include "wasm4.h"

#include <math.h>

#define PAD_MAX_SPEED 2.0f

enum {
    GAME_PLAYING,
    GAME_WON,
    GAME_OVER
};

enum {
    GRID_COLUMNS = 8,
    GRID_ROWS = 10,
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
    entities[ENTITY_BALL].vx = -1.5f;
    entities[ENTITY_BALL].vy = -1.1f;
    entities[ENTITY_BALL].color = COLOR_4;
    entities[ENTITY_BALL].alive = true;
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
            text(
                TEXT_MISSION, (SCREEN_SIZE - 8 * sizeof(TEXT_MISSION)) / 2, 50
            );
            text(
                TEXT_ACCOMPLISHED,
                (SCREEN_SIZE - 8 * sizeof(TEXT_ACCOMPLISHED)) / 2,
                60
            );
        } else if (status == GAME_OVER) {
            static char TEXT_YOU_LOST[] = "GAME OVER!";
            text(
                TEXT_YOU_LOST, (SCREEN_SIZE - 8 * sizeof(TEXT_YOU_LOST)) / 2, 50
            );
        }

        static char TEXT_CLICK_TO_PLAY_AGAIN[] = "CLICK TO PLAY";
        *DRAW_COLORS = 0x4;
        text(
            TEXT_CLICK_TO_PLAY_AGAIN,
            (SCREEN_SIZE - 8 * sizeof(TEXT_CLICK_TO_PLAY_AGAIN)) / 2,
            150
        );
    }
}

static void ballReset(void) {}

static bool aabbRectIntersection(
    int16_t x1, int16_t y1, int16_t w1, int16_t h1, int16_t x2, int16_t y2,
    int16_t w2, int16_t h2, int16_t *xOut, int16_t *yOut, int16_t *wOut,
    int16_t *hOut
) {
    int16_t l1 = x1;
    int16_t r1 = x1 + w1;
    int16_t t1 = y1;
    int16_t b1 = y1 + h1;

    int16_t l2 = x2;
    int16_t r2 = x2 + w2;
    int16_t t2 = y2;
    int16_t b2 = y2 + h2;

    int16_t l3 = MAX(l1, l2);
    int16_t r3 = MIN(r1, r2);
    int16_t t3 = MAX(t1, t2);
    int16_t b3 = MIN(b1, b2);

    if (l3 < r3 && t3 < b3) {
        if (xOut && yOut && wOut && hOut) {
            *xOut = l3;
            *yOut = t3;
            *wOut = r3 - l3;
            *hOut = b3 - t3;
        }
        return true;
    }
    return false;
}

static void checkCollisions(void) {
    static int16_t screenLeftX = -10;
    static int16_t screenLeftY = 0;
    static int16_t screenLeftWidth = 10;
    static int16_t screenLeftHeight = 160;

    static int16_t screenRightX = 160;
    static int16_t screenRightY = 0;
    static int16_t screenRightWidth = 10;
    static int16_t screenRightHeight = 160;

    static int16_t screenTopX = 0;
    static int16_t screenTopY = -10;
    static int16_t screenTopWidth = 160;
    static int16_t screenTopHeight = 10;

    static int16_t screenBottomX = 0;
    static int16_t screenBottomY = 160;
    static int16_t screenBottomWidth = 160;
    static int16_t screenBottomHeight = 10;

    int16_t intersectionX = 0;
    int16_t intersectionY = 0;
    int16_t intersectionWidth = 0;
    int16_t intersectionHeight = 0;
    bool collided = false;

    // pad vs screen left
    if (aabbRectIntersection(
            entities[ENTITY_PAD].x,
            entities[ENTITY_PAD].y,
            entities[ENTITY_PAD].width,
            entities[ENTITY_PAD].height,
            screenLeftX,
            screenLeftY,
            screenLeftWidth,
            screenLeftHeight,
            &intersectionX,
            &intersectionY,
            &intersectionWidth,
            &intersectionHeight
        )) {
        entities[ENTITY_PAD].x += intersectionWidth;
        entities[ENTITY_PAD].vx *= -1;
    }

    // pad vs screen right
    if (aabbRectIntersection(
            entities[ENTITY_PAD].x,
            entities[ENTITY_PAD].y,
            entities[ENTITY_PAD].width,
            entities[ENTITY_PAD].height,
            screenRightX,
            screenRightY,
            screenRightWidth,
            screenRightHeight,
            &intersectionX,
            &intersectionY,
            &intersectionWidth,
            &intersectionHeight
        )) {
        entities[ENTITY_PAD].x -= intersectionWidth;
        entities[ENTITY_PAD].vx *= -1;
    }

    // ball vs screen left
    if (aabbRectIntersection(
            entities[ENTITY_BALL].x,
            entities[ENTITY_BALL].y,
            entities[ENTITY_BALL].width,
            entities[ENTITY_BALL].height,
            screenLeftX,
            screenLeftY,
            screenLeftWidth,
            screenLeftHeight,
            &intersectionX,
            &intersectionY,
            &intersectionWidth,
            &intersectionHeight
        )) {
        entities[ENTITY_BALL].x += intersectionWidth;
        entities[ENTITY_BALL].vx *= -1;
    }

    // ball vs screen right
    if (aabbRectIntersection(
            entities[ENTITY_BALL].x,
            entities[ENTITY_BALL].y,
            entities[ENTITY_BALL].width,
            entities[ENTITY_BALL].height,
            screenRightX,
            screenRightY,
            screenRightWidth,
            screenRightHeight,
            &intersectionX,
            &intersectionY,
            &intersectionWidth,
            &intersectionHeight
        )) {
        entities[ENTITY_BALL].x -= intersectionWidth;
        entities[ENTITY_BALL].vx *= -1;
    }

    // ball vs screen top
    if (aabbRectIntersection(
            entities[ENTITY_BALL].x,
            entities[ENTITY_BALL].y,
            entities[ENTITY_BALL].width,
            entities[ENTITY_BALL].height,
            screenTopX,
            screenTopY,
            screenTopWidth,
            screenTopHeight,
            &intersectionX,
            &intersectionY,
            &intersectionWidth,
            &intersectionHeight
        )) {
        entities[ENTITY_BALL].y += intersectionHeight;
        entities[ENTITY_BALL].vy *= -1;
    }

    // ball vs screen bottom
    if (aabbRectIntersection(
            entities[ENTITY_BALL].x,
            entities[ENTITY_BALL].y,
            entities[ENTITY_BALL].width,
            entities[ENTITY_BALL].height,
            screenBottomX,
            screenBottomY,
            screenBottomWidth,
            screenBottomHeight,
            &intersectionX,
            &intersectionY,
            &intersectionWidth,
            &intersectionHeight
        )) {
        entities[ENTITY_BALL].y -= intersectionHeight;
        entities[ENTITY_BALL].vy *= -1;

        lives--;
        if (lives == 0) {
            status = GAME_OVER;
        }

        tone(600 | (100 << 16), 15, 100, TONE_PULSE1);
    }

    // ball vs pad
    if (aabbRectIntersection(
            entities[ENTITY_BALL].x,
            entities[ENTITY_BALL].y,
            entities[ENTITY_BALL].width,
            entities[ENTITY_BALL].height,
            entities[ENTITY_PAD].x,
            entities[ENTITY_PAD].y,
            entities[ENTITY_PAD].width,
            entities[ENTITY_PAD].height,
            &intersectionX,
            &intersectionY,
            &intersectionWidth,
            &intersectionHeight
        )) {
        entities[ENTITY_BALL].y -= intersectionHeight;
        entities[ENTITY_BALL].vy *= -1;
    }

    // ball vs blocks
    for (int i = 0; i < GRID_TOTAL; ++i) {
        if (!entities[i].alive) {
            continue;
        }

        if (!aabbRectIntersection(
                entities[ENTITY_BALL].x,
                entities[ENTITY_BALL].y,
                entities[ENTITY_BALL].width,
                entities[ENTITY_BALL].height,
                entities[i].x,
                entities[i].y,
                entities[i].width,
                entities[i].height,
                &intersectionX,
                &intersectionY,
                &intersectionWidth,
                &intersectionHeight
            )) {
            continue;
        }

        tone(100 | (500 << 16), 3, 100, TONE_PULSE2);

        score++;
        if (score == GRID_TOTAL) {
            status = GAME_WON;
        }

        int16_t blockLeftX = entities[i].x;
        int16_t blockLeftY = entities[i].y;
        int16_t blockLeftWidth = 1;
        int16_t blockLeftHeight = entities[i].height;

        int16_t blockRightX = entities[i].x + entities[i].width - 1;
        int16_t blockRightY = entities[i].y;
        int16_t blockRightWidth = 1;
        int16_t blockRightHeight = entities[i].height;

        int16_t blockTopX = entities[i].x;
        int16_t blockTopY = entities[i].y;
        int16_t blockTopWidth = entities[i].width;
        int16_t blockTopHeight = 1;

        int16_t blockBottomX = entities[i].x;
        int16_t blockBottomY = entities[i].y + entities[i].height - 1;
        int16_t blockBottomWidth = entities[i].width;
        int16_t blockBottomHeight = 1;

        // ball vs block left
        if (aabbRectIntersection(
                entities[ENTITY_BALL].x,
                entities[ENTITY_BALL].y,
                entities[ENTITY_BALL].width,
                entities[ENTITY_BALL].height,
                blockLeftX,
                blockLeftY,
                blockLeftWidth,
                blockLeftHeight,
                &intersectionX,
                &intersectionY,
                &intersectionWidth,
                &intersectionHeight
            )) {
            entities[ENTITY_BALL].x -= intersectionWidth;
            entities[ENTITY_BALL].vx *= -1;
            entities[i].alive = false;
        }

        // ball vs block right
        else if (aabbRectIntersection(
                     entities[ENTITY_BALL].x,
                     entities[ENTITY_BALL].y,
                     entities[ENTITY_BALL].width,
                     entities[ENTITY_BALL].height,
                     blockRightX,
                     blockRightY,
                     blockRightWidth,
                     blockRightHeight,
                     &intersectionX,
                     &intersectionY,
                     &intersectionWidth,
                     &intersectionHeight
                 )) {
            entities[ENTITY_BALL].x += intersectionWidth;
            entities[ENTITY_BALL].vx *= -1;
            entities[i].alive = false;
        }

        // ball vs block top
        else if (aabbRectIntersection(
                     entities[ENTITY_BALL].x,
                     entities[ENTITY_BALL].y,
                     entities[ENTITY_BALL].width,
                     entities[ENTITY_BALL].height,
                     blockTopX,
                     blockTopY,
                     blockTopWidth,
                     blockTopHeight,
                     &intersectionX,
                     &intersectionY,
                     &intersectionWidth,
                     &intersectionHeight
                 )) {
            entities[ENTITY_BALL].y -= intersectionHeight;
            entities[ENTITY_BALL].vy *= -1;
            entities[i].alive = false;
        }

        // ball vs block bottom
        else if (aabbRectIntersection(
                     entities[ENTITY_BALL].x,
                     entities[ENTITY_BALL].y,
                     entities[ENTITY_BALL].width,
                     entities[ENTITY_BALL].height,
                     blockBottomX,
                     blockBottomY,
                     blockBottomWidth,
                     blockBottomHeight,
                     &intersectionX,
                     &intersectionY,
                     &intersectionWidth,
                     &intersectionHeight
                 )) {
            entities[ENTITY_BALL].y += intersectionHeight;
            entities[ENTITY_BALL].vy *= -1;
            entities[i].alive = false;
        }

        break;
    }
}
