#
# heligun: 3D flight sim shooter
# Copyright (c) 2016 Joseph Kuziel
#
# This software is MIT licensed.
#

IDIR =.
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj

LIBS=-framework OpenGL -lm -ldl -lSDL2_Image

_DEPS = mathext.h game.h glext.h renderer.h sdlext.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o game.o glext.o renderer.o sdlext.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: heligun

$(ODIR):
	mkdir $@

$(ODIR)/%.o: %.c $(DEPS) | $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS) `sdl2-config --cflags`

heligun: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS) `sdl2-config --libs`


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~
