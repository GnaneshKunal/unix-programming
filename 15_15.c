#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAXLINE 4096

int main(void) {
    char line[MAXLINE];
    FILE *fpin;

    if ((fpin = popen("tempf", "r")) == NULL) {
        puts("popen error");
        exit(1);
    }
    for (;;) {
        fputs("prompt> ", stdout);
        fflush(stdout);
        if (fgets(line, MAXLINE, fpin) == NULL)
            break;
        if (fputs(line, stdout) == EOF) {
            puts("fputs error to pipe");
            exit(1);
        }
    }
    if (pclose(fpin) == -1) {
        puts("pclose error");
        exit(1);
    }
    putchar('\n');
    exit(0);
}