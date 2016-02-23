//
// heligun: 3D flight sim shooter
// Copyright (c) 2016 Joseph Kuziel
//
// This software is MIT licensed.
//

#include <SDL2/SDL_opengles2.h>

/// @brief Print text description of OpenGL error from glGetError().
/// @return GL error enum. 0 if no error, !0 if error occurred

GLenum glextCheckError();


/// @brief Print the shader info log to standard output.
/// @param shader GL shader name

void glextPrintShaderInfoLog(
      GLuint shader
);


/// @brief Print the shader program info log to standard output.
/// @param program GL shader program name

void glextPrintProgramInfoLog(
      GLuint program
);


/// @brief Creates and loads a shader (vertex or fragment) from a text file.
/// @param shader_filename Filename of shader to load
/// @param shader_type Can either be GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
/// @return GL shader name

GLuint glextLoadShader(
      const char* shader_filename
    , GLenum shader_type
);


/// @brief Compiles shader and prints any compiler errors to standard output.
/// @param shader GL name of shader to compile
/// @return 0 if compilation succeeded or 1 if failed

int glextCompileShader(
      GLuint shader
);


/// @brief Links shader program and prints compiler errors to standard output.
/// @param program GL name of shader program to link
/// @return 0 on success or 1 on failure

int glextLinkProgram(
      GLuint program
);


/// @brief Loads vertex and fragment shaders, compiles, and links shader program
/// @param vertex_shader_filename Filename of vertex shader in plain text
/// @param fragment_shader_filename Filename of fragment shader in plain text
/// @return GL shader program name or 0 on failure

GLuint glextCreateShaderProgram(
      const char* vertex_shader_filename
    , const char* fragment_shader_filename
);


/// @brief Creates and loads texture from image file
/// @param filename Filename of image to load
/// @return GL name of texture

GLuint glextLoadTexture(
      const char* filename
);


/// @brief Binds texture to shader uniform parameter
/// @param program GL shader program name
/// @param texture GL texture name
/// @param unit Texture unit to bind texture to
/// @param uniform Uniform name to bind texture to
/// @return 0 on success, 1 on failure

int glextBindTextureToUniform(
      GLuint program
    , GLuint texture
    , GLuint unit
    , const char* uniform
);


/// @brief Create a Vertex Buffer Object from an array of data
/// @param vertices Pointer to flat vertex array
/// @param count Number of vertices described by the vertex array
/// @param size Number of floats per vertex
/// @param usage Type of VBO: GL_STREAM_DRAW, GL_STATIC_DRAW, or GL_DYNAMIC_DRAW
GLuint glextCreateVBO(
      GLfloat* vertices
    , GLuint count
    , GLint size
    , GLenum usage
);
