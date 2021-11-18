#!/bin/sh

echo "Building MiniFB..."
cd mfb
cmake .
make minifb
cd ..
echo "Building CLE..."
c++ cle.cpp $(pkg-config x11 gl sdl2 --libs --cflags) -Wall -g -o ./cle
echo "Done."
