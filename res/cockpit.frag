//
// heligun: 3D flight sim shooter
// Copyright (c) 2016 Joseph Kuziel
//
// This software is MIT licensed.
//

precision mediump float;

// Uniforms
uniform sampler2D u_diffusemap;


void main() {

    vec2 texcoord = vec2(
           gl_PointCoord.x * 0.5 + 0.5
        , -gl_PointCoord.y * 0.5 + 0.5
    );

    gl_FragColor = texture2D(u_diffusemap, texcoord);
}