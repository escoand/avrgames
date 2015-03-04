#define TETRIS_BOARD_WIDTH  10
#define TETRIS_BOARD_HEIGHT 18
#define TETRIS_BRICK_SIZE   4
#define TETRIS_BIT_EMPTY    ' '

char TETRIS_BIT_CONSOLE[] = {
  0x23, 0x24, 0x3d, 0x40, 0x4f, 0x7e
};

int TETRIS_BIT_COLOR[] = {
  0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF, 0x00FFFF
};

char TETRIS_BRICKS[][TETRIS_BRICK_SIZE][TETRIS_BRICK_SIZE] = {
  {
   {0, 0, 0, 0},
   {0, 0, 0, 0},
   {1, 1, 1, 1},
   {0, 0, 0, 0}
   }, {
       {0, 0, 0, 0},
       {1, 0, 0, 0},
       {1, 1, 1, 0},
       {0, 0, 0, 0}
       }, {
	   {0, 0, 0, 0},
	   {0, 0, 1, 0},
	   {1, 1, 1, 0},
	   {0, 0, 0, 0}
	   }, {
	       {0, 0, 0, 0},
	       {0, 1, 1, 0},
	       {0, 1, 1, 0},
	       {0, 0, 0, 0}
	       }, {
		   {0, 0, 0, 0},
		   {0, 1, 1, 0},
		   {1, 1, 0, 0},
		   {0, 0, 0, 0}
		   }, {
		       {0, 0, 0, 0},
		       {1, 1, 0, 0},
		       {0, 1, 1, 0},
		       {0, 0, 0, 0}
		       }, {
			   {0, 0, 0, 0},
			   {0, 0, 1, 0},
			   {0, 1, 1, 1},
			   {0, 0, 0, 0}
			   }
};