#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(void) {
    int c;
    while ((c = getc(stdin)) != EOF)
        if (putc(c, stdout) == EOF) {
            perror("Error");
            exit(1);
        }
    if (feof(stdin)) {
        perror("Error");
        exit(1);
    }
    exit(0);
}