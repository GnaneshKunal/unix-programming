#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define COPYINCR (1024*1024*1024) /* 1 GB */
#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

int main(int argc, char **argv) {
    int fdin, fdout;
    void *src, *dst;
    size_t copysz;
    struct stat sbuf;
    off_t fsz = 0;

    if (argc != 3) {
        printf("usage: %s <fromfile> <tofile>\n", argv[0]);
        exit(1);
    }

    if ((fdin = open(argv[1], O_RDONLY)) < 0) {
        printf("can't open %s for reading\n", argv[1]);
        exit(1);
    }

    if ((fdout = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, FILE_MODE)) < 0) {
        printf("can't creat %s for writing\n", argv[2]);
        exit(1);
    }

    if (fstat(fdin, &sbuf) < 0) {
        puts("fstat error");
        exit(1);
    }
    if (ftruncate(fdout, sbuf.st_size) < 0) {
        puts("ftruncate error");
        exit(1);
    }
    printf("%ld", sbuf.st_size);
    while (fsz < sbuf.st_size) {
        if ((sbuf.st_size - fsz) > COPYINCR)
            copysz = COPYINCR;
        else
            copysz = sbuf.st_size - fsz;

        if ((src = mmap(0, copysz, PROT_READ, MAP_SHARED, fdin, fsz)) == MAP_FAILED) {
            puts("mmap error for input");
            exit(1);
        }
        if ((dst = mmap(0, copysz, PROT_READ|PROT_WRITE, MAP_SHARED, fdout, fsz)) == MAP_FAILED) {
            puts("mmap error for output");
            exit(1);
        }
        memcpy(dst, src, copysz);
        munmap(src, copysz);
        munmap(dst, copysz);
        fsz += copysz;
    }
    exit(0);
}