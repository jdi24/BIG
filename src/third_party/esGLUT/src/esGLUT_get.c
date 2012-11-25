#include "esGLUT/esGLUT.h"

#if !defined ESGLUT_OS_WEB

#ifdef ESGLUT_OS_OSX

static unsigned long long ESGLUT_API _RawTimerInNs() {
    static uint64_t start = 0;
    static mach_timebase_info_data_t info = {0,0};
    if (info.denom == 0) {
        mach_timebase_info(&info);
        start = mach_absolute_time();
    }
    uint64_t end = mach_absolute_time();
    uint64_t difference = end - start;
    return difference * (info.numer / info.denom);
}

static unsigned long ESGLUT_API _GetTimeMs() {
    static const unsigned long long _processStartTime = _RawTimerInNs();
    unsigned long long newTime = _RawTimerInNs() - _processStartTime;
    return (unsigned long)newTime;
}

#elif defined ESGLUT_OS_LINUX

#include <unistd.h>
#include <sys/time.h>

static unsigned long ESGLUT_API _GetTimeMs() {
    struct timeval time;
    int status = gettimeofday(&time, NULL);
    return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}

#elif defined ESGLUT_OS_WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

static unsigned long ESGLUT_API _GetTimeMs() {
    static DWORD _processStartTime = ~0;
    if (_processStartTime == ~0) {
        _processStartTime = GetTickCount();
    }
    return (GetTickCount() - _processStartTime);
}

#endif

ESGLUT_EXTERN int ESGLUT_API glutGet(GLenum query) {
    switch (query) {
    case GLUT_ELAPSED_TIME:
        return _GetTimeMs();
    default:
        return 0;
    }
}

#endif
