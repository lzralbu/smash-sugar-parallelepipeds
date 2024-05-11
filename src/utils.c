#include "utils.h"

// out receives u + v * t
void addMultiple(const int16_t *u, const int16_t *v, int16_t t, int16_t *out) {
    int16_t result[2];
    result[0] = u[0] + v[0] * t;
    result[1] = u[1] + v[1] * t;
    out[0] = result[0];
    out[1] = result[1];
}

int16_t dot(const int16_t *u, const int16_t *v) {
    return u[0] * v[0] + u[1] * v[1];
}

void reflection(const int16_t *v, const int16_t *normal, int16_t *out) {
    int16_t t = dot(v, normal);
    addMultiple(v, normal, -2 * t, out);
}

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

