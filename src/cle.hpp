#ifndef CLE_HPP
	#define CLE_HPP
	
	#define RED 0xFF0000
	#define GREEN 0x00FF00
	#define BLUE 0x0000FF
	#define CYAN 0x00FFFF
	#define YELLOW 0xFFFF00
	#define MAGENTA 0xFF00FF
	#define WHITE 0xFFFFFF
	#define BLACK 0x000000
	
	#define CHECKERR(cond,msg,code) if (cond) { fprintf (stderr, "ERROR: %s (E%i,L%i)\n", (msg), (code), __LINE__); exit (1); }
	#define THISERR(cond,msg,code) if (cond) { fprintf (stderr, "ERROR: %s (L%i)\n", (msg), __LINE__); exit (code); }
	#define ARGERR(msg) { fprintf (stderr, "ERROR: %s (L%i)\n", msg, __LINE__); exit (EINVAL); }
	



	typedef unsigned long color;



	struct beams {
		color u, d, l, r = {0};
	};



	class cell {
		public:
			struct beams beam = {0};
			color state = 0;
			char op = '\0';

			void runcell (void);
	};



	struct board {
		class cell** board;
		unsigned int width;
		unsigned int height;
	};


	const char EM_GENERAL[] = "Unknown error";
	
	extern struct board Board;
	extern bool UpdateTextSurface;
	
	extern char *Opt_GIF;
#endif
