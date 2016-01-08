//
// heligun: 3D flight sim shooter
// Copyright (c) 2016 Joseph Kuziel
//
// This software is MIT licensed.
//


#include "game.h"
#include "mathext.h"
#include <stdio.h>


// Globals

static Helicopter g_player;


// Constants

static const float HELI_MAX_THRUST                      = 20.0f;
static const float HELI_THROTTLE_INTERVAL               = 2.0f;

static const float HELI_X_ROTATION_MAX                  = 0.5f;
static const float HELI_Y_ROTATION_MAX                  = 0.3f;

// Helicopter rotational acceleration
static const float HELI_ROT_ACCEL                       = 5.0f;

static const float HELI_LINEAR_DAMPING                  = 0.2f;
static const float HELI_ROTATIONAL_DAMPING              = 0.2f;

static const float GRAVITY                              = 9.8f;


// Public Functions

void startGame() {

    g_player.px1 = 0.0f;
    g_player.py1 = 0.0f;
    g_player.pz1 = 0.5f;
    g_player.px2 = 0.0f;
    g_player.py2 = 0.0f;
    g_player.pz2 = 0.0f;
    g_player.px3 = 0.0f;
    g_player.py3 = 0.0f;
    g_player.pz3 = 0.0f;
    g_player.rx1 = 0.0f;
    g_player.ry1 = 0.0f;
    g_player.rz1 = 0.0f;
    g_player.rx2 = 0.0f;
    g_player.ry2 = 0.0f;
    g_player.rz2 = 0.0f;
    g_player.rx3 = 0.0f;
    g_player.ry3 = 0.0f;
    g_player.rz3 = 0.0f;
    g_player.thrust = 11.0f;
    g_player.throttle = 0.0f;
}


void updateGame(float time_delta) {

    // Throttle
    g_player.thrust += g_player.throttle * time_delta;
    g_player.thrust = CLAMP(g_player.thrust, 0.f, HELI_MAX_THRUST);


    // Rotational
    g_player.rx2 += g_player.rx3 * time_delta;
    g_player.ry2 += g_player.ry3 * time_delta;
    g_player.rz2 += g_player.rz3 * time_delta;

    g_player.rx2 -= g_player.rx2 * HELI_ROTATIONAL_DAMPING;
    g_player.ry2 -= g_player.ry2 * HELI_ROTATIONAL_DAMPING;
    g_player.rz2 -= g_player.rz2 * HELI_ROTATIONAL_DAMPING;

    g_player.rx1 += g_player.rx2 * time_delta;
    g_player.ry1 += g_player.ry2 * time_delta;
    g_player.rz1 += g_player.rz2 * time_delta;

    g_player.rx1 = CLAMP(g_player.rx1,-HELI_X_ROTATION_MAX,HELI_X_ROTATION_MAX);
    g_player.ry1 = CLAMP(g_player.ry1,-HELI_Y_ROTATION_MAX,HELI_Y_ROTATION_MAX);


    // Thrust acceleration
    g_player.px3 = sinf(g_player.ry1) * g_player.thrust;
    g_player.py3 = -sinf(g_player.rx1) * g_player.thrust;
    g_player.pz3 = cosf(g_player.rx1) * cosf(g_player.ry1) * g_player.thrust;


    // Linear
    g_player.px2 += g_player.px3 * time_delta;
    g_player.py2 += g_player.py3 * time_delta;
    g_player.pz2 += g_player.pz3 * time_delta;

    g_player.px2 -= g_player.px2 * HELI_LINEAR_DAMPING;
    g_player.py2 -= g_player.py2 * HELI_LINEAR_DAMPING;
    g_player.pz2 -= g_player.pz2 * HELI_LINEAR_DAMPING;

    g_player.px2 = CLAMP(g_player.px2, -1.0f, 1.0f);
    g_player.py2 = CLAMP(g_player.py2, -1.0f, 1.0f);
    g_player.pz2 = CLAMP(g_player.pz2, -1.0f, 1.0f);

    g_player.pz2 -= GRAVITY * time_delta;

    g_player.px1 += g_player.px2 * time_delta;
    g_player.py1 += g_player.py2 * time_delta;
    g_player.pz1 += g_player.pz2 * time_delta;

}


void changePlayerThrottle(int v) {

    if(v != 0) {
        g_player.throttle += (v > 0 ? 1.f:-1.f) * HELI_THROTTLE_INTERVAL;
    }
}


void rotatePlayer(int x, int y, int z) {

    if(x != 0) {
        g_player.rx3 += (x > 0 ? 1.f:-1.f) * HELI_ROT_ACCEL;
    }

    if(y != 0) {
        g_player.ry3 += (y > 0 ? 1.f:-1.f) * HELI_ROT_ACCEL;
    }

    if(z != 0) {
        g_player.rz3 += (z > 0 ? 1.f:-1.f) * HELI_ROT_ACCEL;
    }
}


void firePlayerBullets(int v) {

    if(v > 0) {
        printf("fire bullets\n");
    }
}


void firePlayerMissle() {

    printf("fire missile\n");
}


Helicopter getPlayer() {

    return g_player;
}
