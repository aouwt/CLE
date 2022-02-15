CFLAGS:=-Wall -Wextra -Wpedantic

SRC_D:=./src

CLE_N:=${SRC_D}/cle
GUI_N:=${SRC_D}/gui
GIF_N:=${SRC_D}/gif

CLE_O:=${CLE_N}.o
GUI_O:=${GUI_N}.o
GIF_O:=${GIF_N}.o

CLE_C:=${CLE_N}.cpp
GUI_C:=${GUI_N}.cpp
GIF_C:=${GIF_N}.cpp

CLE_H:=${CLE_N}.hpp
GUI_H:=${GUI_N}.hpp
GIF_H:=${GIF_N}.hpp

CLE_DEP:=sdl2
GUI_DEP:=sdl2 SDL2_ttf
GIF_DEP:=sdl2 SDL2_ttf

CLE_DEP_LD:=$(shell pkg-config --libs ${CLE_DEP})
GUI_DEP_LD:=$(shell pkg-config --libs ${GUI_DEP})
GIF_DEP_LD:=$(shell pkg-config --libs ${GIF_DEP}) -lSDL_gifwrap -lgif

CLE_DEP_FLAG:=$(shell pkg-config --cflags ${CLE_DEP})
GUI_DEP_FLAG:=$(shell pkg-config --cflags ${GUI_DEP})
GIF_DEP_FLAG:=$(shell pkg-config --cflags ${GIF_DEP})


CLE:=${CLE_O} ${CLE_DEP_LD} ${CLE_DEP_FLAG}
GUI:=${GUI_O} ${GUI_DEP_LD} ${GUI_DEP_FLAG}
GIF:=${GIF_O} ${GIF_DEP_LD} ${GIF_DEP_FLAG}


all: ${GUI_O} ${GIF_O} ${CLE_O}
	c++ ${CFLAGS} ${CLE} ${GIF} ${GUI} -o ./cle

clean:
	rm ./src/*.o

@PHONY: all clean
	
${CLE_O}:
	c++ $(if wildcard(${GUI_O}),-D_GUI) $(if wildcard(${GIF_O}),-D_GIF) ${CFLAGS} ${CLE_DEP_FLAG} -c ${CLE_C} -o ${CLE_O}
	

${GUI_O}: ${CLE_O}
	c++ -D_GUI ${CFLAGS} ${GUI_DEP_FLAG} -c ${GUI_C} -o ${GUI_O}

${GIF_O}: ${CLE_O} ${GUI_O}
	c++ -D_GIF -D_GUI ${CFLAGS} ${GIF_DEP_FLAG} -c ${GIF_C} -o ${GIF_O}

