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


void main() {

    vec2 texcoord = vec2(
           gl_PointCoord.x * 0.45 + 0.5 + clamp(u_offset.x, -0.05, 0.05)
        , -gl_PointCoord.y * 0.45 + 0.5 + clamp(u_offset.y, -0.05, 0.05)
    );

    gl_FragColor = texture2D(u_diffusemap, texcoord);
}
