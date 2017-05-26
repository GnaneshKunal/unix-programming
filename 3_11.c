#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    int val;
    if (argc != 2) {
        puts("Usage: a.out<descriptor>");
        return EXIT_FAILURE;
    }

    if ((val = fcntl(atoi(argv[1]), F_GETFL), 0) < 0) {
        printf("fcnlt error for fd %d", atoi(argv[1]));
        return EXIT_FAILURE;
    }
    switch (val & O_ACCMODE) {
        case O_RDONLY:
            puts("read only");
            break;
        case O_WRONLY:
            puts("write only");
            break;
        case O_RDWR:
            puts("read write");
            break;
        default:
            puts("unknown access mode");
            break;
    }
    if (val & O_APPEND)
        puts(", append");
    if (val & O_NONBLOCK)
        puts(", nonblocking");
    if (val & O_SYNC)
        puts(", synchronous writes");

#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
    if (val & O_FSYNC)
        puts(", synchronous writes");
#endif
    putchar('\n');
    
    return EXIT_SUCCESS;
}