//
// heligun: 3D flight sim shooter
// Copyright (c) 2016 Joseph Kuziel
//
// This software is MIT licensed.
//


#include "renderer.h"
#include "sdlext.h"
#include "glext.h"
#include "game.h"
#include "mathext.h"
#include <stdio.h>


static GLuint g_terrain_shader;

static GLint g_view_position_loc;
static GLint g_view_rotation_loc;

static GLint g_sunangle_loc;

static GLuint g_terrain_vbo;


int initRenderer() {

    // Culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    // Other
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);


    // Create OpenGL shader program
    g_terrain_shader = glextCreateShaderProgram(
          "res/terrain.vert"
        , "res/terrain.frag"
    );
    if(g_terrain_shader == 0) {
        printf("Could not create shader program\n");
        return 1;
    }

    // Geometry
    glBindAttribLocation(g_terrain_shader, 0, "a_position");
    glEnableVertexAttribArray(0);

    GLfloat terrain_vertex_data[] = {
        -1.f, 1.0f,
        1.0f, 1.0f,
        -1.f, -1.f,
        1.0f, -1.f,
    };

    g_terrain_vbo = glextCreateVBO(terrain_vertex_data, 4, 2, GL_STATIC_DRAW);
    if(g_terrain_vbo == 0) {
        return 1;
    }


    // Load textures
    GLuint diffusemap_texture = glextLoadTexture("res/map0d.png");
    GLuint normalmap_texture = glextLoadTexture("res/map0n.png");

    glextBindTextureToUniform(
          g_terrain_shader
        , diffusemap_texture
        , 0
        , "u_diffusemap"
    );
    glextBindTextureToUniform(
          g_terrain_shader
        , normalmap_texture
        , 1
        , "u_normalmap"
    );

    g_sunangle_loc = glGetUniformLocation(g_terrain_shader, "u_sunangle");
    if(g_sunangle_loc == -1) {
        printf("Could not find uniform location: u_sunangle\n");
        return 1;
    }

    g_view_position_loc = glGetUniformLocation(g_terrain_shader, "u_viewpos");
    if(g_view_position_loc == -1) {
        printf("Could not find uniform location: u_viewpos\n");
        return 1;
    }

    g_view_rotation_loc = glGetUniformLocation(g_terrain_shader, "u_viewrot");
    if(g_view_rotation_loc == -1) {
        printf("Could not find uniform location: u_viewrot\n");
        return 1;
    }


    return 0;
}


void setRendererSize(int width, int height) {

    glViewport(0, 0, width, height);

    glUseProgram(g_terrain_shader);

    GLint screensize_loc =
    glGetUniformLocation(
        g_terrain_shader
        , "u_screensize"
    );
    if(screensize_loc != -1) {
        glUniform2f(screensize_loc, (float)width, (float)height);
    } else {
        printf("Could not find uniform location: u_screensize\n");
    }
}


void render() {

    glUseProgram(g_terrain_shader);

    glUniform1f(g_sunangle_loc, getSunAngle());

    Helicopter player = getPlayer();

    float view_position_vec[] = { player.px1, player.py1, player.pz1 };
    glUniform3fv(g_view_position_loc, 1, view_position_vec);

    float view_rotation_vec[] = { player.rx1, player.ry1, player.rz1 };
    glUniform3fv(g_view_rotation_loc, 1, view_rotation_vec);

    glBindBuffer(GL_ARRAY_BUFFER, g_terrain_vbo);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
