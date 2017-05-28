#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>

int main(int argc, char **argv) {
    int i, fd;
    struct stat statbuf;
    struct timespec times[2];
    char *buf = malloc(200 * sizeof(char));

    for (i = 1; i < argc; i++) {
        if (stat(argv[i], &statbuf) < 0) {
            perror("Error");
            continue;
        }
        if ((fd = open(argv[i], O_RDWR | O_TRUNC)) < 0) {
            perror("Error");
            continue;
        }
        times[0] = statbuf.st_atim;
        times[1] = statbuf.st_mtim;
        if (futimens(fd, times) < 0) {
            perror("Error");
            exit(1);
        }
        close(fd);
    }
    exit(0);
}