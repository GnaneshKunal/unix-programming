#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * thr_fn1(void *arg) {
    puts("thread 1 returning");
    return (void *)0;
}

void * thr_fn2(void *arg) {
    puts("thread 2 exiting");
    pthread_exit((void *)2);
}

int main(void) {
    int err;
    pthread_t tid1, tid2;
    void *tret;

    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    if (err != 0) {
        printf("%d %s", err, "can't create thread1\n");
        exit(0);
    }
    err = pthread_create(&tid2, NULL, thr_fn2, NULL);
    if (err != 0) {
        printf("%d %s", err, "can't create thread2\n");
        exit(0);
    }
    err = pthread_join(tid1, &tret);
    if (err != 0) {
        printf("%d %s", err, "can't join with thread1\n");
        exit(0);
    }
    printf("thread 1 exit code %ld\n", (long)tret);
    err = pthread_join(tid2, &tret);
    if (err != 0) {
        printf("%d %s", err, "can't join with thread 2\n");
        exit(0);
    }
    printf("thread 2 exit code %ld\n", (long) tret);
    exit(0);
}