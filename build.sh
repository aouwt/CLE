#!/bin/sh
c++ $@ $(pkg-config --libs --cflags sdl2 SDL2_ttf) ./src/gui.cpp ./src/cle.cpp -o ./cle
