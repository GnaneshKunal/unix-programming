#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>

#define FILE_MODE (S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

void print_and_exit(char * str) {
    puts(str);
    exit(1);
}

static volatile sig_atomic_t sigflag;
static sigset_t zeromask, oldmask, newmask;

static void sig_fn(int signo) {
    sigflag = 1;
}

void TELL_WAIT(void);
void WAIT_PARENT(void);
void TELL_CHILD(pid_t pid);
int lock_reg(int, int, int, off_t, int, off_t);
void set_fl(int, int);
#define write_lock(fd, offset, whence, len) \
        lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define read_lock(fd, offset, whence, len) \
        lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))

int main(int argc, char **argv) {
    int fd;
    pid_t pid;
    char buf[5];
    struct stat statbuf;

    if (argc != 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        exit(1);
    }

    if ((fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0) 
        print_and_exit("open error");

    if (write(fd, "abcdef", 6) != 6) 
        print_and_exit("write error");
    
    /* turn on set-group-ID and turn off group-execute */

    if (fstat(fd, &statbuf) < 0) 
        print_and_exit("fstat error");

    if(fchmod(fd, (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
        print_and_exit("fchmod error");
    TELL_WAIT();

    if ((pid = fork()) < 0)
        print_and_exit("fork error");
    else if (pid > 0) { //parent
    /* write lock entire file */
        if (write_lock(fd, 0, SEEK_SET, 0) < 0)
            print_and_exit("write_lock_error");
        
        TELL_CHILD(pid);

        if (waitpid(pid, NULL, 0) < 0)
            print_and_exit("waitpid error");
    } else {
        WAIT_PARENT(); //wait for parent to set lock

        set_fl(fd, O_NONBLOCK);

        /* first let’s see what error we get if region is locked */

        if (read_lock(fd, 0, SEEK_SET, 0) != -1)
            print_and_exit("read_lock succeeded");
        printf("read_lock of already-locked region return %d\n", errno);

        //now try to read the mandatory locked file

        if (lseek(fd, 0, SEEK_SET) == -1)
            print_and_exit("lseek error");
        if (read(fd, buf, 2) < 0)
            print_and_exit("read failed (mandatory locking works");
        else
            printf("read OK (no mandatory locking), buf, = %2.2s\n", buf);
    }
    exit(0);
}


int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len) {
    struct flock lock; 
    lock.l_type = type; //F_RWLOCK, F_WRLOCK, F_UNLCK
    lock.l_whence = whence; //byte offset, relative to l_whence
    lock.l_start = offset; //SEEK_SET, SEEK_CUR, SEEK_END
    lock.l_len = len; //bytes(0 means to EOF)

    return (fcntl(fd, cmd, &lock));
}

void TELL_WAIT(void) {
    if(signal(SIGUSR1, sig_fn) == SIG_ERR) {
        puts("signal SIGUSR1 error");
        exit(1);
    }

    if (signal(SIGUSR2, sig_fn) == SIG_ERR) {
        puts("signal SIGUSR2 error");
        exit(1);
    }

    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2);

    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
        puts("SIG_BLOCK error");
        exit(1);
    }
}

void WAIT_PARENT(void) {
    while (sigflag == 0)
        sigsuspend(&zeromask);
    sigflag = 0;

    if (sigprocmask(SIG_SETMASK, &oldmask, 0) < 0) {
        puts("sigprocmask error");
        exit(1);
    }
}

void WAIT_CHILD(void) {
    while (sigflag == 0)
        sigsuspend(&zeromask);
    sigflag = 0;

    if(sigprocmask(SIG_SETMASK, &oldmask, 0) < 0) {
        puts("sigprocmask error");
        exit(1);
    }
}

void TELL_PARENT(pid_t pid) {
    kill(pid, SIGUSR2);
}

void TELL_CHILD(pid_t pid) {
    kill(pid, SIGUSR1);
}

void set_fl(int fd, int flags) {
    int val;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
        print_and_exit("fcntl error");
    
    val |= flags;

    if ((val = fcntl(fd, F_SETFL, val)) < 0)
        print_and_exit("fcntl error");
}