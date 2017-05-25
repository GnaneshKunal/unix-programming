#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
/* Read by owner.  */
/* Write by owner.  */
/* Read by group.  */
/* Write by group.  */

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main(void) {
    int fd;
    if ((fd = creat("file.hole", FILE_MODE)) < 0) {
        puts("creat error");
        return EXIT_FAILURE;
    }
    if (write(fd, buf1, 10) != 10){
        puts("buf1 write error");
        return EXIT_FAILURE;
    }
    if (lseek(fd, 16384, SEEK_SET) == -1) {
        puts("lseek error");
        return EXIT_FAILURE;
    }
    if (write(fd, buf2, 10) != 10) {
        puts("buf2 write error");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}