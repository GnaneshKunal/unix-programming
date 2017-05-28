#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

int main(void) {
    umask(0);
    char buf[2048];
    if (creat("foo", RWRWRW) < 0) {
        strcpy(buf, "error for foo");
        write(STDOUT_FILENO, buf, strlen(buf));
        exit(1);
    }
    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (creat("bar", RWRWRW) < 0) {
        strcpy(buf, "error for bar");
        write(STDOUT_FILENO, buf, strlen(buf));
        exit(1);
    }
    exit(0);
}