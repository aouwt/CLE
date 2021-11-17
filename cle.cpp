#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define CYAN 0x00FFFF
#define YELLOW 0xFFFF00
#define MAGENTA 0xFF00FF
#define WHITE 0xFFFFFF
#define BLACK 0x000000

class cell {
	public:
		typedef unsigned long color;
		struct beams {
			color u, d, l, r = {0};
		};
		struct beams beam = {0};
		color state = 0;
		char op = '\0';
		
		void runcell (void);
		
	private:
		color filter (color bcolor, color fcolor);
		unsigned char gr (color color);
		unsigned char gg (color color);
		unsigned char gb (color color);
		color rgb (unsigned char r, unsigned char g, unsigned char b);
		color merge (struct beams colors);
};

color cell::filter (color bcolor, color fcolor) {
	if (!bcolor) return;
	unsigned char max = gr (fcolor);
	if (gg (fcolor) > max) max = gg (fcolor);
	if (gb (fcolor) > max) max = gb (fcolor);
	if (!max) return;
	
	return rgb (
		gr(bcolor) * (gr(fcolor) / max),
		gg(bcolor) * (ggfcolor) / max),
		gb(bcolor) * (gb(fcolor) / max)
	);
}

color cell::merge (struct beams colors) {
	unsigned int
		r = gr(colors.l) + gr(colors.r) + gr(colors.u) + gr(colors.d),
		g = gg(colors.l) + gg(colors.r) + gg(colors.u) + gg(colors.d),
		b = gb(colors.l) + gb(colors.r) + gb(colors.u) + gb(colors.d);
	if (r > 255) r = 255;
	if (g > 255) g = 255;
	if (b > 255) b = 255;
	return rgb (r,g,b);
}

color cell:rgb (unsigned char r, unsigned char g, unsigned char b) { return (r << 16) | (g << 8) | b; }

color cell:gr (color color) { return (color & 0xFF0000) >> 16; }
color cell:gg (color color) { return (color & 0x00FF00) >> 8; }
color cell:gb (color color) { return (color & 0x0000FF); }

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
				distcolor (state);
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
		
		case ' ': if (beam) {
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


void main () {
	//struct mfb_window *window = mfb_open ("CLEc", 800, 600);
	
}
