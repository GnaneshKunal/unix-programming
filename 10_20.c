#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <setjmp.h>
#include <time.h>
#include <errno.h>

void pr_mask(const char *);
static void sig_usr1(int);
static void sig_alrm(int);
static sigjmp_buf jmpbuf;
static volatile sig_atomic_t canjump;

int main(void) {
    if (signal(SIGUSR1, sig_usr1) == SIG_ERR) {
        puts("signal SIGUSR1 error");
        exit(1);
    }
    if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
        puts("signal SIGALRM error");
        exit(2);
    }
    pr_mask("starting main:");
    
    if (sigsetjmp(jmpbuf, 1)) {
        pr_mask("ending main:");
        exit(0);
    }
    canjump = 1;
    for (;;)
        pause();
}

static void sig_usr1(int signo) {
    time_t starttime;
    if (canjump == 0)
        return;
    pr_mask("starting sig_usr1:");

    alarm(3);
    starttime = time(NULL);
    for(;;)
        if (time(NULL) > starttime + 5)
            break;
    pr_mask("finishing sig_usr1");

    canjump = 0;
    siglongjmp(jmpbuf, 1);
}

static void sig_alrm(int signo) {
    pr_mask("in sig_alrm");
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
