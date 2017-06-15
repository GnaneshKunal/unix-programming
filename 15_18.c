#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096

static void sig_pipe(int); // signal handler

int main(void) {
    int n, fd1[2], fd2[2];
    pid_t pid;
    char line[MAXLINE];

    if (signal(SIGPIPE, sig_pipe) == SIG_ERR) {
        puts("signal error");
        exit(1);
    }

    if (pipe(fd1) < 0 || pipe(fd2) < 0) {
        puts("pipe error");
        exit(1);
    }

    if ((pid = fork()) < 0) {
        puts("fork error");
        exit(1);
    } else if (pid > 0) { // parent
        close(fd1[0]);
        close(fd2[1]);

        while (fgets(line, MAXLINE, stdin) != NULL) {
            n = strlen(line);
            if (write(fd1[1], line, n) != n) {
                puts("write error");
                exit(1);
            }
            if ((n = read(fd2[0], line, MAXLINE)) < 0) {
                puts("read error");
                exit(1);
            }

            if (n == 0) {
                puts("child closed pipe");
                exit(1);
            }
            line[n] = 0; // null terminate
            if (fputs(line, stdout) == EOF) {
                puts("fputs error");
                exit(1);
            }
        }
        if (ferror(stdin)) {
            puts("fgets error on stdin");
            exit(1);
        }
        exit(0);
    } else {
        close(fd1[1]);
        close(fd2[0]);
        if (fd1[0] != STDIN_FILENO) {
            if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO) {
                puts("dup2 error");
                exit(1);
            }
            close(fd1[0]);
        }
        if (fd2[1] != STDOUT_FILENO) {
            if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO) {
                puts("dup2 error");
                exit(1);
            }
            close(fd2[1]);
        }
        if (execl("./temp2", "temp2", (char *)0) < 0) {
            puts("execl error");
            exit(1);
        }
    }
    exit(0);
}

static void sig_pipe(int signo) {
    printf("SIGPIPE caught\n");
    exit(1);
}
