CFLAGS:=-Wall -Wextra -Wpedantic

SRC_D:=./src
GIFW_D:=./SDL_gifwrap

CLE_N:=${SRC_D}/cle
GUI_N:=${SRC_D}/gui
GIF_N:=${SRC_D}/gif
GIFW_N:=${GIFW_D}/SDL_gifwrap

CLE_O:=${CLE_N}.o
GUI_O:=${GUI_N}.o
GIF_O:=${GIF_N}.o
GIFW_O:=${GIFW_N}.o

CLE_C:=${CLE_N}.cpp
GUI_C:=${GUI_N}.cpp
GIF_C:=${GIF_N}.cpp
GIFW_C:=${GIFW_N}.c

CLE_H:=${CLE_N}.hpp
GUI_H:=${GUI_N}.hpp
GIF_H:=${GIF_N}.hpp
GIFW_H:=${GIFW_N}.h

CLE_DEP:=sdl2
GUI_DEP:=sdl2 SDL2_ttf
GIF_DEP:=sdl2 SDL2_ttf
GIFW_DEP:=sdl2

CLE_DEP_O:=$(pkg-config --libs ${CLE_DEP})
GUI_DEP_O:=$(pkg-config --libs ${GUI_DEP}) ${CLE_O}
GIFW_DEP_O:=$(pkg-config --libs ${GIFW_DEP})
GIF_DEP_O:=$(pkg-config --libs ${GIF_DEP}) ${GIFW_O} ${GIFW_DEP_O}

CLE_DEP_FLAG:=$(pkg-config --cflags ${CLE_DEP})
GUI_DEP_FLAG:=$(pkg-config --cflags ${GUI_DEP})
GIF_DEP_FLAG:=$(pkg-config --cflags ${GIF_DEP})
GIFW_DEP_FLAG:=$(pkg-config --cflags ${GIFW_DEP})


CLE:=${CLE_O} ${CLE_DEP_O} ${CLE_DEP_FLAG}
GUI:=${GUI_O} ${GUI_DEP_O} ${GUI_DEP_FLAG}
GIF:=${GIF_O} ${GIF_DEP_O} ${GIF_DEP_FLAG}
GIFW:=${GIFW_O} ${GIFW_DEP_O} ${GIFW_DEP_FLAG}


all: ${CLE_O} ${GUI_O} ${GIF_O}
	c++ ${CFLAGS} ${CLE} ${GIF} ${GUI} -o ./cle

${CLE_O}:
	c++ ${CFLAGS} ${CLE_DEP_FLAG} -c ${CLE_C} -o ${CLE_O}

${GUI_O}: ${CLE_O}
	c++ ${CFLAGS} ${GUI_DEP_FLAG} -c ${GUI_C} -o ${GUI_O}

${GIF_O}: ${CLE_O} ${GUI_O} ${GIFW_O}
	cc ${CFLAGS} ${GIF_DEP_FLAG} -c ${GIF_C} -o ${GIF_O}

${GIFW_O}:
	cc ${CFLAGS} ${GIFW_DEP_FLAG} -c ${GIFW_C} -o ${GIFW_O}
