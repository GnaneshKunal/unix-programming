#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

void exit_print(int x, ...) {
    char *buf = malloc(300 * sizeof(char));
    va_list strings;
    va_start(strings, x);
    for (int i = 0; i < x; i++) {
        strcat(buf, va_arg(strings, char *));
    }
    va_end(strings);
    strcat(buf, "\n");
    write(STDOUT_FILENO, buf, strlen(buf));
    free(buf);
}

int main(int argc, char **argv) {
    if (argc != 2)
        exit_print(1, "usage: a.out <pathname>");   
    if (access(argv[1], R_OK) < 0)
        exit_print(2, "access error for ", argv[1]);
    else
        exit_print(1, "read access OK");
    if (open(argv[1], O_RDONLY) < 0)
        exit_print(2, "open error for ", argv[1]);
    else
        exit_print(0, 1, "open for reading");
    exit(0);
}