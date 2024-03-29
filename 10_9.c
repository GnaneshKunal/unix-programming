#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <setjmp.h>

unsigned int sleep2(unsigned int);
static void sig_int(int);
static void sig_alrm(int);
static jmp_buf env_alrm;

int main(void) {
    unsigned int unslept;
    if (signal(SIGINT, sig_int) == SIG_ERR) {
        perror("Error");
        exit(1);
    }
    unslept = sleep2(5);
    printf("unslept returned %u \n", unslept);
    exit(0);
}

static void sig_alrm(int signo) {
    longjmp(env_alrm, 1);
}

unsigned int sleep2(unsigned int seconds) {
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        return seconds;
    if (setjmp(env_alrm) == 0) {
        alarm(seconds);
        pause();
    }
    return (alarm(0));
}

static void sig_int(int signo) {
    int i, j;
    volatile int k;

    puts("sig_int starting");
    for (i = 0; i < 300000; i++)
        for (j = 0; j < 4000; j++)
            k += i * j;
        puts("sig_int finished");
}