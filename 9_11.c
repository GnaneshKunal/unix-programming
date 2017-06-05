#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>

static void sig_hup(int signo) {
    printf("SIGUP received, pid = %ld\n", (long)getpid());
}

static void pr_ids(char *name) {
    printf("%s : pid = %ld, ppid = %ld, pgrp = %ld, tpgrp = %ld\n", name,
            (long)getpid(), (long)getppid(), (long)getpgrp(), (long)tcgetpgrp(STDIN_FILENO));
            fflush(stdout);
}

int main(void) {
    char c;
    pid_t pid;
    pr_ids("parent:");
    if ((pid = fork()) < 0) {
        puts("fork error");
        exit(1);
    } else if (pid > 0) {
        sleep(5); //sleep to let child stop it self
    } else {
        pr_ids("Child:");
        signal(SIGHUP, sig_hup); //establish signal handler
        kill(getpid(), SIGTSTP); // stop ourself
        pr_ids("child"); //prints only if we are continued
        if (read(STDIN_FILENO, &c, 1) != 1)
            printf("read error %d on conrolling TTY\n", errno);
    }
    exit(0);
}