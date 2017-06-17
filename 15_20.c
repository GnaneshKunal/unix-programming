#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

#define MAXSIZE 128

void die(char *s) {
    perror(s);
    exit(1);
}

struct msgbuf {
    long mtype;
    char mtext[MAXSIZE];
};

int main(void) {
    int msqid;
    key_t key;
    struct msgbuf rcvbuffer;

    key = 1234;

    if ((msqid = msgget(key, 0666)) < 0)
        die("msgget()");

    if (msgrcv(msqid, &rcvbuffer, MAXSIZE, 1, 0) < 0)
        die("msgrcv");

    printf("%s\n", rcvbuffer.mtext);
    exit(0);
}