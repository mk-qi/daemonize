#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))


static int _fork_off_to_child(void)
{
    pid_t pid;
    
    pid = fork();
    if(pid < 0)
        return -errno;

    /* exiting parent */
    if(pid > 0)
        _exit(EXIT_SUCCESS);
    
    return 0;
}

static int _create_new_session(void)
{
    pid_t sid;
    
    /* create independant session */
    sid = setsid();
    if(sid < 0)
        return -errno;
    
    return 0;
}

static int _ignore_signals(void)
{
    const int signals[] = { SIGHUP, SIGINT, SIGTSTP, SIGTTIN, SIGTTOU };
    int num_signals;
    struct sigaction sa;
    
    sa.sa_flags   = SA_SIGINFO;
    sa.sa_handler = SIG_IGN;
    
    num_signals = ARRAY_SIZE(signals);
    
    while(num_signals--) {
        if(sigaction(signals[num_signals], &sa, NULL) < 0)
            return -errno;
    }
    
    return 0;
}

static int _redirect_std_streams(void)
{
    int i, num_fds, oflag;
    const int fds[] = { STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO };
    
    num_fds = ARRAY_SIZE(fds);
    
    for(i = 0; i < num_fds; ++i) {
        if(close(fds[i]) < 0)
            return -errno;
        
        oflag = O_NOCTTY | O_NOFOLLOW;
        oflag |= (fds[i] == STDIN_FILENO) ? O_RDONLY : O_RDWR;
        /*
         * 'open()' always picks the lowest available file descriptor
         * thus redirecting one of the standard streams to /dev/null
         */
        if(open("/dev/null", oflag) != fds[i])
            return -errno;
    }
    
    return 0;
}


int daemonize(mode_t mask, const char *__restrict dir)
{
    int err;
    
    err = _fork_off_to_child();
    if(err < 0)
        return err;
    
    err = _create_new_session();
    if(err < 0)
        return err;
    
    err = _fork_off_to_child();
    if(err < 0)
        return err;
    
    err = _ignore_signals();
    if(err < 0)
        return err;
    
    /* set file permissions */
    umask(mask);
    
    /* change working directory */
    if(chdir(dir) < 0)
        return -errno;

    err = _redirect_std_streams();
    if(err < 0)
        return err;
    
    return 0;
}