#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int c;

    while ((c = getchar()) != EOF) {
        if (isupper(c))
            c = tolower(c);
        if (putchar(c) == EOF) {
            puts("output error");
            exit(1);
        }
        if (c == '\n')
            fflush(stdout);
    }
}