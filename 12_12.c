#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

extern char **environ;

char buf[4096];

pthread_mutex_t env_mutex;
pthread_t ntid1, ntid2;

static pthread_once_t init_done = PTHREAD_ONCE_INIT;

static void thread_init(void) {
    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&env_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

int getenv_r(const char *name, char *buf, int buflen) {
    int i, len, olen;

    pthread_once(&init_done, thread_init);
    len = strlen(name);
    pthread_mutex_lock(&env_mutex);
    for (i = 0; environ[i] != NULL; i++)
        if ((strncmp(name, environ[i], len) == 0) && (environ[i][len] == '=')) {
            olen = strlen(&environ[i][len+1]);
            if (olen >= buflen) {
                pthread_mutex_unlock(&env_mutex);
                return ENOSPC;
            }
            strcpy(buf, &environ[i][len+1]);
            pthread_mutex_unlock(&env_mutex);
            return 0;
        }
    pthread_mutex_unlock(&env_mutex);
    return ENOENT;
}

void * thr_fn1(void *arg) {
    puts("here");
    pthread_t tid;
    tid = pthread_self();

    printf("My thread1 Id: is %lu\n", (unsigned long)tid);
    getenv_r("USER", buf, 4096);
    puts(buf);
    return (void *)0;
}
void * thr_fn2(void *arg) {
    pthread_t tid;
    tid = pthread_self();

    printf("My thread2 Id: is %lu\n", (unsigned long)tid);
    getenv_r("HOME", buf, 4096);
    puts(buf);
    return (void *)0;
}

int main(void) {
    int err;

    err = pthread_create(&ntid1, NULL, thr_fn1, NULL);
    if (err != 0)
        puts("error");
    err = pthread_create(&ntid2, NULL, thr_fn2, NULL);
    sleep(1);
    puts(buf);
    exit(0);
}