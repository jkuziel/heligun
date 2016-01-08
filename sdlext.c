//
// heligun: 3D flight sim shooter
// Copyright (c) 2016 Joseph Kuziel
//
// This software is MIT licensed.
//


#include "sdlext.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_rwops.h>
#include <SDL2/SDL_error.h>


void sdlextPrintError() {

    printf("[SDL] %s\n", SDL_GetError());
    SDL_ClearError();
}


char* sdlextReadTextFile(
      const char* filename
) {

    if(filename == NULL) {
        return NULL;
    }

    SDL_RWops* file = SDL_RWFromFile(filename, "r");
    if(file == NULL) {
        sdlextPrintError();
        return NULL;
    }

    Sint64 file_size = SDL_RWsize(file);
    if(file_size < 0) {
        sdlextPrintError();
        return 0;
    }

    char* file_buffer = (char*)malloc(file_size + 1);

    size_t bytes_read = 0;
    size_t total_bytes_read = 0;

    char* bufferptr = file_buffer;
    do {
        bytes_read =
        SDL_RWread(
              file
            , bufferptr
            , 1
            , (file_size - total_bytes_read)
        );

        total_bytes_read += bytes_read;
        bufferptr += bytes_read;

    } while(bytes_read > 0);


    SDL_RWclose(file);

    if(total_bytes_read != file_size) {
        free(file_buffer);
        return NULL;
    }


    file_buffer[total_bytes_read] = '\0';

    return file_buffer;
}
