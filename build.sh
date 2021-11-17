#!/bin/sh

echo "Building MiniFB..."
cd mfb
cmake .
make minifb
cd ..
echo "Building CLE..."
c++ cle.cpp -lGL -lX11 -L./mfb -lminifb $(pkg-config x11 gl --libs --cflags) -Wall -g
echo "Done."
