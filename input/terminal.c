#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#if _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#endif

#include "input.h"

void
initInput(void)
{
#if _WIN32
    DWORD           mode;
    HANDLE          hstdin;

    hstdin = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hstdin, &mode);
    SetConsoleMode(hstdin, 0);
#else
    struct termios  mode;

    tcgetattr(STDIN_FILENO, &mode);
    mode.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &mode);
#endif
}

uint32_t
getInput(void)
{
    uint8_t         button = 0;
    uint8_t         buf[128];
    uint8_t         len;

#ifdef _WIN32
    if (kbhit())
#else
    struct timeval  tv;
    fd_set          fds;

    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    if (FD_ISSET(STDIN_FILENO, &fds))
#endif
    {
	len = read(STDIN_FILENO, buf, sizeof(buf) - 1);
	button = buf[len - 1];
    }

    if (button == 'a')
	return BUTTON_LEFT;
    else if (button == 'd')
	return BUTTON_RIGHT;
    else if (button == 'w')
	return BUTTON_UP;
    else if (button == 's')
	return BUTTON_DOWN;

    return BUTTON_NONE;
}
