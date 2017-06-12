#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>

sigset_t mask;

extern int already_running(void);
extern void daemonize(const char *);

void reread(void) {
    //
}

void *thr_fn(void *arg) {
    int err, signo;

    for (;;) {
        err = sigwait(&mask, &signo);
        if (err != 0) {
            syslog(LOG_ERR, "sigwait failed");
            exit(1);
        }
        switch (signo) {
            case SIGHUP:
                syslog(LOG_INFO, "Re-reading configuration file");
                reread();
                break;
            case SIGTERM:
                syslog(LOG_INFO, "got SIGTERM; exiting");
                exit(0);
            default:
                syslog(LOG_INFO, "unexpected signal %d\n", signo);
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    int err;
    pthread_t tid;
    char *cmd;
    struct sigaction sa;

    if ((cmd = strrchr(argv[0], '/')) == NULL)
        cmd = argv[0];
    else cmd++;

    // Become a daemon

    daemonize(cmd);

    //Make sure only one copy of daemon is running.

    if (already_running()) {
        syslog(LOG_ERR, "daemon already exists");
        exit(1);
    }

    //Restore SIGHUP default and block all signals

    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        printf("can't restore SIGHUP default");
        exit(1);
    }
    sigfillset(&mask);
    if ((err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) {
        puts("SIG_BLOCK eror");
        exit(1);
    });

    //create a thread to handle SIGHUP and SIGTERM

    err = pthread_create(&tid, NULL, thr_fn, 0);
    if (err != 0) {
        puts("can't create thread");
        exit(1);
    }

    //proceed wth the rest of the daemon

    exit(0);

}
