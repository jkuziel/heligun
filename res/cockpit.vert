//
// heligun: 3D flight sim shooter
// Copyright (c) 2018 Joseph Kuziel
//
// This software is MIT licensed.
//

// Attributes
attribute vec4 a_position;


// Uniforms
uniform vec2 u_offset;
uniform float u_rotation;
uniform vec2 u_screensize;


// Varyings
varying vec2 v_textureCoord;


// Main
void main() {

    vec2 pos = vec2(a_position.x, a_position.y) + clamp(u_offset, -0.05, 0.05);

    float c = cos(u_rotation);
    float s = sin(u_rotation);

    vec2 rot = vec2(pos.x * c - pos.y * s , pos.x * s + pos.y * c);

    vec2 aspect = vec2(1.0, 1.0);

    if(u_screensize.x > u_screensize.y) {
        aspect = vec2(1.0, u_screensize.x / u_screensize.y);
    } else {
        aspect = vec2(u_screensize.x / u_screensize.y, 1.0);
    }

    vec2 scale = rot * 1.1 * aspect;

    v_textureCoord = vec2(
          a_position.x * 0.5 + 0.5
        ,-a_position.y * 0.5 + 0.5
    );

    gl_Position = vec4(scale.x, scale.y, 0.0, 1.0);
}
