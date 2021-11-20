#include "common.h"

color rgb (unsigned char r, unsigned char g, unsigned char b) { return (r << 16) | (g << 8) | b; }

color gr (color color) { return (color & 0xFF0000) >> 16; }
color gg (color color) { return (color & 0x00FF00) >> 8; }
color gb (color color) { return (color & 0x0000FF); }


color filter (color bcolor, color fcolor) {

	if (!bcolor) return 0;
	
	unsigned char max = gr (fcolor);
	if (gg (fcolor) > max) max = gg (fcolor);
	if (gb (fcolor) > max) max = gb (fcolor);
	if (!max) return 0;

	return rgb (
		gr(bcolor) * (gr(fcolor) / max),
		gg(bcolor) * (gg(fcolor) / max),
		gb(bcolor) * (gb(fcolor) / max)
	);
}

color merge (struct beams colors) {
	unsigned int
		r = gr(colors.l) + gr(colors.r) + gr(colors.u) + gr(colors.d),
		g = gg(colors.l) + gg(colors.r) + gg(colors.u) + gg(colors.d),
		b = gb(colors.l) + gb(colors.r) + gb(colors.u) + gb(colors.d);
		
	if (r > 255) r = 255;
	if (g > 255) g = 255;
	if (b > 255) b = 255;
	
	return rgb (r,g,b);
}


void cell::runcell (void) {
	switch (op) {
		case 'R': beam = {RED}; break;
		case 'G': beam = {GREEN}; break;
		case 'B': beam = {BLUE}; break;
		case 'W': beam = {WHITE}; break;
		case 'Y': beam = {YELLOW}; break;
		case 'C': beam = {CYAN}; break;
		case 'M': beam = {MAGENTA}; break;

		case 'r': beam = {RED};
			op = '\0';
			UpdateTextSurface = true;
			break;
		case 'g': beam = {GREEN};
			op = '\0';
			UpdateTextSurface = true;
			break;
		case 'b': beam = {BLUE}; 
			op = '\0';
			UpdateTextSurface = true;
			break;
		case 'w': beam = {WHITE};
			op = '\0';
			UpdateTextSurface = true;
			break;
		case 'y': beam = {YELLOW};
			op = '\0';
			UpdateTextSurface = true;
			break;
		case 'c': beam = {CYAN};
			op = '\0';
			UpdateTextSurface = true;
			break;
		case 'm': beam = {MAGENTA};
			op = '\0';
			UpdateTextSurface = true;
			break;

		case '?':
			if (state)
				beam = {state};
			else
				state = merge (beam);
			break;

		case '#':
			beam = {
				rgb (gr(beam.l) / 2, gg(beam.l) / 2, gb(beam.l) / 2),
				rgb (gr(beam.r) / 2, gg(beam.r) / 2, gb(beam.r) / 2),
				rgb (gr(beam.u) / 2, gg(beam.u) / 2, gb(beam.u) / 2),
				rgb (gr(beam.d) / 2, gg(beam.d) / 2, gb(beam.d) / 2)
			}; break;

		case '@':
			beam = {beam.r, beam.l, beam.d, beam.u};
			break;

		case '{': {
				unsigned char avg = (gr(beam.l) + gg(beam.l) + gb(beam.l)) / 3;
				beam.l = rgb (avg, avg, avg);
				beam.u = beam.d = beam.r = 0;
			} break;
		case '}': {
				unsigned char avg = (gr(beam.r) + gg(beam.r) + gb(beam.r)) / 3;
				beam.r = rgb (avg, avg, avg);
				beam.u = beam.d = beam.l = 0;
			} break;

		case '<': beam.l = merge (beam); break;
		case '>': beam.r = merge (beam); break;
		case '^': beam.u = merge (beam); break;
		case 'v': beam.d = merge (beam); break;

		case '(': if (beam.l) { beam.r = beam.l; beam.l = 0; } break;
		case ')': if (beam.r) { beam.l = beam.r; beam.r = 0; } break;

		case'\\': beam = {beam.u, beam.d, beam.l, beam.r}; break;
		case '/': beam = {beam.d, beam.u, beam.r, beam.l}; break;

		case '|': beam.l = beam.r = 0; break;
		case '-': beam.u = beam.d = 0; break;

		case ' ': {
				color hc, vc;

				if (beam.l && beam.r)
					beam.l = beam.r = hc = merge ({beam.l, beam.r, 0, 0});
				else
					hc = beam.r | beam.l;

				if (beam.u && beam.d)
					beam.u = beam.d = vc = merge ({beam.u, beam.d, 0, 0});
				else
					vc = beam.u | beam.d;

				if (hc && vc) beam = {
						filter (beam.l, vc),
						filter (beam.r, vc),
						filter (beam.u, hc),
						filter (beam.d, hc)
					};
			}; break;

		default: op = ' '; UpdateTextSurface = true; break;
	}
}



void updatebeams (void) {
	struct beams newboard [Board.width+1] [Board.height+1];

	unsigned int x, y;

	// clear the board and transfer to new array
	for (x = 0; x != Board.width; x++) {	for (y = 0; y != Board.height; y++) {
		newboard[x][y] = Board.board[x][y].beam;
		Board.board[x][y].beam = {0};
	}	}

	for (x = 0; x != Board.width; x++) {	for (y = 0; y != Board.height; y++) {
		if (x != Board.width-1) Board.board[x][y].beam.l = newboard[x+1][y].l;
		if (y != Board.height-1) Board.board[x][y].beam.r = newboard[x][y+1].r;
		if (x) Board.board[x][y].beam.r = newboard[x-1][y].r;
		if (y) Board.board[x][y].beam.d = newboard[x][y-1].d;
	}	}
}



