#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "log.h"

void
logger(const char *lvl, const char *fmt, ...)
{
    static time_t   rawtime;
    static struct tm *timeinfo;

    char            buf[1024];
    va_list         args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    printf("%04i-%02i-%02i %02i:%02i:%02i %s %s\n",
	   1900 + timeinfo->tm_year, timeinfo->tm_mon, timeinfo->tm_mday,
	   timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, lvl,
	   buf);
}
