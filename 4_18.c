#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    if (symlink("/home/monster/git/unix-programming/foo/testdir", "/home/monster/git/unix-programming/sl") < 0) {
        perror("Error");
        exit(1);
    }
    write(STDOUT_FILENO, "DONE", strlen("DONE"));
    exit(0);
}