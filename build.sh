#!/bin/sh
c++ $@ ./src/gui.cpp ./src/cle.cpp -Wall $(pkg-config --libs --cflags sdl2 SDL2_ttf gl x11) -o ./cle
