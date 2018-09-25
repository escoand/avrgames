#ifndef INPUT_GPIO_H_
#define INPUT_GPIO_H_

#ifdef __AVR__

#define PORT_STRIPE          B
#define PIN_STRIPE           2

#define PORT_LEFT            B
#define PIN_LEFT             4

#define PORT_RIGHT           B
#define PIN_RIGHT            0

#define PORT_UP              B
#define PIN_UP               1

#define PORT_DOWN            B
#define PIN_DOWN             3

#define CONCAT_RAW(a, b, c)  a ## b ## c
#define CONCAT2(a, b)        CONCAT_RAW(a, b, )
#define CONCAT3(a, b, c)     CONCAT_RAW(a, b, c)

#define DDRREG_STRIPE        CONCAT2(DDR, PORT_STRIPE)
#define PORTREG_STRIPE       CONCAT2(PORT, PORT_STRIPE)
#define PINBIT_STRIPE        CONCAT3(P, PORT_STRIPE, PIN_STRIPE)

#define DDRREG_LEFT          CONCAT2(DDR, PORT_LEFT)
#define PORTREG_LEFT         CONCAT2(PORT, PORT_LEFT)
#define PINREG_LEFT          CONCAT2(PIN, PORT_LEFT)
#define PINBIT_LEFT          CONCAT3(P, PORT_LEFT, PIN_LEFT)

#define DDRREG_RIGHT         CONCAT2(DDR, PORT_RIGHT)
#define PORTREG_RIGHT        CONCAT2(PORT, PORT_RIGHT)
#define PINREG_RIGHT         CONCAT2(PIN, PORT_RIGHT)
#define PINBIT_RIGHT         CONCAT3(P, PORT_RIGHT, PIN_RIGHT)

#define DDRREG_UP            CONCAT2(DDR, PORT_UP)
#define PORTREG_UP           CONCAT2(PORT, PORT_UP)
#define PINREG_UP            CONCAT2(PIN, PORT_UP)
#define PINBIT_UP            CONCAT3(P, PORT_UP, PIN_UP)

#define DDRREG_DOWN          CONCAT2(DDR, PORT_DOWN)
#define PORTREG_DOWN         CONCAT2(PORT, PORT_DOWN)
#define PINREG_DOWN          CONCAT2(PIN, PORT_DOWN)
#define PINBIT_DOWN          CONCAT3(P, PORT_DOWN, PIN_DOWN)

#else
#endif

#endif /* INPUT_GPIO_H_ */
