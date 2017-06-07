#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

static void sig_quit(int);

int main(void) {
    sigset_t newmask, oldmask, pendmask;

    if (signal(SIGQUIT, sig_quit) == SIG_ERR) {
        puts("Can't catch SIGQUIT");
        exit(1);
    }
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
        puts("SIG_BLOCK error");
        exit(2);
    }
    sleep(5);
    if (sigpending(&pendmask) < 0)
        puts("sigpending error");
    if (sigismember(&pendmask, SIGQUIT))
        puts("SIGQUIT pending");
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        puts("SIG_SETMASK error");
        exit(3);
    }

    puts("SIGQUIT unblocked");
    sleep(5);
    exit(0);
}

static void sig_quit(int signo) {
    puts("caught SIGQUIT");
    if (signal(SIGQUIT, SIG_DFL) == SIG_ERR) {
        perror("Error");
        exit(1);
    }
}