//
// heligun: 3D flight sim shooter
// Copyright (c) 2016 Joseph Kuziel
//
// This software is MIT licensed.
//


#include <math.h>


#define M_PI_2X                 6.28318530717959f


#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define CLAMP(x, a, b) (MIN(MAX(x, a), b))

inline float wrap(float x, float min, float max) {
    x = fmod(x + max, (max - min));
    if (x < 0) {
        x += (max - min);
    }
    return x + min;
}

#define WRAPF(x, min, max) (fmod((x) + (max), ((max) - (min))) + (min))

#define SCALE(x, x1, x2, y1, y2) \
    ((y1) + ((x) - (x1)) * ((y2) - (y1)) / ((x2) - (x1)))

#define RADIANS_TO_DEGREES(x)   x * 57.295f
