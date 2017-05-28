#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(void) {
    char *ptr;
    size_t size = 300;

    if(chdir("/home/monster/git/unix-programming/foo") < 0) {
        perror("error");
        exit(1);
    }
    ptr = malloc(size * sizeof(char));
    if (getcwd(ptr, size) == NULL) {
        perror("error");
        exit(1);
    }
    printf("cwd = %s\n", ptr);
    free(ptr);
    exit(1);
}