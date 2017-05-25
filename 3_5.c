#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAXSIZE 4096

int main(void) {
    char buf[MAXSIZE];
    int n;

    while((n = read(STDIN_FILENO, buf, MAXSIZE)) > 0)
        if (write(STDOUT_FILENO, buf, n) != n) {
            puts("Write error");
            return EXIT_FAILURE;
        }
    if (n < 0) {
        puts("Read error");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}