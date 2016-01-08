//
// heligun: 3D flight sim shooter
// Copyright (c) 2016 Joseph Kuziel
//
// This software is MIT licensed.
//

precision mediump float;

//// Uniforms ////

uniform sampler2D u_heightmap;
uniform sampler2D u_colormap;

uniform vec2 u_screensize;

uniform float u_sunangle;

uniform vec3 u_viewpos;
uniform vec3 u_viewrot;


//// Constants ////

const float MAX_RAY_DIST = 3.0;
const float RAY_RESOLUTION = 0.01;

const float TERRAIN_SCALE = 0.25;

const vec4 NOON_COLOR = vec4(0.5, 0.75, 0.9, 1.0);
const vec4 DUSK_COLOR = vec4(0.8, 0.4, 0.1, 1.0);
const vec4 NIGHT_COLOR = vec4(0.18, 0.18, 0.18, 1.0);
const vec4 SHADOW_COLOR = vec4(0.1, 0.1, 0.1, 1.0);

const float FOG_DIST = MAX_RAY_DIST * 0.5;


//// Math ////

float map(float x, float x1, float x2, float y1, float y2) {

    return clamp(y1 + (x - x1) * (y2 - y1) / (x2 - x1), y1, y2);
}


//// Colors ////

float brightness(vec4 color) {

    return max(color.x, max(color.y, color.z));
}

vec4 skyColor(vec3 rdir, float angle) {

    angle = abs(angle);

    float zenith = map(1.0 - rdir.z, 0.0, 1.0, 0.8, 1.0);

    if(angle < 0.9) {

        return NOON_COLOR * zenith;

    } else if(angle < 1.5) {

        return mix(NOON_COLOR, DUSK_COLOR, map(angle, 0.9, 1.5, 0.0, 1.0)) * zenith;

    } else if(angle < 1.6) {

        return mix(DUSK_COLOR, NIGHT_COLOR, map(angle, 1.5, 1.6, 0.0, 1.0)) * zenith;

    } else {

        return NIGHT_COLOR * zenith;
    }
}


//// Vectors ////

vec3 rotateX(vec3 v, float t) {

    float c = cos(t);
    float s = sin(t);

    return vec3(v.x,
                v.y * c - v.z * s,
                v.y * s + v.z * c);
}

vec3 rotateY(vec3 v, float t) {

    float c = cos(t);
    float s = sin(t);

    return vec3(v.x * c + v.z * s,
                v.y,
                -v.x * s + v.z * c);
}

vec3 rotateZ(vec3 v, float t) {

    float c = cos(t);
    float s = sin(t);

    return vec3(v.x * c - v.y * s,
                v.x * s + v.y * c,
                v.z);
}


//// Terrain ////

float terrainHeight(vec2 pos) {

    return texture2D(u_heightmap, pos * TERRAIN_SCALE).x * TERRAIN_SCALE;
}

vec4 terrainColor(vec2 pos) {

    return texture2D(u_colormap, pos * TERRAIN_SCALE);
}

vec3 raycastTerrain(vec3 rpos, vec3 rdir) {

    for(float t = RAY_RESOLUTION; t < MAX_RAY_DIST; t += RAY_RESOLUTION) {

        vec3 p = rpos + rdir * t;

        float height = terrainHeight(p.xy);

        if(height >= p.z) {

            return vec3(p.xy, height);
        }
    }

    return vec3(0.0, 0.0, -1.0);
}

vec4 rayColor(vec3 rpos, vec3 rdir) {

    vec3 terrainPos = raycastTerrain(rpos, rdir);
    float dist = length(rpos - terrainPos);

    if(terrainPos.z < 0.0) {

        return skyColor(rdir, u_sunangle);
    }

    vec4 terrainColor = terrainColor(terrainPos.xy);

    // shadow
    if(u_sunangle > 0.2) {

        float shadow = 0.0;
        vec3 occPos = raycastTerrain(terrainPos, vec3(sin(u_sunangle), 0.0, cos(u_sunangle)));
        if(occPos.z > 0.0) {
            shadow = (1.0 - length(terrainPos - occPos) / MAX_RAY_DIST) * 0.5;
        }

        terrainColor = mix(terrainColor, SHADOW_COLOR, shadow);
    }

    // fog
    float fog = 0.0;
    if(dist > FOG_DIST) {
        fog = map(dist, FOG_DIST, MAX_RAY_DIST, 0.0, 1.0);
    }

    return mix(terrainColor, skyColor(rdir, u_sunangle), fog);
}

void main()
{
	vec2 uv = (gl_FragCoord.xy / u_screensize.xy) * 2.0 - vec2(1.0, 1.0);

    vec3 rpos = u_viewpos;
    vec3 rdir = vec3(uv.x, 1.0, uv.y);
    rdir = rotateX(rdir, u_viewrot.x);
    rdir = rotateY(rdir, u_viewrot.y);
    rdir = rotateZ(rdir, u_viewrot.z);

	gl_FragColor = rayColor(rpos, rdir);
}
