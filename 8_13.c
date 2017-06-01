#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

static int	pfd1[2], pfd2[2];
static sigset_t newmask, oldmask, zeromask;
static volatile sig_atomic_t sigflag;

static void charatatime(char *);
static void sig_usr(int);
void TELL_WAIT(void);
void WAIT_PARENT(void);
void TELL_CHILD(pid_t pid);

int main(void) {
    pid_t pid;
    TELL_WAIT();

    if ((pid = fork()) < 0) {
        puts("fork error");
        exit(1);
    } else if (pid == 0) {
        WAIT_PARENT();
        charatatime("output from child\n");
    } else {
        charatatime("output from parent\n");
        TELL_CHILD(pid);
    }
    exit(0);
}

static void charatatime(char *str) {
    char *ptr;
    int c;
    setbuf(stdout, NULL);
    for (ptr = str; (c = *ptr++) != 0;)
        putc(c, stdout);
}

static void
sig_usr(int signo)	/* one signal handler for SIGUSR1 and SIGUSR2 */
{
	sigflag = 1;
}

void
TELL_CHILD(pid_t pid)
{
	kill(pid, SIGUSR1);			/* tell child we're done */
}

void
TELL_WAIT(void)
{
	if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
        puts("signal(SIGUSR1) error");
        exit(1);
    }
	if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
        puts("signal(SIGUSR2) error");
        exit(1);
    }
	sigemptyset(&zeromask);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGUSR1);
	sigaddset(&newmask, SIGUSR2);

	/* Block SIGUSR1 and SIGUSR2, and save current signal mask */
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
        puts("SIG_BLOCK error");
        exit(1);
    }
}

void
WAIT_PARENT(void)
{
	while (sigflag == 0)
		sigsuspend(&zeromask);	/* and wait for parent */
	sigflag = 0;

	/* Reset signal mask to original value */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        puts("SIG_SETMASK error");
        exit(1);
    }
}