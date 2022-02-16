#include <SDL2/SDL.h>
#include <SDL2/SDL_gifwrap.h>
#include "gui.hpp"
#include "cle.hpp"

GIF_Image *gif = NULL;


void GIF_setup (void) {
	gif = GIF_CreateImage ();
	GIF_SetCanvasSize (gif, WindowSurface -> w, WindowSurface -> h);
	GIF_SetLooping (gif, 0);
	GIF_SetGlobalPalette (gif, GIF_CreateBasicPalette ());
}

void GIF_tick (void) {
	GIF_Frame *frame = GIF_CreateFrame (WindowSurface, SDL_FALSE);
	GIF_SetDelay (frame, 500);
	GIF_SetTransparentIndex (frame, 0);
	GIF_AddFrame (gif, frame);
}

void GIF_save (void) {
	GIF_SaveImage (gif, Opt_GIF);
	GIF_FreeImage (gif);
}
