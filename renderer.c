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


static GLuint g_shader_program;

static GLint g_view_position_loc;
static GLint g_view_rotation_loc;


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
    g_shader_program = glextCreateShaderProgram(
          "res/terrain.vert"
        , "res/terrain.frag"
    );
    if(g_shader_program == 0) {
        printf("Could not create shader program\n");
        return 1;
    }

    glUseProgram(g_shader_program);
    glBindAttribLocation(g_shader_program, 0, "a_position");

    // Geometry
    GLuint geom_buffer = 0;
    glGenBuffers(1, &geom_buffer);
    if(geom_buffer == 0) {
        printf("Could not create geometry buffer\n");
        return 1;
    }

    glBindBuffer(GL_ARRAY_BUFFER, geom_buffer);

    GLfloat vertex_data[] = {
        -1.f, 1.0f,
        1.0f, 1.0f,
        -1.f, -1.f,
        1.0f, -1.f,
    };

    glBufferData(
          GL_ARRAY_BUFFER
        , sizeof(GLfloat) * 8
        , vertex_data
        , GL_STATIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, geom_buffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(0);


    // Load textures
    GLuint diffuse_texture = glextLoadTexture("res/diffusemap.png");
    GLuint heightmap_texture = glextLoadTexture("res/heightmap.png");

    glextBindTextureToUniform(
          g_shader_program
        , diffuse_texture
        , 0
        , "u_colormap"
    );
    glextBindTextureToUniform(
          g_shader_program
        , heightmap_texture
        , 1
        , "u_heightmap"
    );

    GLint sunangle_loc = glGetUniformLocation(g_shader_program, "u_sunangle");
    if(sunangle_loc == -1) {
        printf("Could not find uniform location: u_sunangle\n");
        return 1;
    }
    glUniform1f(sunangle_loc, 0.0f);

    g_view_position_loc = glGetUniformLocation(g_shader_program, "u_viewpos");
    if(g_view_position_loc == -1) {
        printf("Could not find uniform location: u_viewpos\n");
        return 1;
    }

    g_view_rotation_loc = glGetUniformLocation(g_shader_program, "u_viewrot");
    if(g_view_rotation_loc == -1) {
        printf("Could not find uniform location: u_viewrot\n");
        return 1;
    }


    return 0;
}


void setRendererSize(int width, int height) {

    glViewport(0, 0, width, height);

    glUseProgram(g_shader_program);

    GLint screensize_loc =
    glGetUniformLocation(
        g_shader_program
        , "u_screensize"
    );
    if(screensize_loc != -1) {
        glUniform2f(screensize_loc, (float)width, (float)height);
    } else {
        printf("Could not find uniform location: u_screensize\n");
    }
}


void render() {

    Helicopter player = getPlayer();

    float view_position_vec[] = { player.px1, player.py1, player.pz1 };
    glUniform3fv(g_view_position_loc, 1, view_position_vec);

    float view_rotation_vec[] = { player.rx1, player.ry1, player.rz1 };
    glUniform3fv(g_view_rotation_loc, 1, view_rotation_vec);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
