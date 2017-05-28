#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int main(void) {
    char * buf = malloc(200 * sizeof(char));
    if (open("tempfile", O_RDWR) < 0) {
        perror("Error");
        strcpy(buf, "open error");
        write(STDOUT_FILENO, buf, strlen(buf));
        exit(1);
    }
    if (unlink("tempfile") < 0) {
        strcpy(buf, "unlink error");
        write(STDOUT_FILENO, buf, strlen(buf));
        exit(1);
    }
    write(STDOUT_FILENO, "\nfile unlinked\n", strlen("\nfile unlinked\n"));
    sleep(15);
    write(STDOUT_FILENO, "done\n", strlen("done\n"));
    free(buf);
    exit(0);
}