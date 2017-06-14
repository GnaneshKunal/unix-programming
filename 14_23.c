#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/uio.h>

#define SIZE 3
#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

struct iovec iovbuf[2];
char buf[2][SIZE];

int main(void) {

    int fd, i, j;

    if ((fd = open("temp.foo", O_RDONLY, FILE_MODE)) < 0) {
        perror("Error");
        exit(1);
    }

    for (i = 0; i < 2; i++) {
        iovbuf[i].iov_base = buf +i;
        iovbuf[i].iov_len = SIZE;
    }

    if (readv(fd, iovbuf, 2) != SIZE+SIZE) {
        puts("readv error");
        exit(1);
    }

    for (i = 0; i < 2; i++)
        for (j = 0; j < SIZE; j++)
            printf("%p: %c\n", *(buf +i) + j, *(*(buf +i) +j));
    
    close(fd);

    exit(0);
}