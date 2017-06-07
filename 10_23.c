#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

volatile sig_atomic_t quitflag;

static void sig_int(int signo) {
    if (signo == SIGINT)
        puts("Interrupt");
    else if (signo == SIGQUIT)
        quitflag = 1;
}

int main(void) {
    sigset_t newmask, oldmask, zeromask;

    if (signal(SIGINT, sig_int) == SIG_ERR) {
        puts("signal error");
        exit(1);
    }
    if (signal(SIGQUIT, sig_int) == SIG_ERR) {
        puts("signal error");
        exit(2);
    }

    sigemptyset(&newmask);
    sigemptyset(&zeromask);
    sigaddset(&newmask, SIGQUIT);
    
    //block sigquit and save current flag

    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
        puts("SIG_BLOCK error");
        exit(3);
    }
    while (quitflag == 0)
        sigsuspend(&zeromask);
    //sigquit has been caught and is now blocked
    quitflag = 0;
    //reset signal mask which unblocks SIGQUIT
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        puts("SIG_SETMASK error");
        exit(4);
    }
    exit(0);
}