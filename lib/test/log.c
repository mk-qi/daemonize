
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdarg.h>

#include "log.h"

#define CLOCK_ID CLOCK_MONOTONIC

static FILE *_log;
static struct timespec _start;
static bool _print_pid;

static inline struct timespec 
_timespec_diff(const struct timespec *__restrict ts1,
               const struct timespec *__restrict ts2)
{
    struct timespec ret;
    
    ret.tv_sec  = ts1->tv_sec - ts2->tv_sec;
    
    if(ts2->tv_nsec > ts1->tv_nsec) {
        --ret.tv_sec;
        ret.tv_nsec = 1e9 + ts1->tv_nsec - ts2->tv_nsec;
    } else {
        ret.tv_nsec = ts1->tv_nsec - ts2->tv_nsec;
    }
    
    return ret;
}

static inline double _timespec_to_double(const struct timespec *__restrict ts)
{
    return (double) ts->tv_sec + ts->tv_nsec / 1e9;
}

static inline double 
_timespec_diff_to_double(const struct timespec *__restrict ts1,
               const struct timespec *__restrict ts2)
{
    return _timespec_to_double(ts1) - _timespec_to_double(ts2);
}

int log_init(bool print_pid)
{
    int err;
    
    _log = fopen("/tmp/daemon.log", "a");
    if(!_log)
        return -errno;
    
    if(clock_gettime(CLOCK_ID, &_start) < 0) {
        err = -errno;
        fclose(_log);
        return err;
    }
    
    _print_pid = print_pid;
    
    log_print("Log initialized\n");
    
    return 0;
}

void log_destroy(void)
{
    log_print("Closing log\n");
    fclose(_log);
}

void log_print(const char *__restrict fmt, ...)
{
    va_list vargs;
    struct timespec now;
    double runtime;
    
    if(_print_pid)
        fprintf(_log, "< PID: %d >", getpid());
    
    if(clock_gettime(CLOCK_ID, &now) < 0) {
        fprintf(_log, "[ ERRTIME ] ");  
    } else {
        runtime = _timespec_diff_to_double(&now, &_start);
        fprintf(_log, "[ %.6f ] ", runtime);
    }
    
    va_start(vargs, fmt);
    
    vfprintf(_log, fmt, vargs);
    
    va_end(vargs);
    
    fflush(_log);
}