#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(void) {
    char buf[2046];

    if (link("/home/monster/git/unix-programming/tempfile", "/home/monster/git/unix-programming/tempfile2") < 0) {
        perror("Error:");
        exit(1);
    }
    write(STDOUT_FILENO, "DONE", strlen("DONE"));
    exit(0);
}