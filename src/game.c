#include <math.h>
#include "wasm4.h"

#include "game.h"
#include "input.h"

void gameStart(Game *game) {
    gameChangeState(game, GAME_STATE_MENU);
}

void gameUpdate(Game *game) {
    inputUpdate();

    if (game->flag == GAME_STATE_MENU) {
        menuUpdate(&game->data.menu, game);
    } else if (game->flag == GAME_STATE_ONGOING) {
        levelUpdate(&game->data.level, game);
    } else if (game->flag == GAME_STATE_LOST) {
        if (isAnyPressed(1)) {
            gameChangeState(game, GAME_STATE_MENU);
        }
        char TEXT_YOU_LOST[] = "GAME OVER!";
        text(TEXT_YOU_LOST, (SCREEN_SIZE - 8 * 10) / 2, 60);
    } else if (game->flag == GAME_STATE_WON) {
        if (isAnyPressed(1)) {
            gameChangeState(game, GAME_STATE_MENU);
        }
        char TEXT_MISSION[] = "MISSION";
        char TEXT_ACCOMPLISHED[] = "ACCOMPLISHED";
        text(TEXT_MISSION, (SCREEN_SIZE - 8 * 7) / 2, 60);
        text(TEXT_ACCOMPLISHED, (SCREEN_SIZE - 8 * 12) / 2, 70);
    }

    // switch (game->flag) {
    //     case GAME_STATE_MENU:
    //         menuUpdate(&game->data.menu, game);
    //         break;
    //     case GAME_STATE_ONGOING:
    //         levelUpdate(&game->data.level, game);
    //         break;
    //     case GAME_STATE_WON:
    //         char TEXT_MISSION[] = "MISSION";
    //         char TEXT_ACCOMPLISHED[] = "ACCOMPLISHED";
    //         text(TEXT_MISSION, (SCREEN_SIZE - 8 * 7) / 2, 60);
    //         text(TEXT_ACCOMPLISHED, (SCREEN_SIZE - 8 * 12) / 2, 70);
    //         break;
    //     case GAME_STATE_LOST:
    //         char TEXT_YOU_LOST[] = "GAME OVER!";
    //         text(TEXT_YOU_LOST, (SCREEN_SIZE - 8 * 10) / 2, 60);
    //         break;
    //     default:
    //         break;
    // }
}

void gameChangeState(Game *game, GameStateFlag newState) {
    game->flag = newState;
    switch (newState) {
        case GAME_STATE_MENU:
            menuStart(&game->data.menu);
            break;
        case GAME_STATE_ONGOING:
            levelStart(&game->data.level);
            break;
        case GAME_STATE_LOST:
        case GAME_STATE_WON:
        default:
            break;
    }
}
