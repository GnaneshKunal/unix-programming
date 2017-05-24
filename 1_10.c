#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

#define MAXSIZE 4096
static void sig_int(int);

int main(void) {
    int status;
    char buff[MAXSIZE];
    pid_t pid;

    if (signal(SIGINT, sig_int) == SIG_ERR) {
        puts("Signal error");
        exit(1);
    }

    printf("%% ");
    while (fgets(buff, MAXSIZE, stdin) != NULL) {
        if (buff[strlen(buff) - 1] == '\n')
            buff[strlen(buff) - 1] = 0;
        if ((pid = fork()) < 0) {
            puts("fork error");
            return 1;
        } else if (pid == 0) {
            execlp(buff, buff, (char *) 0);
            return 127;
        }

        if ((pid = waitpid(pid, &status, 0)) < 0) {
            puts("wait error");
            return 1;
        }
        printf("%% ");
    }
}

void sig_int(int signo) {
    printf("Interrupt\n%% ");
}