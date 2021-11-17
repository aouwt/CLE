#include <stdio.h>

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
		
		case 'r': beam = {RED}; op = '\0'; break;
		case 'g': beam = {GREEN}; op = '\0'; break;
		case 'b': beam = {BLUE}; op = '\0'; break;
		case 'w': beam = {WHITE}; op = '\0'; break;
		case 'y': beam = {YELLOW}; op = '\0'; break;
		case 'c': beam = {CYAN}; op = '\0'; break;
		case 'm': beam = {MAGENTA}; op = '\0'; break;
		
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
		
		default: op = ' ';
	}
}



void updatebeams (void) {
	struct beams newboard [board.width] [board.height];
	
	unsigned int x, y;
	
	// clear the board and transfer to new array
	for (x = 0; x != board.width; x++) {	for (y = 0; y != board.height; y++) {
		newboard[x][y] = board.board[x][y].beam;
		board.board[x][y].beam = {0};
	}	}
	
	for (x = 0; x != board.width; x++) {	for (y = 0; y != board.height; y++) {
		if (x != board.width-1) board.board[x][y].beam.l = newboard[x+1][y].l;
		if (y != board.height-1) board.board[x][y].beam.r = newboard[x][y+1].r;
		if (x) board.board[x][y].beam.r = newboard[x-1][y].r;
		if (y) board.board[x][y].beam.d = newboard[x][y-1].d;
	}	}
}

void tick (void) {
	updatebeams ();
	unsigned int x, y;
	for (y = 0; y != board.height; y++) {
		for (x = 0; x != board.width; x++)
			board.board[x][y].runcell ();
	}
}


void initboard (unsigned int width, unsigned int height) {
	board.board = new class cell*[width];
	
	for(unsigned int i = 0; i < width; ++i)
		board.board[i] = new class cell[height];
	
	board.width = width; board.height = height;
}

void cleanup (void) {
	delete[] board.board; // does this delete everything or...?
}

void loadboard (FILE* f) {
	auto tempboard = new class cell [1024] [1024];
	
	unsigned int x, y;
	for (y = 0; y != 1024; y++) {
		for (x = 0; x != 1024; x++) {
			
			int c = fgetc (f);
			if (c == EOF)
				goto next;
			else if (c == '\n')
				break;
			else tempboard[x][y].op = c;
			
		}
	}
	
	next:;
	unsigned int w = x, h = y;
	initboard (w, h);
	for (y = 0; y != h; y++) {
		for (x = 0; x != w; x++) {
			board.board[x][y] = tempboard[x][y];
		}
	}
	
	delete[] tempboard;
}

int main () {
	//struct mfb_window *window = mfb_open ("CLEc", 800, 600);
	FILE* f = fopen ("test.txt", "r");
	loadboard (f);
	fclose (f);
	
	while (true) {
		updatebeams ();
	}
}
