//
// heligun: 3D flight sim shooter
// Copyright (c) 2016 Joseph Kuziel
//
// This software is MIT licensed.
//


#include "sdlext.h"
#include "renderer.h"
#include "game.h"
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <SDL2/SDL.h>
//#include <SDL2/SDL_opengl.h>


// Defines
#define DEFAULT_WINDOW_WIDTH    854
#define DEFAULT_WINDOW_HEIGHT   480


// Globals
static SDL_Window* g_window = NULL;
static SDL_Renderer* g_renderer = NULL;


void doFrame(void* running) {

    SDL_Event event;

    while(SDL_PollEvent(&event) != 0) {

        switch(event.type) {

            case SDL_QUIT: {

                if(running != NULL) {
                    *((int*)running) = 0;
                }
                break;
            }

            case SDL_WINDOWEVENT_RESIZED: {

                setRendererSize(event.window.data1, event.window.data2);
                break;
            }

            case SDL_KEYUP: {

                if(event.key.repeat == 0) {
                    switch(event.key.keysym.sym) {
                        case SDLK_w: rotatePlayer( 1, 0, 0); break;
                        case SDLK_s: rotatePlayer(-1, 0, 0); break;
                        case SDLK_a: rotatePlayer( 0, 1, 0); break;
                        case SDLK_d: rotatePlayer( 0,-1, 0); break;
                        case SDLK_i: changePlayerThrottle(-1); break;
                        case SDLK_k: changePlayerThrottle( 1); break;
                        case SDLK_j: rotatePlayer( 0, 0,-1); break;
                        case SDLK_l: rotatePlayer( 0, 0, 1); break;
                        case SDLK_KP_SPACE: firePlayerBullets(-1); break;
                    }
                }

                break;
            }

            case SDL_KEYDOWN: {

                if(event.key.repeat == 0) {
                    switch(event.key.keysym.sym) {
                        case SDLK_w: rotatePlayer(-1, 0, 0); break;
                        case SDLK_s: rotatePlayer( 1, 0, 0); break;
                        case SDLK_a: rotatePlayer( 0,-1, 0); break;
                        case SDLK_d: rotatePlayer( 0, 1, 0); break;
                        case SDLK_i: changePlayerThrottle( 1); break;
                        case SDLK_k: changePlayerThrottle(-1); break;
                        case SDLK_j: rotatePlayer( 0, 0, 1); break;
                        case SDLK_l: rotatePlayer( 0, 0,-1); break;
                        case SDLK_KP_SPACE: firePlayerBullets(1); break;
                        case SDLK_LSHIFT:
                        case SDLK_RSHIFT: firePlayerMissle(); break;
                    }
                }

                break;
            }
        }
    }

    updateGame(0.016f);
    render();
    SDL_GL_SwapWindow(g_window);
}

int main() {

    // Init SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        sdlextPrintError();
        return 1;
    }

    // Set OpenGL version (2.1)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    // Create SDL window
    g_window =
        SDL_CreateWindow("heligun"
        , SDL_WINDOWPOS_UNDEFINED
        , SDL_WINDOWPOS_UNDEFINED
        , DEFAULT_WINDOW_WIDTH
        , DEFAULT_WINDOW_HEIGHT
        , SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
    );
    if(g_window == NULL) {
        sdlextPrintError();
        return 1;
    }

    // Create SDL OpenGL context
    SDL_GLContext* context = SDL_GL_CreateContext(g_window);
    if(context == NULL) {
        sdlextPrintError();
        return 1;
    }


    // Init renderer
    if(initRenderer() != 0) {
        return 1;
    }

    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(g_window, &window_width, &window_height);
    setRendererSize(window_width, window_height);

    // Game
    startGame();

    // Run
    int running = 1;

#ifdef __EMSCRIPTEN__

    emscripten_set_main_loop_arg(doFrame, NULL, -1, 1);

#else

    while(running) {

        doFrame(&running);
    }

#endif

    SDL_GL_DeleteContext(context);

    SDL_Quit();

    return 0;
}
