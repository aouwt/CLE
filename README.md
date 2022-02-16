# CLE
a esolang based on colors and lasers

see https://esolangs.org/wiki/cle for information on the language itself.

## compiling

run `make`

but also make sure you have the right dependencies:

 - a c++ compiler
 - `pkg-config`
 - the following ***development*** libraries: (parens are Debian packages)
   - [SDL_gifwrap](https://github.com/grimfang4/SDL_gifwrap) ([use this one for easy installation](https://github.com/aouwt/SDL_gifwrap))
     - depends on giflib (`libgif-dev`)
   - SDL2 (`libsdl2-dev`)
   - SDL2_ttf (`libsdl2-ttf-dev`)
   - x11 (`libx11-dev`)
   - opengl (`libglu1-mesa-dev`)

other than that you're good, ig

## usage

`./cle [options] <file>`

see `./cle --help` for more info

## license
when it becomes necessarry to put a license into place let me know
