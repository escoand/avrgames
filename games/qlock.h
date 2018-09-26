#ifndef QLOCK_H_
#define QLOCK_H_

#define QLOCK_WORD_PREFIX      2,0, 3,0, 5,0, 6,0, 7,0
#define QLOCK_WORD_BEFORE      1,2, 2,2, 3,2, 4,2, 7,2, 8,2, 9,2
#define QLOCK_WORD_AFTER       1,2, 2,2, 3,2, 4,2, 2,3, 3,3, 4,3, 5,3
#define QLOCK_WORD_QUARTER     1,5, 2,5, 3,5, 4,5, 5,5, 6,5, 7,5
#define QLOCK_WORD_HALF        2,6, 3,6, 4,6, 5,6
#define QLOCK_WORD_FORTYFIVE   0,5, 1,5, 2,5, 3,5, 4,5, 5,5, 6,5, 7,5
#define QLOCK_WORD_OCLOCK      7,6, 8,6
#define QLOCK_WORD_ONE         2,16, 3,16, 4,16, 5,16
#define QLOCK_WORD_TWO         4,14, 5,14, 6,14, 7,14
#define QLOCK_WORD_THREE       6,10, 7,10, 8,10, 9,10
#define QLOCK_WORD_FOUR        6,16, 7,16, 8,16, 9,16
#define QLOCK_WORD_FIVE        0,11, 1,11, 2,11, 3,11
#define QLOCK_WORD_SIX         1,8, 2,8, 3,8, 4,8, 5,8
#define QLOCK_WORD_SEVEN       1,12, 2,12, 3,12, 4,12, 5,12, 6,12
#define QLOCK_WORD_EIGHT       1,17, 2,17, 3,17, 4,17
#define QLOCK_WORD_NINE        5,11, 6,11, 7,11, 8,11
#define QLOCK_WORD_TEN         0,9, 1,9, 2,9, 3,9
#define QLOCK_WORD_ELEVEN      0,15, 1,15, 2,15
#define QLOCK_WORD_TWELVE      4,13, 5,13, 6,13, 7,13, 8,13

/*
 * 0 = before next hour
 * 1 = after prev hour
 */
#define QLOCK_QUARTER_AFTER    0
#define QLOCK_HALF_AFTER       0
#define QLOCK_FORTYFIVE_AFTER  0

void            qlock_main(void);

#endif				/* QLOCK_H_ */
