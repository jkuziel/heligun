//
// heligun: 3D flight sim shooter
// Copyright (c) 2018 Joseph Kuziel
//
// This software is MIT licensed.
//


// Uniforms
uniform sampler2D u_diffusemap;
uniform float u_brightness;


// Varyings
varying vec2 v_textureCoord;


// Main
void main() {

    gl_FragColor = texture2D(u_diffusemap, v_textureCoord)
        * vec4(u_brightness, u_brightness, u_brightness, 1.0);
}
