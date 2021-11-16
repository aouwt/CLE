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
		void distcolor (color color);
		color filter (color fcolor, color bcolor);
		unsigned char gr (color color);
		unsigned char gg (color color);
		unsigned char gb (color color);
		color rgb (unsigned char r, unsigned char g, unsigned char b);
		color merge (struct beams colors);
};

void cell::runcell (void) {
	switch (op) {
		case 'R': distcolor (RED); break;
		case 'G': distcolor (GREEN); break;
		case 'B': distcolor (BLUE); break;
		case 'W': distcolor (WHITE); break;
		case 'Y': distcolor (YELLOW); break;
		case 'C': distcolor (CYAN); break;
		case 'M': distcolor (MAGENTA); break;
		
		case 'r': distcolor (RED); op = '\0'; break;
		case 'g': distcolor (GREEN); op = '\0'; break;
		case 'b': distcolor (BLUE); op = '\0'; break;
		case 'w': distcolor (WHITE); op = '\0'; break;
		case 'y': distcolor (YELLOW); op = '\0'; break;
		case 'c': distcolor (CYAN); op = '\0'; break;
		case 'm': distcolor (MAGENTA); op = '\0'; break;
		
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
	struct mfb_window *window = mfb_open ("CLEc", 800, 600);
	
}
