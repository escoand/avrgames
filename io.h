#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <inttypes.h>

#define PORT_STRIPE          B
#define PIN_STRIPE           2

#define PORT_MOVE_LEFT       B
#define PIN_MOVE_LEFT        3

#define PORT_MOVE_RIGHT      B
#define PIN_MOVE_RIGHT       4

#define PORT_ROTATE_LEFT     B
#define PIN_ROTATE_LEFT      1

#define PORT_ROATE_RIGHT     B
#define PIN_ROATE_RIGHT      0

#define BOARD_WIDTH          10
#define BOARD_HEIGHT         18
#define BOARD_STRIPE_MODE    3	// 0=top-bottom 1=bottom-top 2=chain-from-top 3=chain-from-bottom

#define OUTPUT_COLORS_COUNT  6
#define OUTPUT_BLOCK_SIZE    (2*BOARD_HEIGHT)


#define CONCAT(a, b)         a ## b
#define CONCAT_EXP(a, b)     CONCAT(a, b)


#define DDRREG_STRIPE        CONCAT_EXP(DDR,PORT_STRIPE)
#define PORTREG_STRIPE       CONCAT_EXP(PORT,PORT_STRIPE)

#define DDRREG_MOVE_LEFT     CONCAT_EXP(DDR,PORT_MOVE_LEFT)
#define PORTREG_MOVE_LEFT    CONCAT_EXP(PORT,PORT_MOVE_LEFT)

#define DDRREG_MOVE_RIGHT    CONCAT_EXP(DDR,PORT_MOVE_RIGHT)
#define PORTREG_MOVE_RIGHT   CONCAT_EXP(PORT,PORT_MOVE_RIGHT)

#define DDRREG_ROTATE_LEFT   CONCAT_EXP(DDR,PORT_ROATE_LEFT)
#define PORTREG_ROATE_LEFT   CONCAT_EXP(PORT,PORT_ROATE_LEFT)

#define DDRREG_ROTATE_RIGHT  CONCAT_EXP(DDR,PORT_ROATE_RIGHT)
#define PORTREG_ROATE_RIGHT  CONCAT_EXP(PORT,PORT_ROATE_RIGHT)


typedef uint8_t board_matrix[BOARD_HEIGHT][BOARD_WIDTH];

void initOutput (void);
void output (board_matrix * board);
uint8_t getKey (void);

#endif /* OUTPUT_H_ */
