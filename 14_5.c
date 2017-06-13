#include <fcntl.h>

int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len) {
    struct flock lock; 
    lock.l_type = type; //F_RWLOCK, F_WRLOCK, F_UNLCK
    lock.l_whence = whence; //byte offset, relative to l_whence
    lock.l_start = offset; //SEEK_SET, SEEK_CUR, SEEK_END
    lock.l_len = len; //bytes(0 means to EOF)

    return (fcntl(fd, cmd, &lock));
}

/*
    int fcntl(int fd, int cmd, struct flock *flockptr );
    struct flock {
        short l_type;
        short l_whence;
        off_t l_start;
        off_t l_len;
        pid_t l_pid;
    };
*/