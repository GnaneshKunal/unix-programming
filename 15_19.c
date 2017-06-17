#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    struct msgbuf sbuf;
    size_t buflen;

    key = 1234;

    if ((msqid = msgget(key, msgflg)) < 0) // get the message
        die("msgget");

    sbuf.mtype = 1; // message type

    printf("Enter a message to add to message queue: ");
    scanf("%[^\n]", sbuf.mtext);

    buflen = strlen(sbuf.mtext) + 1;

    if (msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0) {
        printf("%d, %ld, %s, %ld\n", msqid, sbuf.mtype, sbuf.mtext, buflen);
        die("msgsnd");
    } else
        printf("Message sent\n");

    exit(0);
}