#include "game.h"
#include "wasm4.h"
#include <stdlib.h>

static Game game;

Boolean firstRun = TRUE;

unsigned int entropyCollector = 0;

typedef void (*GameGridProcessor)(Block *block, void *arg);

void __Game_gridForEach(GameGridProcessor processor, void *arg) {

    for (int i = 0; i < BLOCK_TOTAL_ROWS; ++i) {
        for (int j = 0; j < BLOCK_TOTAL_COLUMNS; ++j) {
            processor(&game.grid[i][j], arg);
        }
    }
}

void Game_reset() {

    if (firstRun) {
        firstRun = FALSE;
    }

    srandom(entropyCollector);

    LOG("LOGGING IS GREAT!!!\n");
    LOG("BLOCK_WIDTH %d\nBLOCK_HORIZONTAL_SPACE %d\n\n", BLOCK_WIDTH, BLOCK_HORIZONTAL_SPACE);

    Pad_reset(&game.pad);
    Ball_reset(&game.ball);

    LOG("game.ball.velocity.x: %f, game.ball.velocity.y: %f\n", game.ball.velocity.x, game.ball.velocity.y);

    game.maximumScore = 0;

    int invincibleCount = 0;

    Point currentPosition = { BLOCK_MAGIC_D, 17 };
    for (int i = 0; i < BLOCK_TOTAL_ROWS; ++i) {
        currentPosition.x = BLOCK_MAGIC_D;
        for (int j = 0; j < BLOCK_TOTAL_COLUMNS; ++j) {
            struct Block *block = &game.grid[i][j];
            block->pos = currentPosition;

            double r = randomNormalized();
            if (r <= 0.05) {
                block->type = BLOCK_1UP;
                block->health = 1;
            } else if (r <= 0.95) {
                block->type = BLOCK_NORMAL;
                block->health = randrange(1, 4);
            } else {
                block->type = BLOCK_INVINCIBLE;
                block->health = 0;
                if (invincibleCount >= 7) {
                    r = randomNormalized();
                    if (r <= 0.03) {
                        block->type = BLOCK_1UP;
                        block->health = 1;
                    } else {
                        block->type = BLOCK_NORMAL;
                        block->health = randrange(1, 4);
                    }
                } else {
                    invincibleCount++;
                }
            }

            game.maximumScore += block->health;

            currentPosition.x += BLOCK_WIDTH + BLOCK_HORIZONTAL_SPACE;
        }
        currentPosition.y += BLOCK_HEIGHT + BLOCK_VERTICAL_SPACE;
    }

    game.score = 0;
    game.lives = 3;
}

void Game_processInput() {

    GameState state = Game_getState();

    uint8_t mouse = *MOUSE_BUTTONS;

    entropyCollector += (unsigned) *MOUSE_X + (unsigned) *MOUSE_Y;
    
    if (state == GAME_IN_PROGRESS) {
        uint8_t gamepad = *GAMEPAD1;
        if (gamepad & BUTTON_RIGHT) {
            entropyCollector += 3;
            Pad_moveBy(&game.pad, game.pad.speed);
        } else if (gamepad & BUTTON_LEFT) {
            Pad_moveBy(&game.pad, (-1) * game.pad.speed);
            entropyCollector += 37;
        }
    } else {
        if (mouse & MOUSE_LEFT) {
            entropyCollector += 739;
            Game_reset();
        }
    }
}

void Game_update() {

    entropyCollector++;

    Game_processInput();
    
    if (Game_getState() == GAME_IN_PROGRESS) {
        Ball_move(&game.ball);
        Game_processCollision();
    }
    
    Game_draw();
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
                .hitActor = actor2,
                .hitActorType = actorType2,
                .hitActorAABB = AABB2, 
                .intersection = &intersection
            });
        }
        if (cb2) {
            cb2(&(CollisionData) {
                .hittingActor = actor2, 
                .hittingActorAABB = AABB2,
                .hitActor = actor1,
                .hitActorType = actorType1, 
                .hitActorAABB = AABB1, 
                .intersection = &intersection
            });
        }
    }
}

void __Game_processBallBlockCollision(Block *block, void *arg) {

    (void)arg;

    if (!Block_isAlive(block)) return;

    Rect blockAABB = {
        .min = { block->pos.x, block->pos.y },
        .max = { block->pos.x + BLOCK_WIDTH, block->pos.y + BLOCK_HEIGHT }
    };
    Rect ballAABB = Ball_getAABB(&game.ball);

    __Game_processCollision(&ballAABB, Ball_onCollision, &game.ball, ACTOR_BALL,
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

    __Game_gridForEach(__Game_processBallBlockCollision, 0);
}

void __Game_drawBlock(Block *block, void *arg) {

    if (!Block_isAlive(block)) return;

    // Used to following trick to get it of "error: parameter 'arg' set but not used"
    (void)arg;
    Block_draw(block);
}

void Game_draw() {

    GameState state = Game_getState();

    if (state == GAME_WELCOME) {
        static char TEXT_SMASH_SUGAR[] = "SMASH SUGAR";
        static char TEXT_PARALLELEPIPEDS[] = "PARALLELEPIPEDS";
        *DRAW_COLORS = 0x32;
        text(TEXT_SMASH_SUGAR, (SCREEN_SIZE - 8 * sizeof(TEXT_SMASH_SUGAR)) / 2, 50);
        text(TEXT_PARALLELEPIPEDS, (SCREEN_SIZE - 8 * sizeof(TEXT_PARALLELEPIPEDS)) / 2, 60);

        static char TEXT_CLICK_TO_PLAY[] = "CLICK TO PLAY";
        *DRAW_COLORS = 0x4;
        text(TEXT_CLICK_TO_PLAY, (SCREEN_SIZE - 8 * sizeof(TEXT_CLICK_TO_PLAY)) / 2, 150);
    } else if (state == GAME_IN_PROGRESS) {
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
    } else {
        *DRAW_COLORS = 0x23;
        if (state == GAME_WON) {
            static char TEXT_MISSION[] = "MISSION";
            static char TEXT_ACCOMPLISHED[] = "ACCOMPLISHED";
            text(TEXT_MISSION, (SCREEN_SIZE - 8 * sizeof(TEXT_MISSION)) / 2, 50);
            text(TEXT_ACCOMPLISHED, (SCREEN_SIZE - 8 * sizeof(TEXT_ACCOMPLISHED)) / 2, 60);
        } else if (state == GAME_LOST) {
            static char TEXT_YOU_LOST[] = "GAME OVER!";
            text(TEXT_YOU_LOST, (SCREEN_SIZE - 8 * sizeof(TEXT_YOU_LOST)) / 2, 50);
        }
        static char TEXT_CLICK_TO_PLAY_AGAIN[] = "CLICK TO PLAY";
        *DRAW_COLORS = 0x4;
        text(TEXT_CLICK_TO_PLAY_AGAIN, (SCREEN_SIZE - 8 * sizeof(TEXT_CLICK_TO_PLAY_AGAIN)) / 2, 150);
    }
}

void Game_incrementScore() {
    game.score++;
}

void Game_incrementLives() {
    game.lives++;
}

void Game_decrementLives() {
    game.lives--;
}

GameState Game_getState() {

    if (firstRun) return GAME_WELCOME;
    if (game.lives < 0) return GAME_LOST;
    if (game.score == game.maximumScore) return GAME_WON;
    return GAME_IN_PROGRESS;
}
