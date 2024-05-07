#include "game.h"

#include "wasm4.h"

static Game game;

#include "block.h"
#include <stddef.h>
#include <limits.h>

#include "collision.h"

void start(void) {
    // DREAM CANDY PALETTE(https://lospec.com/palette-list/dream-candy)
    PALETTE[0] = 0x442d6e;
    PALETTE[1] = 0xd075b7;
    PALETTE[2] = 0xf0d063;
    PALETTE[3] = 0xffffff;

    *DRAW_COLORS = 0;

    // tracef("offsetof(Block, x) = %d\n", (int)offsetof(Block, x) * CHAR_BIT);
    // tracef("offsetof(Block, y) = %d\n", (int)offsetof(Block, y) * CHAR_BIT);
    // tracef(
    //     "offsetof(Block, color) = %d\n", (int)offsetof(Block, color) *
    //     CHAR_BIT
    // );
    // tracef("sizeof(Block) = %d\n", (int)sizeof(Block) * CHAR_BIT);
    // tracef("sizeof(BlockManager) = %d\n", (int)sizeof(BlockManager) *
    // CHAR_BIT);

    /*
    <p, n> = -3
    */

    CollisionLine screenLeftLine = { .normal = { 1, 0 }, .offset = -1 };
    CollisionLine screenRightLine = { .normal = { -1, 0 }, .offset = -160 };
    CollisionLine screenTopLine = { .normal = { 0, 1 }, .offset = -1 };
    CollisionLine screenBottomLine = { .normal = { 0, -1 }, .offset = -160 };

    RigidBody body = { .position = { 0, -5 } };
    CollisionRectangle rect = { .body = &body, .d = { 200, 170 } };
    Contact contacts[10];
    CollisionData collisionData = { .contacts = contacts,
                                    .length = 0,
                                    .capacity = 10 };

    collisionRectangleAndHalfPlane(&rect, &screenLeftLine, &collisionData);
    collisionRectangleAndHalfPlane(&rect, &screenRightLine, &collisionData);
    collisionRectangleAndHalfPlane(&rect, &screenTopLine, &collisionData);
    collisionRectangleAndHalfPlane(&rect, &screenBottomLine, &collisionData);
    for (uint8_t i = 0; i < collisionData.length; ++i) {
        tracef(
            "contactPoint = (%d, %d)\ncontactNormal = (%d, "
            "%d)\npenetration %d\n\n",
            collisionData.contacts[i].contactPoint[0],
            collisionData.contacts[i].contactPoint[1],
            collisionData.contacts[i].contactNormal[0],
            collisionData.contacts[i].contactNormal[1],
            collisionData.contacts[i].penetration
        );
    }

    // RigidBody shortBody = {
    //     .position = { 5, 10 },
    // };
    // CollisionRectangle shortRect = { .body = &shortBody, .d = { 3, 2 } };

    // RigidBody longBody = {
    //     .position = { 5, 5 },
    // };
    // CollisionRectangle longRect = { .body = &longBody, .d = { 7, 2 } };

    // if (intersectionTestRectangleAndHalfPlane(&shortRect, &line)) {
    //     trace("short rectangle: FOUND INTERSECTION");
    // } else {
    //     trace("short rectangle: DIDN'T FIND INTERSECTION");
    // }

    // if (intersectionTestRectangleAndHalfPlane(&longRect, &line)) {
    //     trace("long rectangle: FOUND INTERSECTION");
    // } else {
    //     trace("long rectangle: DIDN'T FIND INTERSECTION");
    // }

    // gameStart(&game);
}

void update(void) {
    // *DRAW_COLORS = 0x30;
    // rect(2, 2, 30, 30);
    // *DRAW_COLORS = 3;
    // uint8_t x = 8;
    // uint8_t y = 23;
    // uint8_t w = 11;
    // uint8_t h = 5;
    // // rect(8, 23, 11, 5);
    // // rect(8, 23, w, h);
    // rect(x, y, w, h);
    // gameUpdate(&game);
}
