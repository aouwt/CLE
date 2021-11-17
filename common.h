#include <stdio.h>
#include "mfb/include/MiniFB.h"

#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define CYAN 0x00FFFF
#define YELLOW 0xFFFF00
#define MAGENTA 0xFF00FF
#define WHITE 0xFFFFFF
#define BLACK 0x000000

typedef unsigned long color;
struct beams {
	color u, d, l, r = {0};
};
struct board {
	class cell** board;
	unsigned int width;
	unsigned int height;
};
struct board board;

class cell {
	public:
		struct beams beam = {0};
		color state = 0;
		char op = '\0';

		void runcell (void);
};

struct mfb_window* window;
uint32_t* windowbuf;
