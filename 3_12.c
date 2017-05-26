#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 4096

int main(void) {
    int n, fdp_in, fdp_out;
    char buf[MAXSIZE];

    //dup actually duplicates file descriptor (or) with fcntl(fd, F_DUPFD, 0);
    //with dup2, you can choose your own fd number (or) with fcntl(fd, F_DUPFD, 7);

    fdp_in = dup(STDIN_FILENO);
    fdp_out = dup2(STDOUT_FILENO, 7);

    while ((n = read(fdp_in, buf, MAXSIZE)) > 0) {
        if ((write(7, buf, n)) != n) { //can use fdp_out or 7
            puts("write error");
            return EXIT_FAILURE;
        }
        
        if (n < 0) {
            puts("read error");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}