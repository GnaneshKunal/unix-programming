#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

pid_t lock_test(int fd, int type, off_t offset, int whence, off_t len) {
    struct flock lock;
    lock.l_type = type; //F_RDLK, F_WRLK
    lock.l_whence = whence;
    lock.l_start = offset;
    lock.l_len = len;

    if (fcntl(fd, F_GETLK, &lock) < 0) {
        puts("fcntl error");
        exit(0);
    }
    if (lock.l_type == F_ULOCK)
        return 0; //false, region isn't locked by another process
    return (lock.l_pid); //true, return pid
}