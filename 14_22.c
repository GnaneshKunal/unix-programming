#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>

char buf1[3] = "abc", buf2[3] = "123";
struct iovec iobuf[2];

int main(void) {
    
    int fd, i, n;

    if ((fd = open("temp.foo", O_RDWR |O_CREAT)) < 0) {
        puts("open error");
        exit(1);
    }

    iobuf[0].iov_base = buf1;
    iobuf[1].iov_base = buf2;

    for (i = 0; i < 2; i++) {
        iobuf[i].iov_len = 3;
    }

    if ((n = writev(fd, iobuf, 2)) != 6) {
        puts("some thing went wrong");
        exit(1);
    }
    puts("write successful");
    exit(0);
}