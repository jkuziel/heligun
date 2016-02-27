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


static GLuint g_terrain_diffuse_texture;
static GLuint g_terrain_normal_texture;
static GLuint g_cockpit_diffuse_texture;

static GLuint g_terrain_shader;
static GLuint g_cockpit_shader;

static GLint g_terrain_position_loc;
static GLint g_terrain_rotation_loc;
static GLint g_cockpit_offset_loc;
static GLint g_cockpit_rotation_loc;

static GLint g_sunangle_loc;

static GLuint g_terrain_vbo;


int initRenderer() {

    // Culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    // Depth
    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);

    // Blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Other
    glDisable(GL_STENCIL_TEST);

    // Create OpenGL shader program
    g_terrain_shader = glextCreateShaderProgram(
          "res/terrain.vert"
        , "res/terrain.frag"
    );
    if(g_terrain_shader == 0) {
        printf("Could not create terrain shader program\n");
        return 1;
    }

    g_cockpit_shader = glextCreateShaderProgram(
          "res/cockpit.vert"
        , "res/cockpit.frag"
    );
    if(g_cockpit_shader == 0) {
        printf("Could not create cockpit shader program\n");
        return 1;
    }

    // Geometry
    glBindAttribLocation(g_terrain_shader, 0, "a_position");
    glBindAttribLocation(g_cockpit_shader, 0, "a_position");

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
    g_terrain_diffuse_texture = glextLoadTexture("res/map0d.png", 1, 0);
    g_terrain_normal_texture = glextLoadTexture("res/map0n.png", 1, 0);
    g_cockpit_diffuse_texture = glextLoadTexture("res/helicockpit.png", 0, 1);
    if(g_cockpit_diffuse_texture == 0) {
        return 1;
    }

    glextBindTextureToUniform(
          g_terrain_shader
        , g_terrain_diffuse_texture
        , 0
        , "u_diffusemap"
    );
    glextBindTextureToUniform(
          g_terrain_shader
        , g_terrain_normal_texture
        , 1
        , "u_normalmap"
    );
    if( glextBindTextureToUniform(
          g_cockpit_shader
        , g_cockpit_diffuse_texture
        , 0
        , "u_diffusemap"
    ) == 1) {
            return 1;
    }

    // Program attribute locations
    g_sunangle_loc = glGetUniformLocation(g_terrain_shader, "u_sunangle");
    if(g_sunangle_loc == -1) {
        printf("Could not find uniform location: u_sunangle\n");
        return 1;
    }

    g_terrain_position_loc = glGetUniformLocation(g_terrain_shader,"u_viewpos");
    if(g_terrain_position_loc == -1) {
        printf("Could not find uniform location: u_viewpos\n");
        return 1;
    }

    g_terrain_rotation_loc = glGetUniformLocation(g_terrain_shader,"u_viewrot");
    if(g_terrain_rotation_loc == -1) {
        printf("Could not find uniform location: u_viewrot\n");
        return 1;
    }

    g_cockpit_offset_loc = glGetUniformLocation(g_cockpit_shader, "u_offset");
    if(g_cockpit_offset_loc == -1) {
        printf("Could not find uniform location: u_offset");
        return 1;
    }

    g_cockpit_rotation_loc =glGetUniformLocation(g_cockpit_shader,"u_rotation");
    if(g_cockpit_rotation_loc == -1) {
        printf("Could not find uniform location: u_rotation");
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

    glEnableVertexAttribArray(0);

    // Render terrain //////////////////
    glUseProgram(g_terrain_shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_terrain_diffuse_texture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_terrain_normal_texture);

    glUniform1f(g_sunangle_loc, getSunAngle());

    Helicopter player = getPlayer();

    float terrain_position_vec[] = { player.px1, player.py1, player.pz1 };
    glUniform3fv(g_terrain_position_loc, 1, terrain_position_vec);

    float terrain_rotation_vec[] = { player.rx1, player.ry1, player.rz1 };
    glUniform3fv(g_terrain_rotation_loc, 1, terrain_rotation_vec);

    glBindBuffer(GL_ARRAY_BUFFER, g_terrain_vbo);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Render cockpit //////////////////
    glUseProgram(g_cockpit_shader);

    float cockpit_offset_vec[] = { -player.rz2 * 0.05f, -player.rx2 * 0.05f };
    glUniform2fv(g_cockpit_offset_loc, 1, cockpit_offset_vec);

    glUniform1f(g_cockpit_rotation_loc, -player.ry2 * 0.05f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_cockpit_diffuse_texture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(0);
}
