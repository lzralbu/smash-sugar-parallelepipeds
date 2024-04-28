#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define FPS 60
#define DELTA_TIME (1.0f / FPS)
#define TIME_STEP (1.0f / FPS)

#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifdef NDEBUG
#define MY_ASSERT(x) (void)0
#else
#include "wasm4.h"
#include <stdlib.h>
#define MY_ASSERT(x)                                                           \
    ((void)((x) || (tracef(                                                    \
                        "Assertion failed: %s\nFilename: %s\nLine: "           \
                        "%d\nFunction name: %s\n\n",                           \
                        #x,                                                    \
                        __FILE__,                                              \
                        __LINE__,                                              \
                        __func__                                               \
                    ),                                                         \
                    abort(),                                                   \
                    0)));
#endif

char *itoa(char *dest, int i);

#endif // UTILS_H
