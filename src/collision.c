#include <stdlib.h>

#include "collision.h"
#include "utils.h"

bool intersectionTestRectangleAndRectangle(
    const CollisionRectangle *rect1, const CollisionRectangle *rect2
) {
    int16_t left1 = rect1->body->position[0];
    int16_t right1 = rect1->body->position[0] + rect1->d[0] - 1;
    int16_t top1 = rect1->body->position[1];
    int16_t bottom1 = rect1->body->position[1] + rect1->d[1] - 1;

    int16_t left2 = rect2->body->position[0];
    int16_t right2 = rect2->body->position[0] + rect2->d[0] - 1;
    int16_t top2 = rect2->body->position[1];
    int16_t bottom2 = rect2->body->position[1] + rect2->d[1] - 1;

    if (left1 > right2 || left2 > right1) {
        return false;
    }
    if (top1 > bottom2 || top2 > bottom1) {
        return false;
    }
    return true;
}

bool intersectionTestRectangleAndHalfPlane(
    const CollisionRectangle *rect, const CollisionLine *line
) {
    int16_t projectedRadius =
        dot(rect->d,
            (int16_t[2]){ (int16_t)abs(line->normal[0]),
                          (int16_t)abs(line->normal[1]) });
    int16_t boxDoubleDistance =
        dot(line->normal,
            (int16_t[2]){ 2 * rect->body->position[0] + rect->d[0],
                          2 * rect->body->position[1] + rect->d[1] }) -
        projectedRadius;
    return boxDoubleDistance <= 2 * line->offset;
}

uint8_t collisionRectangleAndRectangle(
    const CollisionRectangle *rect1, const CollisionRectangle *rect2,
    CollisionData *data
) {
    if (data->length >= data->capacity) {
        return 0;
    }

    int16_t left1 = rect1->body->position[0];
    int16_t right1 = rect1->body->position[0] + rect1->d[0] - 1;
    int16_t top1 = rect1->body->position[1];
    int16_t bottom1 = rect1->body->position[1] + rect1->d[1] - 1;

    int16_t left2 = rect2->body->position[0];
    int16_t right2 = rect2->body->position[0] + rect2->d[0] - 1;
    int16_t top2 = rect2->body->position[1];
    int16_t bottom2 = rect2->body->position[1] + rect2->d[1] - 1;

    if (left1 > right2 || left2 > right1) {
        return 0;
    }
    if (top1 > bottom2 || top2 > bottom1) {
        return 0;
    }

    /*

    // I'll assume that neither of the following situations ever happens:
    //     left2 <= left1 <= right1 <= right2
    //     top2 <= top1 <= bottom1 <= bottom2
    IGNORE the comments above, for now

    The possible scenarios are(only parallel edge-edge collisions are
    considered):

    vertical edges check:

        left1 can only cross right2
        right1 can only cross left2

    horizontal edges check:

        top1 can only cross bottom2
        bottom1 can only cross top2

    */

    uint8_t initialDataLength = data->length;

    // bool forbiddenVerticalEdgesPosition = left2 <= left1 && right1 <= right2;
    // left1 crosses right2
    // if (!forbiddenVerticalEdgesPosition && left2 <= left1 && left1 <= right2)
    // {
    if (left2 <= left1 && left1 <= right2) {
        data->contacts[data->length].contactPoint[0] = right2;
        data->contacts[data->length].contactPoint[1] = MAX(top1, top2);

        data->contacts[data->length].contactNormal[0] = 1;
        data->contacts[data->length].contactNormal[1] = 0;

        data->contacts[data->length].penetration = right2 - left1 + 1;

        data->contacts[data->length].firstBody = rect1->body;
        data->contacts[data->length].secondBody = rect2->body;

        ++data->length;
    }

    // right1 crosses left2
    // if (!forbiddenVerticalEdgesPosition && left2 <= right1 &&
    //     right1 <= right2) {
    if (left2 <= right1 && right1 <= right2) {
        data->contacts[data->length].contactPoint[0] = left2;
        data->contacts[data->length].contactPoint[1] = MAX(top1, top2);

        data->contacts[data->length].contactNormal[0] = -1;
        data->contacts[data->length].contactNormal[1] = 0;

        data->contacts[data->length].penetration = right1 - left2 + 1;

        data->contacts[data->length].firstBody = rect1->body;
        data->contacts[data->length].secondBody = rect2->body;

        ++data->length;
    }

    // bool forbiddenHorizontalEdgesPosition = top2 <= top1 && bottom1 <=
    // bottom2; top1 crosses bottom2 if (!forbiddenHorizontalEdgesPosition &&
    // top2 <= top1 && top1 <= bottom2) {
    if (top2 <= top1 && top1 <= bottom2) {
        data->contacts[data->length].contactPoint[0] = MAX(left1, left2);
        data->contacts[data->length].contactPoint[1] = bottom2;

        data->contacts[data->length].contactNormal[0] = 0;
        data->contacts[data->length].contactNormal[1] = 1;

        data->contacts[data->length].penetration = bottom2 - top1 + 1;

        data->contacts[data->length].firstBody = rect1->body;
        data->contacts[data->length].secondBody = rect2->body;

        ++data->length;
    }

    // bottom1 crosses top2
    // if (!forbiddenHorizontalEdgesPosition && top2 <= bottom1 &&
    //     bottom1 <= bottom2) {
    if (top2 <= bottom1 && bottom1 <= bottom2) {
        data->contacts[data->length].contactPoint[0] = MAX(left1, left2);
        data->contacts[data->length].contactPoint[1] = top2;

        data->contacts[data->length].contactNormal[0] = 0;
        data->contacts[data->length].contactNormal[1] = -1;

        data->contacts[data->length].penetration = bottom1 - top2 + 1;

        data->contacts[data->length].firstBody = rect1->body;
        data->contacts[data->length].secondBody = rect2->body;

        ++data->length;
    }

    return data->length - initialDataLength;
}

uint8_t collisionRectangleAndHalfPlane(
    const CollisionRectangle *rect, const CollisionLine *line,
    CollisionData *data
) {
    if (data->length >= data->capacity) {
        return 0;
    }

    if (!intersectionTestRectangleAndHalfPlane(rect, line)) {
        return 0;
    }

    // // horizontal line
    // if (line->normal[0] == 0) {
    // }
    // // vertical line
    // else {}

    int16_t vertices[4][2] = {
        { rect->body->position[0], rect->body->position[1] },
        { rect->body->position[0] + rect->d[0], rect->body->position[1] },
        { rect->body->position[0] + rect->d[0],
          rect->body->position[1] + rect->d[1] },
        { rect->body->position[0], rect->body->position[1] + rect->d[1] }
    };
    uint8_t initialLength = data->length;
    for (int i = 0; i < 4; ++i) {
        int16_t vertexDistance = dot(vertices[i], line->normal);
        int16_t distanceDifference = line->offset - vertexDistance;
        if (distanceDifference >= 0) {
            data->contacts[data->length].contactPoint[0] =
                vertices[i][0] + distanceDifference * line->normal[0];
            data->contacts[data->length].contactPoint[1] =
                vertices[i][1] + distanceDifference * line->normal[1];

            data->contacts[data->length].contactNormal[0] = line->normal[0];
            data->contacts[data->length].contactNormal[1] = line->normal[1];

            data->contacts[data->length].penetration = distanceDifference;

            data->contacts[data->length].firstBody = rect->body;
            data->contacts[data->length].secondBody = 0;

            ++data->length;
            if (data->length >= data->capacity) {
                break;
            }
        }
    }

    return data->length - initialLength;
}
