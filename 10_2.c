#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

static void sig_usr(int);

int main(void) {
    if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
        puts("can't catch SIGUSR1");
        exit(1);
    }
    if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
        puts("can't catch SIGUSR2");
        exit(1);
    }
    for (;;)
        pause();
}

static void sig_usr(int signo) {
    if (signo == SIGUSR1)
        puts("received SIGUSR1");
    else if (signo == SIGUSR2)
        puts("received SIGUSR2");
    else
        puts("received some signal");
}