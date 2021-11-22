#ifndef GUI_HPP
	#define GUI_HPP
	
	#define _GUI

	#define FONT_SIZE 32
	
	#define SDLERR(cond,msg) CHECKERR (cond, msg, *SDL_GetError ())
	#define TTFERR(cond,msg) CHECKERR (cond, msg, *TTF_GetError ())
	
	#define DEFMASK 0x00FF0000,0x0000FF00,0x000000FF,0xFF000000
	
	extern void setupwindow (void);
	extern void stopsdl (void);
	extern void updatewindow (void);
	extern void waitforrefresh (void);
	
	extern const char EM_GENERAL[];
#endif
