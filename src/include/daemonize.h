#ifndef DAEMONIZE_H_
#define DAEMONIZE_H_

#include <sys/types.h>

#include <unistd.h>

/*
 * function:
 *      daemonize
 * description:
 *      After calling this function the process has taken all actions
 *      to be able to run as a daemon, except creating and writing to log
 *      files. After a successful function call the application shall not make
 *      use of stderr, stdout and stdin.
 * 
 * @mask:
 *      value gained by or'ing the following values:
 * 
 *         S_IRWXU    00700     mask for file owner permissions
 *         S_IRUSR    00400     owner has read permission
 *         S_IWUSR    00200     owner has write permission
 *         S_IXUSR    00100     owner has execute permission
 *         S_IRWXG    00070     mask for group permissions
 *         S_IRGRP    00040     group has read permission
 *         S_IWGRP    00020     group has write permission
 *         S_IXGRP    00010     group has execute permission
 *         S_IRWXO    00007     mask for permissions for others (not in group)
 *         S_IROTH    00004     others have read permission
 *         S_IWOTH    00002     others have write permission
 *         S_IXOTH    00001     others have execute permission
 * 
 * @dir:
 *      Working directory of the daemon.
 * 
 * @return:
 *      Session id of the daemon or -1 on failure.
 * 
 * 
 * example usage:
 * 
 *      #include <unistd.h>
 *      #include "daemonize.h"
 * 
 *      int main(int argc, char *argv[])
 *      {
 *              if(daemonize(S_IXUSR | S_IWGRP | S_IXGRP | S_IWOTH | S_IXOTH, "/") < 0)
 *                      exit(EXIT_FAILURE);
 * 
 *              init_other_stuff();
 * 
 *              while(1) {
 *                      do_daemon_stuff();
 *              }
 * 
 *              return EXIT_SUCCESS;
 *      }
 */

pid_t daemonize(mode_t mask, const char *__restrict dir);

#endif /* DAEMONIZE_H_ */