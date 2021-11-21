#!/bin/sh
c++ cle.cpp $(pkg-config x11 gl sdl2 SDL2_ttf --libs --cflags) -Wall -ggdb -o ./cle
