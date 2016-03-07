//
// heligun: 3D flight sim shooter
// Copyright (c) 2016 Joseph Kuziel
//
// This software is MIT licensed.
//

precision mediump float;


// Uniforms
uniform sampler2D u_diffusemap;


// Varyings
varying vec2 v_textureCoord;


// Main
void main() {

    gl_FragColor = texture2D(u_diffusemap, v_textureCoord);
}