void tick (void) {
	updatebeams ();
	unsigned int x, y;
	for (y = 0; y != Board.height; y++) {
		for (x = 0; x != Board.width; x++)
			Board.board[x][y].runcell ();
	}
}



void initboard (unsigned int width, unsigned int height) {
	Board.board = new class cell* [width+1];

	for(unsigned int i = 0; i < width+1; ++i)
		Board.board[i] = new class cell [height+1];

	Board.width = width; Board.height = height;
}



void cleanup (void) {
	delete[] Board.board; // does this delete everything or...?
}



void loadboard (FILE* f) {
	char tempboard[1024][1024] = {0};

	unsigned int x, y;
	for (y = 0; y != 1024; y++) {
		for (x = 0; x != 1024; x++) {

			int c = fgetc (f);
			if (c == EOF)
				goto next;
			else if (c == '\n')
				break;
			else tempboard[x][y] = c;

		}
	}

	next:;
	unsigned int w = x, h = y;
	initboard (w, h);
	for (y = 0; y != h; y++) {
		for (x = 0; x != w; x++) {
			Board.board[x][y].op = tempboard[x][y];
		}
	}

	Board.width = ++w; Board.height = ++h;
}



void rendertext (void) {
	SDL_FillRect (TextSurface, NULL, 0x00000000);
	
	SDL_Surface* tempsurf;
	
	unsigned int x, y;
	unsigned char
		fw = TextSurface -> w / Board.width,
		fh = TextSurface -> h / Board.height;
	
	SDL_Rect rect = { 0,0, fw, fh };
	for (y = 0; y != Board.height; y++) {
		rect.y = fh*y;
		
		for (x = 0; x != Board.width; x++) {
			rect.x = fw*x;
			
			const char op[] = { Board.board[x][y].op, 0 };
			if (op[0] <= 32) continue;
			
			tempsurf = TTF_RenderText_Solid (Font, op, { 0xFF, 0xFF, 0xFF, 0xFF });
			
			TTFERR (tempsurf == NULL, EM_CREATESURFACE);
			
			SDL_BlitScaled (
				tempsurf, NULL,
				TextSurface, &rect
			);
			
			SDL_FreeSurface (tempsurf);
			
		}
	}
	UpdateTextSurface = false;
}



void resizewindow (void) {
	unsigned int
		w = Event.window.data1,
		h = Event.window.data2;
		
	SDL_FreeSurface (TextSurface);
	
	TextSurface = SDL_CreateRGBSurface (0, w,h, 32, 0,0,0,0);
	SDLERR (TextSurface == NULL, EM_CREATESURFACE);
	
	UpdateTextSurface = true;
	
	SDL_SetWindowSize (Window, w, h);
	WindowSurface = SDL_GetWindowSurface (Window);
}


void setupwindow (void) {
	// SDL initialize
	SDLERR (SDL_Init (SDL_INIT_VIDEO) < 0, "Could not initialize SDL");
	TTFERR (TTF_Init () < 0, "Could not initialize SDL_ttf");
	
	// Window initialize
	Window = SDL_CreateWindow (
		"CLE",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		800, 600,
		SDL_WINDOW_RESIZABLE
	);
	SDLERR (Window == NULL, "Could not create window");
	
	// surface
	WindowSurface = SDL_GetWindowSurface (Window);
	SDLERR (WindowSurface == NULL, "Could not retrieve window surface");
	
	TextSurface = SDL_CreateRGBSurface (0, 800,600, 32, 0,0,0,0);
	SDLERR (TextSurface == NULL, EM_CREATESURFACE);
		
	// load font
	Font = TTF_OpenFont ("./LiberationSansMono-Regular.txt", FONT_SIZE);//("/data/data/com.termux/files/usr/share/fonts/TTF/DejaVuSansMono.ttf", 16);
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


/*void makebeamsurfacethingaaaaaa (void) {
	#define get_index(px,py) (((x*3)+px) + ((y*3)+py) * Board.width)
	
		unsigned int x, y;
		for (y = 0; y != Board.height; y++) {
			for (x = 0; x != Board.width; x++) {
			
				windowbuf[get_index(1,1)] = Board.board[x][y].state << 8;
				windowbuf[get_index(0,1)] = Board.board[x][y].beam.l << 8;
				windowbuf[get_index(2,1)] = Board.board[x][y].beam.r << 8;
				windowbuf[get_index(1,0)] = Board.board[x][y].beam.u << 8;
				windowbuf[get_index(1,2)] = Board.board[x][y].beam.d << 8;
				
			}
		}
		
	#undef get_index
}*/



void updatewindow (void) {
	if (SDL_PollEvent (&Event)) eventdriver ();
	if (UpdateTextSurface) rendertext ();
	
	//SDL_BlitScaled (BeamsSurface, NULL, WindowSurface, NULL);
	SDL_BlitScaled (TextSurface, NULL, WindowSurface, NULL);
	SDL_UpdateWindowSurface (Window);
}



int main (int argcount, char* args[]) {
	/*char* file;
	// argument parser
	for (unsigned char i = 0; i != argcount; i++) {

		if (args[i][0] == '-') {
			char arg = args[i][1];

			if (arg == '-') {
				char* a = args[i];
				// alias system here
			}

			switch (arg) {
			}

		} else {
			// assume its a file
			file = args[i];
		}

	}*/

	char file[] = "./test.txt";
	//load file
	{
		FILE* f = fopen (file, "r");
		THISERR (f == NULL, "Could not open file", ENOENT);
		loadboard (f);
		fclose (f);
	}
	
	setupwindow ();
	
	while (true) {
		tick ();
		updatewindow ();
		//getchar ();
		//SDL_Delay (100);
	}
}
