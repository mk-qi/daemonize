#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <stdarg.h>

#define ERR_WARNING  "*WARNING* :: "
#define ERR_CRITICAL "*CRITICAL* :: "

/* declaration with __attribute__ */
static void _err_out(FILE *__restrict stream, 
                    const char *__restrict severity, 
                    const char *__restrict fmt, 
                    ...)  __attribute__((format(printf, 3, 4)));

/* definition */
static void _err_out(FILE *__restrict stream, 
                    const char *__restrict severity, 
                    const char *__restrict fmt, 
                    ...)
{
    va_list varg;
    
    if(!stream)
        stream = stderr;
    
    va_start(varg, fmt);
    
    fprintf(stream, "%s :: ", severity);
    vfprintf(stream, fmt, varg);
    fflush(stream);
    
    va_end(varg);
}

static void _err_func(FILE *__restrict stream,
                     const char *__restrict severity,
                     const char *__restrict func,
                     int err)
{
    char *fmt = "%s failed: %s, errno: %d\n";
    
    _err_out(stream, severity, fmt, func, strerror(err), err);
}


pid_t daemonize(mode_t mask, const char *__restrict dir)
{
    pid_t pid, sid;
    struct sigaction sa;
    
    pid = fork();
    if(pid < 0) {
        _err_func(stderr, ERR_CRITICAL, "fork()", errno);
        return -1;
    }
    
    /* exiting parent */
    if(pid > 0)
        exit(EXIT_SUCCESS);
    
    /* create independant session */
    sid = setsid();
    if(sid < 0) {
        _err_func(stderr, ERR_CRITICAL, "setsid()", errno);
        return -1;
    }
    
    /* ignore signals */
    sa.sa_flags   = SA_SIGINFO;
    sa.sa_handler = SIG_IGN;
    
    if(sigaction(SIGHUP, &sa, NULL) < 0) {
        _err_func(stderr, ERR_CRITICAL, "sigaction()", errno);
        return -1;
    }
    
    if(sigaction(SIGINT, &sa, NULL) < 0) {
        _err_func(stderr, ERR_CRITICAL, "sigaction()", errno);
        return -1;
    }
    
    /* set file permissions */
    umask(mask);
    
    /* change working directory */
    if(chdir(dir) < 0) {
        _err_func(stderr, ERR_CRITICAL, "chdir()", errno);
        return -1;
    }
    
    /* close standard streams */
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);
    
    return sid;
}