#ifndef CLOCK_H_
#define CLOCK_H_

/*
 * 0 = 12 hours clock
 * 1 = 24 hours clock
 */
#define CLOCK_24_HOURS    1

#define CLOCK_DIGIT_WIDTH 3
#define CLOCK_DIGIT_HEIGHT 5
#define CLOCK_DIGIT_ROW1  2
#define CLOCK_DIGIT_ROW2  11
#define CLOCK_DIGIT_COL1  1
#define CLOCK_DIGIT_COL2  6

#define CLOCK_RETURN_SUCCESS  0
#define CLOCK_RETURN_FAILURE  1

uint8_t         clock_main(void);

#endif				/* CLOCK_H_ */
