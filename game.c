//
// heligun: 3D flight sim shooter
// Copyright (c) 2018 Joseph Kuziel
//
// This software is MIT licensed.
//


#include "game.h"
#include "mathext.h"
#include <stdio.h>
#include <string.h>

#define BULLET_COUNT 50

// Globals

static Helicopter g_player;
static Bullet g_bullet_projectiles[BULLET_COUNT];

int g_bullets;
float g_sunAngle;


// Constants
static const float HELI_X_LINEAR_WRAP                   = 4.0f;
static const float HELI_Y_LINEAR_WRAP                   = 4.0f;
static const float HELI_Z_LINEAR_MAX                    = 1.2f;

static const float HELI_LINEAR_DAMPING                  = 0.05f;

static const float HELI_X_LINEAR_ACCEL                  = 4.0f;
static const float HELI_Y_LINEAR_ACCEL                  = 3.0f;
static const float HELI_Z_LINEAR_ACCEL                  = 2.0f;

static const float HELI_X_ROTATION_DAMPING              = 0.5f;
static const float HELI_Y_ROTATION_DAMPING              = 0.5f;

static const float HELI_X_ROTATION_MAX                  = 0.7f;
static const float HELI_Y_ROTATION_MAX                  = 0.7f;

static const float HELI_ROTATIONAL_DAMPING              = 0.05f;

static const float HELI_X_ROTATIONAL_ACCEL              = 5.0f;
static const float HELI_Y_ROTATIONAL_ACCEL              = 4.0f;
static const float HELI_Z_ROTATIONAL_ACCEL              = 6.0f;

static const float SUN_ROTATIONAL_SPEED                 = 0.02f;

static const int BULLET_DAMAGE                          = 5;
static const float BULLET_VELOCITY[3]                   = {0, -.05, .05};

// Public Functions

void startGame() {

    g_player.px1 = 0.0f;
    g_player.py1 = 0.0f;
    g_player.pz1 = 1.2f;
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

    g_sunAngle = 0.0f;
    g_bullets = 0;

    for(int i = 0; i < BULLET_COUNT; i++) {
       float empty_array[3] = { 0.0, 0.0, 0.0 };
       memcpy(g_bullet_projectiles[i].position, empty_array, sizeof(g_bullet_projectiles[i].position) );
       memcpy(g_bullet_projectiles[i].velocity, empty_array, sizeof(g_bullet_projectiles[i].velocity) );
       g_bullet_projectiles[i].active = false;
       g_bullet_projectiles[i].damage = BULLET_DAMAGE;
    }
}


void updateGame(float time_delta) {

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

    if(fabs(g_player.rx1) > HELI_X_ROTATION_DAMPING
    && (g_player.rx1 * g_player.rx2) > 0.0f) {
        g_player.rx1 -=
        SCALE(
              fabs(g_player.rx1)
            , HELI_X_ROTATION_DAMPING
            , HELI_X_ROTATION_MAX
            , 0.0f
            , g_player.rx2
        ) * time_delta;
    }

    if(fabs(g_player.ry1) > HELI_Y_ROTATION_DAMPING
    && (g_player.ry1 * g_player.ry2) > 0.0f) {
        g_player.ry1 -=
        SCALE(
              fabs(g_player.ry1)
            , HELI_Y_ROTATION_DAMPING
            , HELI_Y_ROTATION_MAX
            , 0.0f
            , g_player.ry2
        ) * time_delta;
    }

    g_player.rx1 = CLAMP(g_player.rx1,-HELI_X_ROTATION_MAX,HELI_X_ROTATION_MAX);
    g_player.ry1 = CLAMP(g_player.ry1,-HELI_Y_ROTATION_MAX,HELI_Y_ROTATION_MAX);
    g_player.rz1 = WRAPF(g_player.rz1, 0.0f, M_PI_2X);

    // Linear
    g_player.px3 =
          sinf(g_player.rx1) * sinf(g_player.rz1) * HELI_Y_LINEAR_ACCEL
        + sinf(g_player.ry1) * cosf(g_player.rz1) * HELI_X_LINEAR_ACCEL;
    g_player.py3 =
        - sinf(g_player.rx1) * cosf(g_player.rz1) * HELI_Y_LINEAR_ACCEL
        + sinf(g_player.ry1) * sinf(g_player.rz1) * HELI_X_LINEAR_ACCEL;

    g_player.px2 += g_player.px3 * time_delta;
    g_player.py2 += g_player.py3 * time_delta;
    g_player.pz2 += g_player.pz3 * time_delta;

    g_player.px2 -= g_player.px2 * HELI_LINEAR_DAMPING;
    g_player.py2 -= g_player.py2 * HELI_LINEAR_DAMPING;
    g_player.pz2 -= g_player.pz2 * HELI_LINEAR_DAMPING;

    g_player.px1 += g_player.px2 * time_delta;
    g_player.py1 += g_player.py2 * time_delta;
    g_player.pz1 += g_player.pz2 * time_delta;

    g_player.px1 = WRAPF(g_player.px1, 0.0f, HELI_X_LINEAR_WRAP);
    g_player.py1 = WRAPF(g_player.py1, 0.0f, HELI_Y_LINEAR_WRAP);
    g_player.pz1 = CLAMP(g_player.pz1, 0.0f, HELI_Z_LINEAR_MAX);

    g_sunAngle =WRAPF(g_sunAngle+SUN_ROTATIONAL_SPEED*time_delta, 0.0f,M_PI_2X);
    // TODO: Add rendering of g_bullet_projectiles here
}


void changePlayerThrottle(int v) {

    if(v != 0) {
        g_player.pz3 += (v > 0 ? 1.f : -1.f) * HELI_Z_LINEAR_ACCEL;
    }
}


void rotatePlayer(int x, int y, int z) {

    if(x != 0) {
        g_player.rx3 += (x > 0 ? 1.f:-1.f) * HELI_X_ROTATIONAL_ACCEL;
    }

    if(y != 0) {
        g_player.ry3 += (y > 0 ? 1.f:-1.f) * HELI_Y_ROTATIONAL_ACCEL;
    }

    if(z != 0) {
        g_player.rz3 += (z > 0 ? 1.f:-1.f) * HELI_Z_ROTATIONAL_ACCEL;
    }
}


void firePlayerBullets(int v) {

    if(v > 0) {
        printf("fire bullets\n");
        addProjectile(-1);
    }
}


void firePlayerMissle() {

    printf("fire missile\n");
}

void addProjectile(int type) {
    if(type == -1) {  // TODO: Replace these ugly ints with an enum
        if(g_bullets < BULLET_COUNT) {
           g_bullets++;
        } else {
           g_bullets = 0;
        }
       g_bullet_projectiles[g_bullets].active = true;
       memcpy(g_bullet_projectiles[g_bullets].velocity, BULLET_VELOCITY, sizeof(g_bullet_projectiles[g_bullets].velocity) );
    }
}

Helicopter getPlayer() {

    return g_player;
}


float getSunAngle() {

    return g_sunAngle;
}
