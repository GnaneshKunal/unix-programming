/*
    * Not part of any standard 

    * Useful for Pipes, FIFOs, and some devices—notably terminals and networks have 
      the following two properties.
        1. A read operation may return less than asked for, even though we have not
        encountered the end of file. This is not an error, and we should simply continue
        reading from the device.
        2. A write operation can return less than we specified. This may be caused by
        kernel output buffers becoming full, for example. Again, it’s not an error, and
        we should continue writing the remainder of the data. (Normally, this short
        return from a write occurs only with a nonblocking descriptor or if a signal is
        caught.)
*/
#include <stdio.h>
#include <unistd.h>

ssize_t readn(int fd, void *ptr, size_t n) {
    size_t nleft;
    ssize_t nread;

    nleft = n;

    while(nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (nleft == n)
                return -1; // error, return -1
            else
                break; //error, return amount read so fat
        } else if (nread == 0)
            break; //EOF
        nleft -= nread;
        ptr += nread;
    }
    return (n - nleft); //return >= 0
}

ssize_t writen(int fd, void *ptr, size_t n) {
    size_t nleft;
    ssize_t nwritten;

    nleft = n;
    while(nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) < 0) {
            if (nleft == n)
                return -1; //error, return -1
            else
                break; //error, return amount written so far
        } else if (nwritten == 0)
            break;
        nleft -= nwritten;
        ptr += nwritten;
    }
    return (n - nleft); //return >= 0
}