#include <stdlib.h>

#include "collision.h"

bool intersectionTestRectangleAndRectangle(
    const CollisionRectangle *a, const CollisionRectangle *b
) {
    int16_t t;
    if ((t = a->body->position[0] - b->body->position[0]) > b->d[0] ||
        -t > a->d[0]) {
        return false;
    }
    if ((t = a->body->position[1] - b->body->position[1]) > b->d[1] ||
        -t > a->d[1]) {
        return false;
    }
    return true;
}

static int16_t dot(const int16_t *u, const int16_t *v) {
    return u[0] * v[0] + u[1] * v[1];
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
    const CollisionRectangle *a, const CollisionRectangle *b,
    CollisionData *data
) {
    if (data->length >= data->capacity) {
        return 0;
    }

    if (!intersectionTestRectangleAndRectangle(a, b)) {
        return 0;
    }

    // TODO

    return 0;
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
