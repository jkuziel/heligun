//
// heligun: 3D flight sim shooter
// Copyright (c) 2016 Joseph Kuziel
//
// This software is MIT licensed.
//


#include <math.h>


#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define CLAMP(x, a, b) (MIN(MAX(x, a), b))

#define RADIANS_TO_DEGREES(x) x * 57.295f
