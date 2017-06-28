#include <stdio.h>
#include <string.h>
#include <sys/socket.h>


extern ssize_t writen(int fd, void *ptr, size_t n);

int send_err(int fd, int errcode, const char *msg) {
    int n;
    if ((n = strlen(msg)) > 0)
        if (writen(fd, msg, n) != n)
            return -1;
    if (errcode >= 0)
        errcode = -1; /* must be negative */
    if (send_fd(fd, errcode) < 0)
        return -1;
    return 0;
}