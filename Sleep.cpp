#include "Sleep.h"
#include <QObject>

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

void Sleep::sleep(unsigned long secs)
{
#ifdef Q_OS_WIN32
    ::Sleep(secs);
#else
    struct timeval tv;
    gettimeofday(&tv, 0);
    struct timespec ti;
    ti.tv_sec = tv.tv_sec + secs;
    ti.tv_nsec = (tv.tv_usec);
    thread_sleep(&ti);
#endif
}
