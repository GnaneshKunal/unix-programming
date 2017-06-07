#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void pr_mask(const char*);
static void sig_int(int);

int main(void) {
    sigset_t newmask, oldmask, waitmask;
    pr_mask("program start: ");

    if (signal(SIGINT, sig_int) == SIG_ERR) {
        puts("signal SIGINT error");
        exit(1);
    }
    sigemptyset(&waitmask);
    sigaddset(&waitmask, SIGUSR1);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);
    // Block SIGINT and save current signal mask.

    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
        puts("SIG_BLOCK error");
        exit(1);
    }
    // Critical region code
    pr_mask("in critical region: ");
    //pause all signals except SIGUSR1

    if (sigsuspend(&waitmask) != -1) {
        puts("sigsuspend error");
        exit(1);
    }

    pr_mask("after return from sigsuspend: ");
    //reset signal mask whick unblocks SIGINT

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        puts("SIG_SETMASK error");
        exit(1);
    }

    // and continue processing...

    pr_mask("program exit: ");
    exit(0);
}

static void sig_int(int signo) {
    pr_mask("\ninsig_int: ");
}

void pr_mask(const char *str) {
	sigset_t	sigset;
	int			errno_save;

	errno_save = errno;		/* we can be called by signal handlers */
	if (sigprocmask(0, NULL, &sigset) < 0) {
		puts("sigprocmask error");
        exit(1);
	} else {
		printf("%s", str);
		if (sigismember(&sigset, SIGINT))
			printf(" SIGINT");
		if (sigismember(&sigset, SIGQUIT))
			printf(" SIGQUIT");
		if (sigismember(&sigset, SIGUSR1))
			printf(" SIGUSR1");
		if (sigismember(&sigset, SIGALRM))
			printf(" SIGALRM");

		/* remaining signals can go here  */

		printf("\n");
	}

	errno = errno_save;		/* restore errno */
}
