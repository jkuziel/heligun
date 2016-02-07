//
// heligun: 3D flight sim shooter
// Copyright (c) 2016 Joseph Kuziel
//
// This software is MIT licensed.
//

// Structures

typedef struct {

    float px1;
    float py1;
    float pz1;

    float px2;
    float py2;
    float pz2;

    float px3;
    float py3;
    float pz3;

    float rx1;
    float ry1;
    float rz1;

    float rx2;
    float ry2;
    float rz2;

    float rx3;
    float ry3;
    float rz3;

    float thrust;
    float throttle;

} Helicopter;


// Functions

void startGame();


void updateGame(float time_delta);


void changePlayerThrottle(int v);


void rotatePlayer(int x, int y, int z);


void firePlayerBullets(int v);


void firePlayerMissle();


Helicopter getPlayer();


float getSunAngle();
