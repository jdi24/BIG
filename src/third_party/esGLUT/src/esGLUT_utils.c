#include "esGLUT/esGLUT.h"

#if !defined ESGLUT_OS_WEB

#include <stdio.h>
#include <stdarg.h>

ESGLUT_EXTERN void ESGLUT_API glutLogMessage(const char *formatStr, ...)
{
    va_list params;
    char buf[BUFSIZ];

    va_start(params, formatStr);
    vsprintf_s(buf, sizeof(buf), formatStr, params);

    puts(buf);

    va_end(params);

    fflush(stdout);
}

#endif
