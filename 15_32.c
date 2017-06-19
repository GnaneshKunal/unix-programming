#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <unistd.h>

union sem {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main(void) {
    int i, j;
    pid_t pid;
    int semid;
    key_t key = 1234;
    int semflg = IPC_CREAT | 0666;
    int nsems = 1;
    int nops; // no of operations to do
    struct sembuf *sops = (struct sembuf *)malloc(2 * sizeof(struct sembuf));
    // ptr to operations to perform

    // setup a semaphore

    if ((semid = semget(key, nsems, semflg)) == -1) {
        perror("semget: semget failed");
        exit(1);
    }
    puts("semget: succeded");

    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // child
        i = 0;
        while (i < 3) {
            // allow for 3 semaphore sets
            nops = 2;
            // wait for semaphore to reach zero
            sops[0].sem_num = 0; // we only use one track
            sops[0].sem_op = 0; // wait for semaphore flag to become zero
            sops[0].sem_flg = SEM_UNDO; // take off semaphore asynchronous

            sops[1].sem_num = 0;
            sops[1].sem_op = 1; // increment semaphore -- take control of track
            sops[1].sem_flg = SEM_UNDO | IPC_NOWAIT; // take of semaphore

            puts("calling semop");
            for (j = 0; j < nops; j++) {
                fprintf(stdout, "\n\tsemops[%d].sem_num = %d ", j, sops[j].sem_num);
                fprintf(stdout, "sem_op = %d, ", sops[j].sem_op);
                fprintf(stdout, "sem_flg = %#o\n", sops[j].sem_flg);
            }
            if ((j = semop(semid, sops, nops)) == -1) {
                perror("semop: semop failed");
                exit(1);
            } else {
                printf("\tsemop returned %d\n", j);
                printf("Child process taking controll of tracj: %d/3 times\n", i + 1);
                sleep(5);
                nops = 1;

                sops[0].sem_num = 0;
                sops[0].sem_op = -1; // Give up control of track
                sops[0].sem_flg = SEM_UNDO | IPC_NOWAIT; // take of semaphore, asynchronous

                if ((j = semop(semid, sops, nops)) == -1) {
                    perror("semop failed");
                    exit(1);
                } else
                    printf("child giving up control of track %d/3\n", i + 1);
                sleep(5);
            }
            i++;
        }
    } else { /* pid hold id of child */

        i = 0;


        while (i < 3) { /* allow for 3 semaphore sets */

            nops = 2;

            /* wait for semaphore to reach zero */
            sops[0].sem_num = 0;
            sops[0].sem_op = 0; /* wait for semaphore flag to become zero */
            sops[0].sem_flg = SEM_UNDO; /* take off semaphore asynchronous  */


            sops[1].sem_num = 0;
            sops[1].sem_op = 1; /* increment semaphore -- take control of track */
            sops[1].sem_flg = SEM_UNDO | IPC_NOWAIT; /* take off semaphore */

            /* Recap the call to be made. */

            (void)fprintf(stderr, "\nsemop:Parent Calling semop(%d, &sops, %d) with:", semid, nops);
            for (j = 0; j < nops; j++) {
                fprintf(stderr, "\n\tsops[%d].sem_num = %d, ", j, sops[j].sem_num);
                fprintf(stderr, "sem_op = %d, ", sops[j].sem_op);
                fprintf(stderr, "sem_flg = %#o\n", sops[j].sem_flg);
            }

            /* Make the semop() call and report the results. */
            if ((j = semop(semid, sops, nops)) == -1) {
                perror("semop: semop failed");
            } else {
                fprintf(stderr, "semop: semop returned %d\n", j);

                fprintf(stderr, "Parent Process Taking Control of Track: %d/3 times\n", i + 1);
                sleep(5); /* Do nothing for 5 seconds */

                nops = 1;

                /* wait for semaphore to reach zero */
                sops[0].sem_num = 0;
                sops[0].sem_op = -1; /* Give UP COntrol of track */
                sops[0].sem_flg = SEM_UNDO | IPC_NOWAIT; /* take off semaphore, asynchronous  */

                if ((j = semop(semid, sops, nops)) == -1) {
                    perror("semop: semop failed");
                } else
                    fprintf(stderr, "Parent Process Giving up Control of Track: %d/3 times\n", i + 1);
                sleep(5); /* halt process to allow child to catch semaphor change first */
            }
            ++i;
        }
    }
}