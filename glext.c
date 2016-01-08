//
// heligun: 3D flight sim shooter
// Copyright (c) 2016 Joseph Kuziel
//
// This software is MIT licensed.
//


#include "glext.h"
#include "sdlext.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>


void glextGetPixelFormat(
      Uint32 sdl_format
    , GLenum* gl_format
    , GLenum* gl_type
) {

    if(gl_format == NULL || gl_type == NULL) {
        return;
    }

    switch(sdl_format) {
        case SDL_PIXELFORMAT_RGB24:
            *gl_format = GL_RGB;
            *gl_type = GL_UNSIGNED_BYTE;
            break;

        case SDL_PIXELFORMAT_RGBA8888:
            *gl_format = GL_RGBA;
            *gl_type = GL_UNSIGNED_BYTE;
            break;
    }
}


GLenum glextCheckError() {

    GLenum gl_error = glGetError();

    switch(gl_error) {

        case GL_INVALID_ENUM:
            printf("[GL] Invalid value\n");
            break;

        case GL_INVALID_VALUE:
            printf("[GL] Numeric argument out of range\n");
            break;

        case GL_INVALID_OPERATION:
            printf("[GL] Not allowed in current state\n");
            break;

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            printf("[GL] Invalid framebuffer operation\n");
            break;

        case GL_OUT_OF_MEMORY:
            printf("[GL] Out of memory\n");
            break;

        // case GL_STACK_UNDERFLOW:
        //     printf("[GL] Stack underflow\n");
        //     break;
        //
        // case GL_STACK_OVERFLOW:
        //     printf("[GL] Stack overflow\n");
        //     break;
    }

    return gl_error;
}


void glextPrintShaderInfoLog(
      GLuint shader
) {

    GLint log_length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

    if(log_length > 0) {

        char* log = (char*)malloc(log_length);

        GLint actual_log_length = 0;
        glGetShaderInfoLog(shader, log_length, &actual_log_length, log);

        if(actual_log_length > 0) {
            printf("%s\n", log);
        }

        free(log);
    }
}


void glextPrintProgramInfoLog(
      GLuint program
) {

    GLint log_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

    if(log_length > 0) {

        char* log = (char*)malloc(log_length);

        GLint actual_log_length = 0;
        glGetProgramInfoLog(program, log_length, &actual_log_length, log);

        if(actual_log_length > 0) {
            printf("%s\n", log);
        }

        free(log);
    }
}


GLuint glextLoadShader(
      const char* shader_filename
    , GLenum shader_type
) {

    if(shader_filename == NULL) {
        printf("No shader file name specified\n");
        return 0;
    }

    GLchar* shader_src = sdlextReadTextFile(shader_filename);
    if(shader_src == NULL) {
        printf("Could not load shader file: %s\n", shader_filename);
        return 0;
    }

    GLuint shader = glCreateShader(shader_type);

    if(shader == 0) {
        glextCheckError();
        return 0;
    }

    GLchar* shader_lines[] = { shader_src };

    glShaderSource(shader, 1, (const GLchar**)shader_lines, NULL);

    free(shader_src);

    return shader;
}


int glextCompileShader(
      GLuint shader
) {
    glCompileShader(shader);

    GLint compile_status = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if(compile_status != GL_TRUE) {
        glextPrintShaderInfoLog(shader);
        return 1;
    }

    return 0;
}


int glextLinkProgram(
      GLuint program
) {

    glLinkProgram(program);

    GLint link_status = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if(link_status != GL_TRUE) {
        glextPrintProgramInfoLog(program);
        return 1;
    }

    return 0;
}


GLuint glextCreateShaderProgram(
      const char* vertex_shader_filename
    , const char* fragment_shader_filename
) {

    // Load shaders
    GLuint vertex_shader =
        glextLoadShader(vertex_shader_filename, GL_VERTEX_SHADER);

    if(vertex_shader == 0) {
        return 0;
    }

    GLuint fragment_shader =
        glextLoadShader(fragment_shader_filename, GL_FRAGMENT_SHADER);

    if(fragment_shader == 0) {
        return 0;
    }

    // Compile shaders
    if(glextCompileShader(vertex_shader) != 0) {
        printf("Failed to compile shader: %s\n", vertex_shader_filename);
        return 0;
    }

    if(glextCompileShader(fragment_shader) != 0) {
        printf("Failed to compile shader: %s\n", fragment_shader_filename);
        return 0;
    }

    // Create program
    GLuint program = glCreateProgram();

    if(program == 0) {
        glextCheckError();
        return 0;
    }

    // Attach shaders
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    // Link program
    if(glextLinkProgram(program) != 0) {
        return 0;
    }

    return program;
}


GLuint glextLoadTexture(
      const char* filename
) {
    if(filename == NULL) {
        printf("Texture filename not specified\n");
        return 0;
    }

    SDL_Surface* surface = IMG_Load(filename);
    if(surface == NULL) {
        printf("Could not load texture \"%s\": %s\n", filename, IMG_GetError());
        return 0;
    }

    GLuint texture = 0;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum pixel_format = GL_RGBA;
    GLenum pixel_type = GL_UNSIGNED_BYTE;
    glextGetPixelFormat(surface->format->format, &pixel_format, &pixel_type);

    SDL_LockSurface(surface);
    glTexImage2D(
          GL_TEXTURE_2D
        , 0
        , pixel_format
        , surface->w
        , surface->h
        , 0
        , pixel_format
        , pixel_type
        , surface->pixels
    );
    SDL_UnlockSurface(surface);

    SDL_FreeSurface(surface);

    return texture;
}


int glextBindTextureToUniform(
      GLuint program
    , GLuint texture
    , GLuint unit
    , const char* uniform
) {

    GLint uniform_loc = glGetUniformLocation(program, uniform);
    if(uniform_loc == -1) {
        printf("Could not find uniform location: %s\n", uniform);
        return 1;
    }

    glUniform1i(uniform_loc, unit);

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture);

    return 0;
}
