#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <aio.h>
#include <errno.h>
#include <sys/stat.h>

#define BSZ 4096
#define NBUF 8
#define FILE_MODE (S_IRUSR|S_IRUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

enum rwop {
    UNUSED = 0,
    READ_PENDING = 1,
    WRITE_PENDING = 2
};

struct buf {
    enum rwop op;
    int last;
    struct aiocb aiocb;
    unsigned char data[BSZ];
};

struct buf bufs[NBUF];

unsigned char translate (unsigned char c) {
    if (isalpha(c)) {
        if (c >= 'n')
            c -= 13;
        else if (c >= 'a')
            c += 13;
        else if (c >= 'N')
            c -= 13;
        else
            c += 13;
    }
    return c;
}

int main(int argc, char **argv) {
    int ifd, ofd, i, j, n, err, numop;
    struct stat sbuf;
    const struct aiocb *aiolist[NBUF];
    off_t off = 0;

    if (argc != 3) {
        puts("usage: a.out infile outfile");
        exit(1);
    }

    if ((ifd = open(argv[1], O_RDONLY)) < 0) {
        puts("can't open infile");
        exit(1);
    }

    if ((ofd = open(argv[2], O_CREAT|O_TRUNC|O_RDWR, FILE_MODE)) < 0) {
        puts("can't create outfile");
        exit(1);
    }

    if (fstat(ifd, &sbuf) < 0) {
        puts("fstat failed");
        exit(1);
    }

    for (i = 0; i < NBUF; i++) {
        bufs[i].op = UNUSED;
        bufs[i].aiocb.aio_buf = bufs[i].data;
        bufs[i].aiocb.aio_sigevent.sigev_notify = SIGEV_NONE;
        aiolist[i] = NULL;
    }

    numop = 0;
    for(;;) {
        for (i = 0; i < NBUF; i++) {
            switch(bufs[i].op) {
                case UNUSED:
                //read from the input file if more date remains unread
                    if (off < sbuf.st_size) {
                        bufs[i].op = READ_PENDING;
                        bufs[i].aiocb.aio_fildes = ifd;
                        bufs[i].aiocb.aio_offset = off;
                        off += BSZ;
                        if (off < sbuf.st_size)
                            bufs[i].last = 1;
                        bufs[i].aiocb.aio_nbytes = BSZ;
                        if (aio_read(&bufs[i].aiocb) < 0) {
                            puts("aio_read failed");
                            exit(1);
                        }
                        aiolist[i] = &bufs[i].aiocb;
                        numop++;
                    }
                    break;
                case READ_PENDING:
                    if ((err = aio_error(&bufs[i].aiocb)) == EINPROGRESS)
                        continue;
                    if (err != 0) {
                        if (err == -1) {
                            puts("aio_error failed");
                            exit(1);
                        } else {
                            puts("err, read failed");
                            exit(1);
                        }
                    }
                    //A read is complete; translate the buffer
                    if ((n = aio_return(&bufs[i].aiocb)) < 0) {
                        puts("aio_return failed");
                        exit(1);
                    }
                    if (n != BSZ && !bufs[i].last) {
                        printf("short read (%d/%d)", n, BSZ);
                        exit(1);
                    }
                    for (j = 0; j < n; j++)
                        bufs[i].data[j] = translate(bufs[i].data[j]);
                    bufs[i].op = WRITE_PENDING;
                    bufs[i].aiocb.aio_fildes = ofd;
                    bufs[i].aiocb.aio_nbytes = n;
                    if (aio_write(&bufs[i].aiocb) < 0) {
                        puts("aio_write failed");
                        exit(1);
                    }
                    //retain our spot in aiolist
                    break;
                case WRITE_PENDING:
                    if ((err = aio_error(&bufs[i].aiocb)) == EINPROGRESS)
                        continue;
                    if (err != 0) {
                        if (err == -1) {
                            puts("aio_error failed");
                            exit(1);
                        } else {
                            puts("write failed");
                            exit(1);
                        }
                    }
                    //A write is complete; mark the buffer unused
                    if ((n = aio_return(&bufs[i].aiocb)) < 0) {
                        puts("aio_return error");
                        exit(1);
                    }
                    if (n != bufs[i].aiocb.aio_nbytes) {
                        printf("short write (%d/%d)", n, BSZ);
                        exit(1);
                    }
                    aiolist[i] = NULL;
                    bufs[i].op = UNUSED;
                    numop--;
                    break;
            }
        }
        if (numop == 0) {
            if (off > sbuf.st_size)
                break;
        } else
            if (aio_suspend(aiolist, NBUF, NULL) < 0) {
                puts("aio_suspend failed");
                exit(1);
            }
    }
    bufs[0].aiocb.aio_fildes = ofd;
    if (aio_fsync(O_SYNC, &bufs[0].aiocb) < 0) {
        puts("aio_fsync failed");
        exit(1);
    }
    exit(0);
}