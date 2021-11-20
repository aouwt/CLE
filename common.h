#include <stdio.h>
#include <errno.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <fontconfig/fontconfig.h>

#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define CYAN 0x00FFFF
#define YELLOW 0xFFFF00
#define MAGENTA 0xFF00FF
#define WHITE 0xFFFFFF
#define BLACK 0x000000

#define FONT_SIZE 36

#define CHECKERR(cond,msg,code) if (cond) { fprintf (stderr, "ERROR: %s (E%i,L%i)\n", (msg), (code), __LINE__); exit (1); }
#define SDLERR(cond,msg) CHECKERR (cond, msg, *SDL_GetError ())
#define TTFERR(cond,msg) CHECKERR (cond, msg, *TTF_GetError ())
#define THISERR(cond,msg,code) if (cond) { fprintf (stderr, "ERROR: %s (L%i)\n", (msg), __LINE__); exit(code); }
#define FCERR(thing,msg) THISERR (!(thing), (msg), 1)

const char EM_GENERAL[] = "Unknown error";
const char EM_CREATESURFACE[] = "Could not create surface";

typedef unsigned long color;

struct beams {
	color u, d, l, r = {0};
};

struct board {
	class cell** board;
	unsigned int width;
	unsigned int height;
};

class cell {
	public:
		struct beams beam = {0};
		color state = 0;
		char op = '\0';

		void runcell (void);
};

SDL_Window* Window;

SDL_Surface* WindowSurface;
SDL_Surface* BeamsSurface;
SDL_Surface* TextSurface;

SDL_Rect BeamsStretch;

SDL_Event Event;

//SDL_Renderer* Renderer;

TTF_Font* Font;

struct board Board;
bool UpdateTextSurface = true;
