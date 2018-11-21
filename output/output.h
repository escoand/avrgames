#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdint.h>

#define BOARD_WIDTH          10
#define BOARD_HEIGHT         18

enum BOARD_PALETTE {
    BOARD_PALETTE_DEFAULT, BOARD_PALETTE_RAINBOW, BOARD_PALETTE_FIRE
};

typedef uint8_t board_matrix[BOARD_HEIGHT][BOARD_WIDTH];

void            initOutput(void);
uint8_t         mapToPalette(uint8_t, enum BOARD_PALETTE);
void            setOutput(board_matrix * board) { return setOutputUsePalette(board, BOARD_PALETTE_DEFAULT); }
void            setOutputUsePalette(board_matrix *, enum BOARD_PALETTE);
void            setRawOutput(board_matrix * board) { return setRawOutputUsePalette(board, BOARD_PALETTE_DEFAULT); }
void            setRawOutputUsePalette(board_matrix *, enum BOARD_PALETTE);
void            clearOutput(void);

#endif				/* OUTPUT_H */
