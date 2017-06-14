#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAXLINE 4096

int main(void) {
    int n;
    int fd[2];
    pid_t pid;
    char line[MAXLINE];

    if (pipe(fd) < 0) {
        puts("pipe error");
        exit(1);
    }
    if ((pid = fork()) < 0) {
        puts("fork error");
        exit(1);
    } else if (pid > 0) {
        close(fd[0]);
        write(fd[1], "hello world\n", 12);
    } else {
        close(fd[1]);
        n = read(fd[0], line, MAXLINE);
        write(STDOUT_FILENO, line, n);
    }
    exit(0);
}