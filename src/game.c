#include "game.h"
#include "wasm4.h"

#ifdef DEBUG
#define STB_SPRINTF_IMPLEMENTATION
// #define STB_SPRINTF_NOFLOAT
#include "stb_sprintf.h"

static char __gameLogBuffer[256];
#define LOG(fmt, ...) { \
            stbsp_snprintf(__gameLogBuffer, sizeof(__gameLogBuffer), fmt, ##__VA_ARGS__); \
            trace(__gameLogBuffer); \
        }
#else
#define LOG(fmt, ...) {}
#endif

static Game game;

typedef void (*GameGridProcessor)(Block *block, void *arg);

void __Game_gridForEach(GameGridProcessor processor, void *arg) {

    for (int i = 0; i < BLOCK_TOTAL_ROWS; ++i) {
        for (int j = 0; j < BLOCK_TOTAL_COLUMNS; ++j) {
            processor(&game.grid[i][j], arg);
        }
    }
}

void Game_reset() {

    LOG("LOGGING IS GREAT!!!\n");
    LOG("BLOCK_WIDTH %d\nBLOCK_HORIZONTAL_SPACE %d\n\n", BLOCK_WIDTH, BLOCK_HORIZONTAL_SPACE);

    Pad_reset(&game.pad);
    Ball_reset(&game.ball);

    LOG("game.ball.velocity.x: %f, game.ball.velocity.y: %f\n", game.ball.velocity.x, game.ball.velocity.y);

    Point currentPosition = { BLOCK_MAGIC_D, 17 };
    for (int i = 0; i < BLOCK_TOTAL_ROWS; ++i) {
        currentPosition.x = BLOCK_MAGIC_D;
        for (int j = 0; j < BLOCK_TOTAL_COLUMNS; ++j) {
            struct Block *block = &game.grid[i][j];
            block->pos = currentPosition;
            block->health = BLOCK_INITIAL_HEALTH;

            currentPosition.x += BLOCK_WIDTH + BLOCK_HORIZONTAL_SPACE;
        }
        currentPosition.y += BLOCK_HEIGHT + BLOCK_VERTICAL_SPACE;
    }

    game.score = 0;
    game.lives = 3;
    game.paused = TRUE;
}

void Game_processInput() {

    uint8_t gamepad = *GAMEPAD1;
    if (gamepad & BUTTON_RIGHT) {
        Pad_moveBy(&game.pad, game.pad.speed);
    } else if (gamepad & BUTTON_LEFT) {
        Pad_moveBy(&game.pad, (-1) * game.pad.speed);
    }
}

void Game_update() {

    Ball_move(&game.ball);
}

void __Game_processCollision(Rect const *const AABB1, CollisionCallback cb1, void *actor1, GameActorType actorType1,
                             Rect const *const AABB2, CollisionCallback cb2, void *actor2, GameActorType actorType2) {

    if (Rect_hasIntersectionWithRect(AABB1, AABB2)) {
        Rect intersection;
        Rect_intersectionWithRect(AABB1, AABB2, &intersection);

        if (cb1) {
            cb1(&(CollisionData) {
                .hittingActor = actor1, 
                .hittingActorAABB = AABB1,
                .hitActorType = actorType2,
                .hitActorAABB = AABB2, 
                .intersection = &intersection
            });
        }
        if (cb2) {
            cb2(&(CollisionData) {
                .hittingActor = actor2, 
                .hittingActorAABB = AABB2,
                .hitActorType = actorType1, 
                .hitActorAABB = AABB1, 
                .intersection = &intersection
            });
        }
    }
}

void __Game_processBallBlockCollision(Block *block, void *arg) {

    if (block->health <= 0) return;

    Rect blockAABB = {
        .min = { block->pos.x, block->pos.y },
        .max = { block->pos.x + BLOCK_WIDTH, block->pos.y + BLOCK_HEIGHT }
    };
    __Game_processCollision((Rect *)arg, Ball_onCollision, &game.ball, ACTOR_BALL,
                            &blockAABB, Block_onCollision, block, ACTOR_BLOCK);
}

void Game_processCollision() {

    static Rect screenAABB = {
        .min = { -1, 0 },
        .max = { SCREEN_SIZE, SCREEN_SIZE }
    };
    Rect padAABB = Pad_getAABB(&game.pad);
    Rect ballAABB = Ball_getAABB(&game.ball);

    __Game_processCollision(&padAABB, Pad_onCollision, &game.pad, ACTOR_PAD,
                            &screenAABB, 0, 0, ACTOR_SCREEN);

    __Game_processCollision(&ballAABB, Ball_onCollision, &game.ball, ACTOR_BALL,
                            &screenAABB, 0, 0, ACTOR_SCREEN);

    __Game_processCollision(&padAABB, Pad_onCollision, &game.pad, ACTOR_PAD,
                            &ballAABB, Ball_onCollision, &game.ball, ACTOR_BALL);

    __Game_gridForEach(__Game_processBallBlockCollision, &ballAABB);
}

void __Game_drawBlock(Block *block, void *arg) {

    if (block->health <= 0) return;

    // Used to following trick to get it of "error: parameter 'arg' set but not used"
    (void)arg;
    Block_draw(block);
}

void Game_draw() {

    Pad_draw(&game.pad);
    Ball_draw(&game.ball);

    __Game_gridForEach(__Game_drawBlock, 0);

    *DRAW_COLORS = 0x03;

    char textScore[16] = "SCORE:";
    itoa(textScore + 6, game.score);
    text(textScore, GAME_SCORE_X, GAME_SCORE_Y);

    char textLives[16] = "LIVES:";
    itoa(textLives + 6, game.lives);
    text(textLives, GAME_LIVES_X, GAME_LIVES_Y);
}

void Game_incrementScore() {

    game.score++;
}
