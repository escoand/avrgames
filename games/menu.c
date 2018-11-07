#include <stdint.h>
#include <string.h>

#if _WIN32
#include <windows.h>
#define ms_sleep(x) Sleep(x)
#else
#include <unistd.h>
#define ms_sleep(x) usleep(x * 1000)
#endif

#include "../input/input.h"
#include "../output/output.h"
#include "menu.h"
#include "clock.h"
#include "qlock.h"
#include "tetris.h"

board_matrix    menu = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    ,
    {0, 1, 0, 0, 0, 2, 2, 2, 0, 0}
    ,
    {0, 1, 0, 0, 2, 0, 3, 0, 2, 0}
    ,
    {0, 1, 0, 0, 2, 0, 3, 3, 2, 0}
    ,
    {0, 1, 0, 0, 2, 0, 0, 0, 2, 0}
    ,
    {0, 1, 0, 0, 0, 2, 2, 2, 0, 0}
    ,
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    ,
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    ,
    {0, 2, 0, 0, 3, 0, 5, 0, 0, 0}
    ,
    {0, 2, 0, 0, 3, 5, 5, 5, 6, 0}
    ,
    {0, 2, 0, 0, 3, 3, 2, 2, 6, 0}
    ,
    {0, 2, 0, 0, 4, 4, 2, 2, 6, 0}
    ,
    {0, 2, 0, 0, 0, 4, 4, 0, 6, 0}
    ,
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    ,
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    ,
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    ,
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    ,
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

void
menu_main(void)
{
    uint32_t        slept = 0;
    uint32_t        button;

    setOutput(&menu);

    while (slept <= MENU_TIMEOUT_MS) {
	button = getInput();

	if (button & BUTTON_UP) {
	    clock_main();
	    break;
	} else if (button & BUTTON_LEFT) {
	    tetris_main();
	    break;
	}

	ms_sleep(100);
	slept += 100;
    }
}
