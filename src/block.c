#include "wasm4.h"

#include "block.h"
#include "utils.h"
#include "color.h"

void blockManagerInit(
    BlockManager *manager, uint8_t blockWidth, uint8_t blockHeight,
    uint8_t cols, uint8_t rows, uint8_t hSpace, uint8_t vSpace
) {
    MY_ASSERT(rows <= 15 && cols <= 15);

    manager->cols = cols;
    manager->rows = rows;
    manager->hSpace = hSpace;
    manager->vSpace = vSpace;
    manager->numTotal = rows * cols;
    manager->numAlive = manager->numTotal;
    for (uint8_t i = 0; i < rows; ++i) {
        for (uint8_t j = 0; j < cols; ++j) {
            uint8_t blockIndex = i * cols + j;
            manager->blocks[blockIndex].body.position[0] =
                hSpace + (blockWidth + hSpace) * j;
            manager->blocks[blockIndex].body.position[1] =
                15 + (hSpace + (blockHeight + vSpace) * i);
            manager->blocks[blockIndex].body.velocity[0] = 0;
            manager->blocks[blockIndex].body.velocity[1] = 0;
            manager->blocks[blockIndex].rect.body =
                &manager->blocks[blockIndex].body;
            manager->blocks[blockIndex].rect.d[0] = blockWidth;
            manager->blocks[blockIndex].rect.d[1] = blockHeight;
            manager->blocks[blockIndex].color = COLOR_3;
        }
    }
}

void blockManagerDraw(const BlockManager *manager) {
    for (int i = 0; i < manager->numAlive; ++i) {
        setDrawColor(COLOR_PRIMARY, manager->blocks[i].color);
        rect(
            manager->blocks[i].body.position[0],
            manager->blocks[i].body.position[1],
            (uint32_t)manager->blocks[i].rect.d[0],
            (uint32_t)manager->blocks[i].rect.d[1]
        );
    }
}

void blockManagerActivateBlock(BlockManager *manager, uint8_t blockIndex) {
    MY_ASSERT(blockIndex < manager->numTotal);
    MY_ASSERT(blockIndex >= manager->numAlive);

    Block temp = manager->blocks[manager->numAlive];
    manager->blocks[manager->numAlive] = manager->blocks[blockIndex];
    manager->blocks[blockIndex] = temp;
    ++manager->numAlive;
}

void blockManagerDeactivateBlock(BlockManager *manager, uint8_t blockIndex) {
    MY_ASSERT(blockIndex < manager->numAlive);

    --manager->numAlive;
    Block temp = manager->blocks[manager->numAlive];
    manager->blocks[manager->numAlive] = manager->blocks[blockIndex];
    manager->blocks[blockIndex] = temp;

    manager->blocks[blockIndex].rect.body = &manager->blocks[blockIndex].body;
}
