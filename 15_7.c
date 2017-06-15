#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

static int pfd1[2], pfd2[2];

void TELL_WAIT(void) {
    puts("pipe");
    if (pipe(pfd1) < 0 || pipe(pfd2) < 0) {
        puts("pipe error");
        exit(1);
    }
}

void TELL_PARENT(pid_t pid) {
    puts("tell par");
    if (write(pfd2[1], "c", 1) != 1) {
        puts("write error");
        exit(1);
    }
}

void WAIT_PARENT(void) {
    puts("wait par");
    char c;
    if (read(pfd1[0], &c, 1) != 1) {
        puts("read error");
        exit(1);
    }

    if (c != 'p') {
        puts("WAIT_PARENT: incorrect data");
        exit(0);
    }
}

void TELL_CHILD(pid_t pid) {
    puts("tell chi");

    if (write(pfd1[1], "p", 1) != 1) {
        puts("write error");
        exit(1);
    }
}

void WAIT_CHILD(void) {
    puts("wait chi");
    char c;

    if (read(pfd2[0], &c, 1) != 1) {
        puts("read error");
        exit(1);
    }

    if (c != 'c') {
        puts("WAIT_CHILD: incorrect data");
        exit(0);
    }
}

int main(void) {
    pid_t pid;

    TELL_WAIT();
    if ((pid = fork()) < 0) {
        puts("fork error");
        exit(1);
    } else if (pid > 0) {
        sleep(1);
        TELL_CHILD(pid);
        WAIT_PARENT();

        if (waitpid(pid, NULL, 0) < 0) {
            puts("waitpid error");
            exit(1);
        }
    } else {
        TELL_PARENT(pid);
        WAIT_CHILD();
    }
    exit(0);
}