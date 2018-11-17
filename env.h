#ifndef ENV_H
#define ENV_H

#ifdef __AVR__
#include <avr/io.h>
#include <util/delay.h>
#define ms_sleep(x) {uint16_t ms; for(ms=x;ms>0;ms--) _delay_ms(1);}
#elif _WIN32
#include <windows.h>
#define ms_sleep(x) Sleep(x)
#else
#include <unistd.h>
#define ms_sleep(x) usleep(x * 1000)
#endif

#endif				/* ENV_H */
