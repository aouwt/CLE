#ifndef GUI_HPP
	#define GUI_HPP

	#define FONT_SIZE 32
	
	#define SDLERR(cond,msg) CHECKERR (cond, msg, *SDL_GetError ())
	#define TTFERR(cond,msg) CHECKERR (cond, msg, *TTF_GetError ())
	
	#define DEFMASK 0x00FF0000,0x0000FF00,0x000000FF,0xFF000000
	
	extern void GUI_setup (void);
	extern void GUI_tick (void);
	
	extern const char EM_GENERAL[];
	
	#ifdef _GIF
		#include <SDL2/SDL.h>
		extern SDL_Surface *WindowSurface;
	#endif
#endif
