#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

#define ARRAY_SIZE 40000
#define MALLOC_SIZE 100000
#define SHM_SIZE 100000
#define SHM_MODE 0600 // user read/write

char array[ARRAY_SIZE]; // uninitialized data = bss

int main(void) {
    int shmid;
    char *ptr, *shmptr;
    struct shmid_ds shmbuf;

    printf("array[] from %p to %p\n", (void *)&array[0], (void *)&array[ARRAY_SIZE]);
    printf("stack around %p\n", (void *)&shmid);

    if ((ptr = malloc(MALLOC_SIZE)) == NULL) {
        puts("malloc error");
        exit(1);
    }

    printf("malloced from %p to %p\n", (void *)ptr, (void *)ptr + MALLOC_SIZE);

    if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0) {
        puts("shmget error");
        exit(1);
    }
    printf("key: %d\n", shmid);
    if ((shmptr = shmat(shmid, 0, 0)) == (void *)-1) {
        puts("shmat error");
        exit(1);
    }
    printf("shared memory attached from %p to %p\n", (void *)shmptr, (void *)shmptr + SHM_SIZE);

    if (shmctl(shmid, IPC_STAT, &shmbuf) < 0) {
        puts("shmctl error");
        exit(1);
    }

    printf("size: %ld\ncpid: %d\nattach-time: %ld\n", shmbuf.shm_segsz, shmbuf.shm_cpid, shmbuf.shm_atime);

    if (shmctl(shmid, IPC_RMID, 0) < 0) {
        puts("shmctl error");
        exit(1);
    }
    exit(0);
}