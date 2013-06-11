#include <sys/stat.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>

#include <daemonize.h>

#include "log.h"

static bool _loop;

static void _handle_sigterm(int sig)
{
    if(sig == SIGTERM)
        _loop = false;
}

static int _sighandlers_init(void)
{
    struct sigaction sa;
    
    memset(&sa, 0, sizeof(sa));
    
    sa.sa_flags   = SA_SIGINFO;
    sa.sa_handler = &_handle_sigterm;
    
    if(sigaction(SIGTERM, &sa, NULL) < 0)
        return -errno;
    
    return 0;
}

static int _init(void)
{
    int err;
    mode_t mask;
    
    mask = S_IXUSR | S_IWGRP | S_IXGRP | S_IWOTH | S_IXOTH;
    
    err = daemonize(mask, "/", true);
    if(err < 0)
        return err;
    
    err = _sighandlers_init();
    if(err < 0)
        return err;
    
    err = log_init(true);
    if(err < 0)
        return err;
    
    _loop = true;
    
    return 0;
}

static void _destroy(void)
{
    log_destroy();
}

int main(int argc, char *argv[])
{
    int seconds;

    if(_init() < 0)
        exit(EXIT_FAILURE);
    
    log_print("Daemon initialized\n");
    
    seconds = 0;
    
    while(_loop) {
        log_print("Daemon running for %d seconds\n", seconds);
        sleep(5);
        seconds += 5;
    }
    
    log_print("Daemon received signal to terminate\n");
    
    _destroy();
    
    return EXIT_SUCCESS;
}