#ifndef TETRIS_H_
#define TETRIS_H_

#define BOARD_WIDTH  4
#define BOARD_HEIGHT 18
#define TETRIS_BRICK_SIZE   4

enum tetris_actions
{ NONE, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, ROTATE_LEFT, ROTATE_RIGHT,
  REVERSE
};


void output (void);
int tetris_main (void);

#endif /* TETRIS_H_ */
