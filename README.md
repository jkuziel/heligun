# heligun
*3D helicopter shooter with raytraced terrain for Emscripten*

Copyright (c) 2018 Joseph Kuziel


#### Description

Heligun is a game based on the Comanche series by NovaLogic. The player takes control of an attack helicopter to complete missions and battle with other attack helicopters.

The terrain is generated using a similar technique found in Comanche where a ray is cast from the viewpoint to a heightmap as an alternative to vectorized graphics which were computationally expensive in 1992 (http://simulationcorner.net/index.php?page=comanche). Heligun uses raymarching to draw the terrain which is run on the GPU in parallel for each pixel.


#### Controls

|Key|Command|
|---|---|
|w|Pitch forward|
|s|Pitch back|
|a|Roll left|
|d|Roll right|
|i|Increase throttle|
|k|Decrease throttle|
|j|Yaw left|
|l|Yaw right|


#### License

MIT

See *LICENSE* for more information


#### Languages

* C
* GLSL


#### Dependencies

* [SDL2](http://www.libsdl.org)
* [OpenGL 2.0](http://docs.gl)
* [OpenGL ES 2.0](http://docs.gl)
* [Emscripten](http://www.emscripten.org) (optional)


#### Supported platforms

* Web (Javascript / WebGL)
* Native
    * Windows
    * Mac
    * Linux


#### Building

* Emscripten
    * Add Emscripten SDK to your PATH (Bash: `source ./<emscripten sdk>/emsdk_env.sh`)
    * Run `./build_emscripten.sh`
    * *index.html*, *index.html.mem*, *index.data*, *index.js* should be generated
    * Run `python -m SimpleHTTPServer 8080` from heligun directory
    * Open `http://localhost:8080` in your web browser
