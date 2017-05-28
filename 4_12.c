#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

int main(void) {
    struct stat statbuf;
    char *buf = malloc(200 * sizeof(char));
    if (stat("foo", &statbuf) < 0) {
        strcpy(buf, "stat error for foo");
        write(STDOUT_FILENO, buf, strlen(buf));
        exit(1);
    }
    if (chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0) {
        strcpy(buf, "chmod error for foo");
        write(STDOUT_FILENO, buf, strlen(buf));
        exit(1);
    }
    if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH) < 0) {
        strcpy(buf, "chmod error for bar");
        write(STDOUT_FILENO, buf, strlen(buf));
        exit(1);
    }
    free(buf);
    exit(0);
}