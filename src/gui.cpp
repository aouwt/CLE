#include "cle.hpp"
#include "gui.hpp"
#include <stdio.h>
#include <cstdlib>
#include <errno.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


SDL_Window *Window;
SDL_Surface *WindowSurface, *BeamsSurface, *TextSurface;
SDL_Rect BeamsStretch;
SDL_Event Event;
TTF_Font *Font;

const char EM_CREATESURFACE[] = "Could not create surface";
bool UpdateTextSurface = false;

void setupwindow (void) {
	// SDL initialize
	SDLERR (SDL_Init (SDL_INIT_VIDEO) < 0, "Could not initialize SDL");
	TTFERR (TTF_Init () < 0, "Could not initialize SDL_ttf");
	
	// Window initialize
	Window = SDL_CreateWindow (
		"CLE",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		Board.width*FONT_SIZE*2,Board.height*FONT_SIZE*2,
		SDL_WINDOW_RESIZABLE
	);
	SDLERR (Window == NULL, "Could not create window");
	
	// surface
	WindowSurface = SDL_GetWindowSurface (Window);
	SDLERR (WindowSurface == NULL, "Could not retrieve window surface");
	
	TextSurface = SDL_CreateRGBSurface (0, Board.width * FONT_SIZE*2, Board.height * FONT_SIZE*2, 32, DEFMASK);
	SDLERR (TextSurface == NULL, EM_CREATESURFACE);
	
	BeamsSurface = SDL_CreateRGBSurface (0, Board.width*3, Board.height*3, 32, DEFMASK);
	SDLERR (BeamsSurface == NULL, EM_CREATESURFACE);
	SDL_FillRect (BeamsSurface, NULL, 0xFF7F7F7F);
	
	// renderer
	//Renderer = SDL_CreateRenderer (Window, -1, NULL);
	//SDLERR (Renderer == NULL, "Could not create renderer");
	
	// load font
	Font = TTF_OpenFont ("./rc/LiberationMono-Regular.ttf", FONT_SIZE);
	TTFERR (Font == NULL, "Could not load font");
}

void stopsdl (void) {
	// SDL
	SDL_FreeSurface (WindowSurface);
	SDL_FreeSurface (BeamsSurface);
	SDL_FreeSurface (TextSurface);
	
	SDL_DestroyWindow (Window);
	
	
	// TTF
	TTF_CloseFont (Font);
	
	
	// Quit
	TTF_Quit ();
	SDL_Quit ();
}



void resizewindow (void) {
	unsigned int
		w = Event.window.data1,
		h = Event.window.data2;
		
	SDL_FreeSurface (TextSurface);
	
	TextSurface = SDL_CreateRGBSurface (0, w,h, 32, DEFMASK);
	SDLERR (TextSurface == NULL, EM_CREATESURFACE);
	
	UpdateTextSurface = true;
	
	SDL_SetWindowSize (Window, w, h);
	WindowSurface = SDL_GetWindowSurface (Window);
}


void eventdriver (void) {
	if (Event.type == SDL_WINDOWEVENT) {
		
		switch (Event.window.event) {
			
			case SDL_WINDOWEVENT_RESIZED:
				resizewindow ();
				
		}
		
	} else if (Event.type == SDL_QUIT) {
		
		stopsdl ();
		exit (0);
		
	}
}


void drawbeams (void) {
	uint32_t* buf = ((uint32_t*)BeamsSurface -> pixels);
	
	#define get_index(px,py) ( \
		((x * 3) + (px)) + \
		(((y * 3) + (py)) * (Board.width * 3)) \
	)
	
	#define rgba(rgb) /*((rgb) ? */((rgb) | 0xFF000000)/* : 0)*/
		unsigned int x, y;
		for (y = 0; y != Board.height; y++) {
			for (x = 0; x != Board.width; x++) {
			
				buf[get_index(1,1)] = rgba (Board.board[x][y].state);
				buf[get_index(0,1)] = rgba (Board.board[x][y].beam.l);
				buf[get_index(2,1)] = rgba (Board.board[x][y].beam.r);
				buf[get_index(1,0)] = rgba (Board.board[x][y].beam.u);
				buf[get_index(1,2)] = rgba (Board.board[x][y].beam.d);
				
			}
		}
	#undef rgba
	#undef get_index
}




void rendertext (void) {
	SDL_FillRect (TextSurface, NULL, 0x00000000);
	
	SDL_Surface *tempsurf, *tempsurf2;
	
	unsigned int x, y;
	unsigned char
		fw = TextSurface -> w / Board.width,
		fh = TextSurface -> h / Board.height;
	
	SDL_Color white = { 0xFF, 0xFF, 0xFF };
	SDL_Rect rect = { 0,0, fw,fh };
	for (y = 0; y != Board.height; y++) {
		rect.y = fh*y;
		
		for (x = 0; x != Board.width; x++) {
			rect.x = fw*x;
			
			const char op[] = { Board.board[x][y].op, 0 };
			if (op[0] <= 32) continue;
			
			tempsurf = TTF_RenderText_Solid (Font, op, white);
			TTFERR (tempsurf == NULL, EM_CREATESURFACE);
			
			tempsurf2 = SDL_ConvertSurface (tempsurf, TextSurface -> format, 0);
			SDLERR (tempsurf2 == NULL, EM_CREATESURFACE);
			
			SDL_BlitScaled (
				tempsurf2, NULL,
				TextSurface, &rect
			);
			
			SDL_FreeSurface (tempsurf); SDL_FreeSurface (tempsurf2);
			
		}
	}
	
	/*SDL_DestroyTexture (TextTexture);
	TextTexture = SDL_CreateTextureFromSurface (Renderer, TextSurface);
	SDLERR (TextTexture == NULL, "Could not convert surface to texture");*/
	
	//TextSurfaceOptimized = SDL_ConvertSurface (TextSurface, ScreenSurface -> format, 0);
	//SDLERR (TextSurfaceOptimized == NULL, "Could not optimize surface");
	
	UpdateTextSurface = false;
}



void updatewindow (void) {
	while (SDL_PollEvent (&Event)) eventdriver ();
	if (UpdateTextSurface) rendertext ();
	
	drawbeams ();
	
	//SDL_BlitSurface (TextSurface, NULL, WindowSurface, NULL);
	SDL_BlitScaled (BeamsSurface, NULL, WindowSurface, NULL);
	SDL_BlitSurface (TextSurface, NULL, WindowSurface, NULL);
	SDL_UpdateWindowSurface (Window);
}
