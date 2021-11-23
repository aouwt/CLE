#!/bin/sh
c++ $@ ./src/cle.cpp ./src/gui.cpp $(pkg-config --libs --cflags sdl2 SDL2_ttf) -o ./cle
