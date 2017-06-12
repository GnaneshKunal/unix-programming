#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <unistd.h>
#include <signal.h>

void daemonize(const char *cmd) {
    int i, fd0, fd1, fd2;
    pid_t pid;

    struct rlimit r1;
    struct sigaction sa;

    //clear file creation mask
    umask(0);
    //get max number of file descriptors

    if (getrlimit(RLIMIT_NOFILE, &r1) < 0) {
        printf("%s: can't get file limit\n", cmd);
        exit(1);
    }

    //become session leader to close controlling TTY
    if ((pid = fork()) < 0) {
        printf("%s: can't fork", cmd);
        exit(1);
    } else if (pid != 0)
        exit(0); // parent
    setsid();

    //ensure future opens won't allocate controlling TTY

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        printf("%s: can't ignore SIGHUP", cmd);
        exit(1);
    }
    if ((pid = fork()) < 0) {
        printf("%s: can't fork", cmd);
        exit(1);
    } else if (pid != 0)
        exit(0);

    //change current working directory to the root
    //so we won't prevent file system from being unmounted

    if (chdir("/") < 0) {
        printf("%s: can't change directory", cmd);
        exit(1);
    }
    //close all file descriptors

    if (r1.rlim_max == RLIM_INFINITY)
        r1.rlim_max = 1024;
    for (i = 0; i < r1.rlim_max; i++)
        close(i);

    //attach file descriptors 0, 1, 2 to /dev/null

    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    //Initialize the log file.
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "Unexpected file descriptors %d %d %d", fd0, fd1, fd2);
        exit(1);
    }
}

int main(void) {
    daemonize("gk");
    sleep(100);
    exit(0);
}