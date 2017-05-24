#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int c;
    while ((c = getc(stdin)) != EOF) //getc() - Get character from stream
        if (putc(c, stdout) == EOF) { //putc() - put character to the stream
            puts("output error");
            exit(1);
        }
    if (ferror(stdin)) { // Check error indicator
        puts("Input error");
        exit(1);
    }

    exit(0);
}