#!/bin/sh

#
# heligun: 3D flight sim shooter
# Copyright (c) 2016 Joseph Kuziel
#
# This software is MIT licensed.
#

emcc main.c game.c glext.c renderer.c sdlext.c \
    -s USE_SDL=2 \
    -s USE_SDL_IMAGE=2 \
    -s SDL2_IMAGE_FORMATS='["png"]' \
    --preload-file res \
    -O2 \
    -o index.html
