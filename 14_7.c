#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>

int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len);

#define FILE_MODE (S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)
#define writew_lock(fd, offset, whence, len) lock_reg(fd, F_SETLK, F_WRLCK, offset, whence, len)

static sigset_t oldmask, newmask, zeromask;
static volatile sig_atomic_t sigflag;

static void sig_fn(int signo) {
    sigflag = 1;
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

static void lockabyte(const char *name, int fd, off_t offset) {
    if (writew_lock(fd, offset, SEEK_SET, 1) < 0) {
        puts("write_lock error");
        exit(1);
    }
    printf("%s: got the lock, byte %lld\n", name, (long long)offset);
}

int main(void) {
    int fd;
    pid_t pid;

    //create a file and write two bytes to it

    if ((fd = creat("templock", FILE_MODE)) < 0) {
        puts("creat error");
        exit(1);
    }
    if (write(fd, "ab", 2) != 2) {
        puts("write error");
        exit(1);
    }

    TELL_WAIT();

    if ((pid = fork()) < 0) {
        puts("fork error");
        exit(1);
    } else if (pid == 0) {
        lockabyte("child", fd, 0);
        TELL_PARENT(getppid());
        WAIT_PARENT();
        lockabyte("child", fd, 1);
    } else {
        lockabyte("parent", fd, 1);
        TELL_CHILD(pid);
        WAIT_CHILD();
        lockabyte("parent", fd, 0);
    }
    exit(0);
}