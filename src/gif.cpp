#include <SDL2/SDL_gifwrap.h>
#include "gui.hpp"
#include "cle.hpp"

GIF_Image *gif = NULL;


void GIF_setup (void) {
	gif = GIF_CreateImage ();
	GIF_SetCanvasSize (gif, WindowSurface -> w, WindowSurface -> h);
}

void GIF_tick (void) {
	GIF_Frame *frame = GIF_CreateFrame (WindowSurface, SDL_FALSE);
	GIF_SetDelay (frame, 500);
	GIF_AddFrame (gif, frame);
	GIF_FreeFrame (frame);
}

void GIF_save (void) {
	GIF_SaveImage (gif, Opt_GIF);
	GIF_FreeImage (gif);
}
