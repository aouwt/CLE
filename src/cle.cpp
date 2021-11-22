#include "cle.hpp"
#include "gui.hpp"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <unistd.h>

struct board Board;
bool Opt_Debug = false;
unsigned int Opt_Delay = 0;
bool Opt_Step = false;


color rgb (unsigned char r, unsigned char g, unsigned char b) { return (r << 16) | (g << 8) | b; }

color gr (color color) { return (color & 0xFF0000) >> 16; }
color gg (color color) { return (color & 0x00FF00) >> 8; }
color gb (color color) { return (color & 0x0000FF); }


color filter (color bcolor, color fcolor) {

	if (!bcolor) return 0;
	
	unsigned char max = gr (fcolor);
	if (gg (fcolor) > max) max = gg (fcolor);
	if (gb (fcolor) > max) max = gb (fcolor);
	if (max == 0) return 0;

	return rgb (
		gr(bcolor) * (float(gr(fcolor)) / max),
		gg(bcolor) * (float(gg(fcolor)) / max),
		gb(bcolor) * (float(gb(fcolor)) / max)
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

struct beams distcolor (color color) {
	return { color, color, color, color };
}


void cell::runcell (void) {
	switch (op) {
		case 'R': beam = distcolor (RED); break;
		case 'G': beam = distcolor (GREEN); break;
		case 'B': beam = distcolor (BLUE); break;
		case 'W': beam = distcolor (WHITE); break;
		case 'Y': beam = distcolor (YELLOW); break;
		case 'C': beam = distcolor (CYAN); break;
		case 'M': beam = distcolor (MAGENTA); break;

		case 'r': beam = distcolor (RED);
			op = '\0';
			#ifdef _GUI
				UpdateTextSurface = true;
			#endif
			break;
		case 'g': beam = distcolor (GREEN);
			op = '\0';
			#ifdef _GUI
				UpdateTextSurface = true;
			#endif
			break;
		case 'b': beam = distcolor (BLUE);
			op = '\0';
			#ifdef _GUI
				UpdateTextSurface = true;
			#endif
			break;
		case 'w': beam = distcolor (WHITE);
			op = '\0';
			#ifdef _GUI
				UpdateTextSurface = true;
			#endif
			break;
		case 'y': beam = distcolor (YELLOW);
			op = '\0';
			#ifdef _GUI
				UpdateTextSurface = true;
			#endif
			break;
		case 'c': beam = distcolor (CYAN);
			op = '\0';
			#ifdef _GUI
				UpdateTextSurface = true;
			#endif
			break;
		case 'm': beam = distcolor (MAGENTA);
			op = '\0';
			#ifdef _GUI
				UpdateTextSurface = true;
			#endif
			break;

		case '?':
			if (state)
				beam = distcolor (state);
			else
				state = merge (beam);
			break;

		case '!':
			if (state)
				state = 0;
			else
				state = merge (beam);
			break;

		case '#':
			beam = {
				rgb (gr(beam.u) / 2, gg(beam.u) / 2, gb(beam.u) / 2),
				rgb (gr(beam.d) / 2, gg(beam.d) / 2, gb(beam.d) / 2),
				rgb (gr(beam.l) / 2, gg(beam.l) / 2, gb(beam.l) / 2),
				rgb (gr(beam.r) / 2, gg(beam.r) / 2, gb(beam.r) / 2)
			}; break;

		case '@':
			beam = {beam.d, beam.u, beam.r, beam.l};
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

		case'\\': beam = {beam.l, beam.r, beam.u, beam.d}; break;
		case '/': beam = {beam.r, beam.l, beam.d, beam.u}; break;

		case '|': beam.l = beam.r = 0; break;
		case '-': beam.u = beam.d = 0; break;

		case ' ': {
				state = merge (beam);
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
						filter (beam.r, vc),
						filter (beam.l, vc),
						filter (beam.d, hc),
						filter (beam.u, hc)
					};
			}; break;

		default:
			if (Opt_Debug) {
			} else {
				op = ' ';
				#ifdef _GUI
					UpdateTextSurface = true;
				#endif
			}
			break;
	}
}



void updatebeams (void) {
	struct beams newboard [Board.width+1] [Board.height+1];

	unsigned int x, y;

	// clear the board and transfer to new array
	for (x = 0; x != Board.width; x++) {
		for (y = 0; y != Board.height; y++) {
			newboard[x][y] = Board.board[x][y].beam;
			Board.board[x][y].beam = {0};
		}
	}

	for (x = 0; x != Board.width; x++) {
		for (y = 0; y != Board.height; y++) {
			if (x != Board.width-1) Board.board[x][y].beam.l = newboard[x+1][y].l;
			if (y != Board.height-1) Board.board[x][y].beam.u = newboard[x][y+1].u;
			if (x) Board.board[x][y].beam.r = newboard[x-1][y].r;
			if (y) Board.board[x][y].beam.d = newboard[x][y-1].d;
		}
	}
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


/*
void cleanup (void) {
	delete[] Board.board; // does this delete everything or...?
}*/



void loadboard (FILE* f) {
	char tempboard[1024][1024] = {0};

	unsigned int x, y;
	unsigned int w = 0, h = 0;
	for (y = 0; y != 1024; y++) {
		for (x = 0; x != 1024; x++) {

			int c = fgetc (f);
			if (c == EOF)
				goto next;
			else if (c == '\n')
				break;
			else tempboard[x][y] = c;

		}
		if (x > w) w =x;
	}

	next:;
	h = y;
	initboard (w, h);
	for (y = 0; y != h; y++) {
		for (x = 0; x != w; x++) {
			switch (tempboard[x][y]) {
				case 'r': case 'R': Board.board[x][y].state = RED; break;
				case 'g': case 'G': Board.board[x][y].state = GREEN; break;
				case 'b': case 'B': Board.board[x][y].state = BLUE; break;
				case 'c': case 'C': Board.board[x][y].state = CYAN; break;
				case 'y': case 'Y': Board.board[x][y].state = YELLOW; break;
				case 'm': case 'M': Board.board[x][y].state = MAGENTA; break;
				case 'w': case 'W': Board.board[x][y].state = WHITE; break;
			}
			Board.board[x][y].op = tempboard[x][y];
		}
	}

//	Board.width = ++w; Board.height = ++h;
}


void printhelpscreen (char *cmd) {
	printf ("CLE -- Color Laser Esolang\n");
	printf ("https://esolang.org/wiki/CLE\n");
	printf ("https://github.com/aouwt/CLE\n");
	printf ("\n");
	printf ("Usage: %s [options] <file>\n", cmd);
	printf ("\n");
	printf (" -h, --help   Displays this help message\n");
	printf (" -s, --step   Enables single-stepping mode\n");
	//printf (" -d, --debug  Enables debugging features\n");
	printf (" -D, --delay <num>  Set the delay between cycles\n");
	printf ("\n");
}



int main (int argcount, char* args[]) {
	struct alias { char s; char* a; } aliases[] = {
		{ 'h', "--help" },
		//{ 'd', "--debug" },
		{ 0, 0 }
	};
	char* file = NULL;
	
	// argument parser
	for (unsigned char i = 1; args[i] != NULL; i++) {

		if (args[i][0] == '-') {
			char* arg = args[i] + 1;

			if (arg[0] == '-') {
				for (unsigned char al = 0; aliases[al].s != 0; al++) {
					if (!strcmp (aliases[al].a, args[i])) {
						arg = &aliases[al].s;
						goto checkarg;
					}
				}
				ARGERR ("Invalid argument");
			}
			
			checkarg:
			for (unsigned char ac = 0; arg[ac]; ac++) {
				switch (arg[0]) {
					case 'h':
					printhelpscreen (args[0]);
					exit (0);
					break;

	//				case 'd':
	//					Opt_Debug = true;
	//					break;

					case 's':
						Opt_Step = true;
						break;

					case 'D':
						Opt_Delay = atoi (args[++i]) * 1000;
						break;

					default:
						ARGERR ("Invalid argument");
						break;
				}
			}

		} else {
			// assume its a file
			file = args[i];
		}

	}

	if (file == NULL) ARGERR ("No file specified");
	
	//load file
	{	FILE* f = fopen (file, "r");
		THISERR (f == NULL, "Could not open file", ENOENT);
		loadboard (f);
		fclose (f);
	}
	
	#ifdef _GUI
		setupwindow ();
	#endif
	while (true) {
		tick ();
		#ifdef _GUI
			updatewindow ();
		#endif
		if (Opt_Step) getchar ();
		if (Opt_Delay) usleep (Opt_Delay);
	}
}
