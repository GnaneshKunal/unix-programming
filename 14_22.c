#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>

#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

char buf[][3] = { "abc", "123"};
struct iovec iobuf[2];

int main(void) {
    
    int fd, i, n;

    if ((fd = open("temp.foo", O_RDWR |O_CREAT|O_TRUNC, FILE_MODE)) < 0) {
        puts("open error");
        exit(1);
    }

    for (i = 0; i < 2; i++) {
        iobuf[i].iov_base = buf +i;
        iobuf[i].iov_len = 3;
    }
    if ((n = writev(fd, iobuf, 2)) != 6) {
        puts("some thing went wrong");
        exit(1);
    }
    close(fd);
    puts("write successful");
    exit(0);
}