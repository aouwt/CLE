#!/bin/sh
echo "Building CLE..."
c++ cle.cpp $(pkg-config x11 gl sdl2 --libs --cflags) -Wall -g -o ./cle
echo "Done."
