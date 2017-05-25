#include <unistd.h>
#include <stdio.h>

int main(void) {
    if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1)
        puts("Cannot seek");
    else
        puts("Seek ok");
    return 0;
}