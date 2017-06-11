#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void prepare(void) {
    int err;

    puts("Preparing locks...");
    if ((err = pthread_mutex_lock(&lock1)) != 0) {
        puts("can't lock lock1 in prepare handler");
        exit(1);
    }
    if ((err = pthread_mutex_lock(&lock2)) != 0) {
        puts("can't lock lock2 in prepare handler");
        exit(1);
    }
}

void parent(void) {
    int err;

    puts("parent unlocking locks...");
    if ((err = pthread_mutex_unlock(&lock1)) != 0) {
        puts("can't unlock lock1 in parent handler");
        exit(1);
    }
    if ((err = pthread_mutex_unlock(&lock2)) != 0) {
        puts("can't unlock lock2 in parent handler");
        exit(1);
    }
}

void child(void) {
    int err;
    
    puts("child unlocking locks...");
    if ((err = pthread_mutex_unlock(&lock1)) != 0) {
        puts("can't unlock lock1 in child handler");
        exit(1);
    }
    if ((err = pthread_mutex_unlock(&lock2)) != 0) {
        puts("can't unlock lock2 in child handler");
        exit(1);
    }
}

void *thr_fn(void *arg) {
    puts("thread started...");
    pause();
    return 0;
}

int main(void) {
    int err;
    pid_t pid;
    pthread_t tid;

    if ((err = pthread_atfork(prepare, parent, child)) != 0) {
        puts("can't install fork handlers");
        exit(1);
    }
    if ((err = pthread_create(&tid, NULL, thr_fn, 0)) != 0) {
        puts("can't create thread");
        exit(1);
    }
    sleep(2);
    puts("parent about to fork...");

    if ((pid = fork()) < 0) {
        puts("fork failed");
        exit(1);
    } else if (pid == 0)
        puts("child returned from fork");
    else
        puts("parent returned from fork");
    exit(0);
}