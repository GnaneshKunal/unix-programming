#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv) {
    fprintf(stderr, "EACCES: %s\n", strerror(EACCES));
    // char *strerror(int errnum) - returns error message based on errnum
    errno = ENOENT;
    perror(argv[0]); //produces err message based on errno (errno.h)
    exit(0);
}