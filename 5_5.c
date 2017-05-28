#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define MAXSIZE 4096

int main(void) {
    char buf[MAXSIZE];

    while (fgets(buf, MAXSIZE, stdin) != NULL)
        if (fputs(buf, stdout) == EOF) {
            perror("Error");
            exit(1);
        }
    if (ferror(stdin)) {
        puts("Input error");
        exit(1);
    }
    exit(0);
}