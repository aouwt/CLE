#!/bin/sh
echo "Building CLE..."
c++ cle.cpp $(pkg-config x11 gl sdl2 SDL2_ttf --libs --cflags) -Wall -ggdb -o ./cle
echo "Done."
