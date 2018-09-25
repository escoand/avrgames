#ifndef QLOCK_H_
#define QLOCK_H_

#define QLOCK_WORD_PREFIX      2,17, 3,17, 5,17, 6,17, 7,17
#define QLOCK_WORD_BEFORE      1,15, 2,15, 3,15, 4,15, 7,14, 8,14, 9,14
#define QLOCK_WORD_AFTER       1,15, 2,15, 3,15, 4,15, 2,14, 3,14, 4,14, 5,14
#define QLOCK_WORD_QUARTER     1,12, 2,12, 3,12, 4,12, 5,12, 6,12, 7,12
#define QLOCK_WORD_HALF        2,11, 3,11, 4,11, 5,11
#define QLOCK_WORD_FORTYFIVE   0,12, 1,12, 2,12, 3,12, 4,12, 5,12, 6,12, 7,12
#define QLOCK_WORD_OCLOCK      7,11, 8,11
#define QLOCK_WORD_ONE         2,1, 3,1, 4,1, 5,1
#define QLOCK_WORD_TWO         4,3, 5,3, 6,3, 7,3
#define QLOCK_WORD_THREE       6,7, 7,7, 8,7, 9,7
#define QLOCK_WORD_FOUR        6,1, 7,1, 8,1, 9,1
#define QLOCK_WORD_FIVE        0,6, 1,6, 2,6, 3,6
#define QLOCK_WORD_SIX         1,9, 2,9, 3,9, 4,9, 5,9
#define QLOCK_WORD_SEVEN       1,5, 2,5, 3,5, 4,5, 5,5, 6,5
#define QLOCK_WORD_EIGHT       1,0, 2,0, 3,0, 4,0
#define QLOCK_WORD_NINE        5,6, 6,6, 7,6, 8,6
#define QLOCK_WORD_TEN         0,8, 1,8, 2,8, 3,8
#define QLOCK_WORD_ELEVEN      0,2, 1,2, 2,2
#define QLOCK_WORD_TWELVE      4,4, 5,4, 6,4, 7,4, 8,4

#define QLOCK_QUARTER_AFTER    0   // 0 = before next hour, 1 = after prev hour
#define QLOCK_HALF_AFTER       0   // 0 = before next hour, 1 = after prev hour
#define QLOCK_FORTYFIVE_AFTER  0   // 0 = before next hour, 1 = after prev hour


void qlock_main (void);

#endif /* QLOCK_H_ */
