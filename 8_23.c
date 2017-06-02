#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {
    int status;
    if ((status = system("date")) < 0) {
        puts("system() error");
        exit(1);
    }
    printf("%d\n", status);

    if ((status = system("nosuchcommand")) < 0) {
        puts("system() error");
        exit(1);
    }
    printf("%d\n", status);
    
    if ((status = system("who; exit 44")) < 0) {
        puts("system() error");
        exit(1);
    }
    printf("%d\n", status);

    exit(0);
}