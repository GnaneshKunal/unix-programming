#include <stdio.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

static void my_alarm(int signo) {
    struct passwd *rootptr;

    puts("In a signal handler");
    if ((rootptr = getpwnam("root")) == NULL) {
        puts("getpwnam(root) error");
        exit(1);
    }
    alarm(1);
}


int main(void) {
    struct passwd *ptr;
    signal(SIGALRM, my_alarm);
    alarm(1);
    for (;;) {
        if ((ptr = getpwnam("monster")) == NULL) {
            puts("getpwnam error");
            exit(1);
        }
        
        if (strcmp(ptr->pw_name, "monster") != 0)
            printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
    }
}