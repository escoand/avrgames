#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <inttypes.h>


#define PORT_STRIPE          B
#define PIN_STRIPE           2

#define PORT_LEFT            B
#define PIN_LEFT             3

#define PORT_RIGHT           B
#define PIN_RIGHT            4

#define PORT_UP              B
#define PIN_UP               1

#define PORT_DOWN            B
#define PIN_DOWN             0

#define BOARD_WIDTH          10
#define BOARD_HEIGHT         18
#define BOARD_STRIPE_MODE    3	// 0=top-bottom 1=bottom-top 2=chain-from-top 3=chain-from-bottom

#define OUTPUT_COLORS_COUNT  6
#define OUTPUT_BLOCK_SIZE    (2 * BOARD_HEIGHT)


#ifdef __AVR__
#define BUTTON_LEFT          1
#define BUTTON_RIGHT         2
#define BUTTON_UP            3
#define BUTTON_DOWN          4
#else
#define BUTTON_LEFT          'a'
#define BUTTON_RIGHT         'd'
#define BUTTON_UP            'q'
#define BUTTON_DOWN          'e'
#endif


#define CONCAT_RAW(a, b, c)  a ## b ## c
#define CONCAT2(a, b)        CONCAT_RAW(a, b, )
#define CONCAT3(a, b, c)     CONCAT_RAW(a, b, c)


#define DDRREG_STRIPE        CONCAT2(DDR, PORT_STRIPE)
#define PORTREG_STRIPE       CONCAT2(PORT, PORT_STRIPE)
#define PINBIT_STRIPE        CONCAT3(P, PORT_STRIPE, PIN_STRIPE)

#define DDRREG_LEFT          CONCAT2(DDR, PORT_LEFT)
#define PORTREG_LEFT         CONCAT2(PORT, PORT_LEFT)
#define PINBIT_LEFT          CONCAT3(P, PORT_LEFT, PIN_LEFT)

#define DDRREG_RIGHT         CONCAT2(DDR, PORT_RIGHT)
#define PORTREG_RIGHT        CONCAT2(PORT, PORT_RIGHT)
#define PINBIT_RIGHT         CONCAT3(P, PORT_RIGHT, PIN_RIGHT)

#define DDRREG_UP            CONCAT2(DDR, PORT_UP)
#define PORTREG_UP           CONCAT2(PORT, PORT_UP)
#define PINBIT_UP            CONCAT3(P, PORT_UP, PIN_UP)

#define DDRREG_DOWN          CONCAT2(DDR, PORT_DOWN)
#define PORTREG_DOWN         CONCAT2(PORT, PORT_DOWN)
#define PINBIT_DOWN          CONCAT3(P, PORT_DOWN, PIN_DOWN)


typedef uint8_t board_matrix[BOARD_HEIGHT][BOARD_WIDTH];

void initOutput (void);
void output (board_matrix * board);
uint8_t getButton (void);

#endif /* OUTPUT_H_ */
