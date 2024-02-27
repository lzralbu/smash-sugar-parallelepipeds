#include "utils.h"
#include "wasm4.h"
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>

// #ifdef DEBUG
// #define STB_SPRINTF_IMPLEMENTATION
// // #define STB_SPRINTF_NOFLOAT
// #include "stb_sprintf.h"

// void LOG(const char* fmt, ...) {
//     static char __logBuffer[256];
//     va_list args;
//     va_start(args, fmt);
//     stbsp_vsnprintf(__logBuffer, sizeof(__logBuffer), fmt, args);
//     trace(__logBuffer);
//     va_end(args);
// }
// #else
// void LOG(const char* fmt, ...) { (void)fmt; }
// #endif

void LOG(const char* fmt, ...) { (void)fmt; }

static char *itoa_helper(char *dest, int i) {
    if (i <= -10) {
        dest = itoa_helper(dest, i / 10);
    }
    *dest++ = '0' - i % 10;
    return dest;
}

char *itoa(char *dest, int i) {
    char *s = dest;
    if (i < 0) {
        *s++ = '-';
    } else {
        i = -i;
    }
    *itoa_helper(s, i) = '\0';
    return dest;
}

double randomNormalized(void)
{
    return rand() / (double)RAND_MAX;
}

double randrangef(double a, double b) {
    return a + (b - a) * randomNormalized();
}

int randrange(int a, int b) {
    return (int)(a + (b - a) * randomNormalized());
}

// float Point_distanceToPoint(const Point *p, const Point *q) {
//     return sqrt(pow(p->x - q->x, 2) + pow(p->y - q->y, 2));
// }

// typedef struct HorizontalSegment {
//     int16_t x1;
//     int16_t x2;
//     int16_t y0;
// } HorizontalSegment;

// // x1 < x2
// float Point_distanceToHorizontalSegment(const Point *p, const HorizontalSegment *s) {

//     if (p->x < s->x1) return Point_distanceToPoint(p, &((Point){ s->x1, s->y0 }));
//     if (p->x > s->x2) return Point_distanceToPoint(p, &((Point){ s->x2, s->y0 }));
//     return abs(p->y - s->y0);
// }

// typedef struct VerticalSegment {
//     int16_t y1;
//     int16_t y2;
//     int16_t x0;
// } VerticalSegment;

// // y1 < y2
// float Point_distanceToVerticalSegment(const Point *p, const VerticalSegment *s) {

//     if (p->y < s->y1) return Point_distanceToPoint(p, &((Point){ s->x0, s->y1 }));
//     if (p->y > s->y2) return Point_distanceToPoint(p, &((Point){ s->x0, s->y2 }));
//     return abs(p->x - s->x0);
// }

// Boolean Rect_containsPoint(const Rect *r, const Point *p) {

//     return 
//         ((r->min.x <= p->x) && (p->x <= r->max.x)) && 
//         ((r->min.y <= p->y) && (p->y <= r->max.y));
// }

Boolean Rect_hasIntersectionWithRect(Rect const * a, Rect const * b) {

    return 
        (a->min.x <= b->max.x && a->max.x >= b->min.x) && 
        (a->min.y <= b->max.y && a->max.y >= b->min.y);
}

// only makes sense if Rect_hasIntersectionWithRect(r1, r2) is TRUE
void Rect_intersectionWithRect(Rect const * a, Rect const * b, Rect *intersection) {

    intersection->min.x = max(a->min.x, b->min.x);
    intersection->min.y = max(a->min.y, b->min.y);
    intersection->max.x = min(a->max.x, b->max.x);
    intersection->max.y = min(a->max.y, b->max.y);
}
