//
// heligun: 3D flight sim shooter
// Copyright (c) 2016 Joseph Kuziel
//
// This software is MIT licensed.
//

precision mediump float;

// Uniforms
uniform sampler2D u_diffusemap;

uniform vec2 u_offset;
uniform float u_rotation;


// Main
void main() {

    vec2 pos = vec2(
           gl_PointCoord.x + clamp(u_offset.x, -0.05, 0.05)
        , -gl_PointCoord.y + clamp(u_offset.y, -0.05, 0.05)
    );

    float c = cos(u_rotation);
    float s = sin(u_rotation);

    vec2 rot = vec2(pos.x * c - pos.y * s , pos.x * s + pos.y * c);

    vec2 scale = rot * 0.95;

    vec2 uv = scale * 0.5 + 0.5;

    gl_FragColor = texture2D(u_diffusemap, uv);
}
