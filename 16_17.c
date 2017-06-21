#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>

#define BUFLEN 128
#define QLEN 10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

int initserver(int, const struct sockaddr *, socklen_t, int);
void serve(int);
int set_cloexec(int);
void daemonize(const char *);

int main(int argc, char **argv) {
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    int sockfd, err, n;
    char *host;

    if (argc != 1) {
        puts("usage ruptimed");
        exit(1);
    }

    if ((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
        n = HOST_NAME_MAX; // best guess
    if ((host = malloc(n)) == NULL) {
        puts("malloc error");
        exit(1);
    }
    if (gethostname(host, n) < 0) {
        puts("gethostname error");
        exit(1);
    }
    daemonize("ruptime");
    memset(&hint, 0, sizeof(hint));
    hint.ai_flags = AI_CANONNAME;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;
    if ((err = getaddrinfo(host, "ruptime", &hint, &ailist)) != 0) {
        syslog(LOG_ERR, "ruptimed: getaddrinfo error: %s", gai_strerror(err));
        exit(1);
    }
    for (aip = ailist; aip != NULL; aip = aip->ai_next) {
        if ((sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, QLEN)) >= 0) {
            serve(sockfd);
            exit(0);
        }
    }
    exit(1);
}

void serve(int sockfd) {
    int clfd;
    FILE *fp;
    char buf[BUFLEN];
    set_cloexec(sockfd);
    for (;;) {
        if ((clfd = accept(sockfd, NULL, NULL)) < 0) {
            syslog(LOG_ERR, "ruptime: accept error: %s", strerror(errno));
            exit(1);
        }
        set_cloexec(clfd);
        if ((fp = popen("/usr/bin/uptime", "r")) == NULL) {
            sprintf(buf, "errno: %s\n", strerror(errno));
            send(clfd, buf, strlen(buf), 0);
        } else {
            while (fgets(buf, BUFLEN, fp) != NULL)
                send(clfd, buf, strlen(buf), 0);
            pclose(fp);
        }
        close(clfd);
    }
}

int set_cloexec(int fd) {
    int val;

    if ((val = fcntl(fd, F_GETFD, 0)) < 0)
        return (-1);

    val |= FD_CLOEXEC; /* enable close-on-exec */

    return (fcntl(fd, F_SETFD, val));
}


int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen) {
    int fd;
    int err = 0;

    if ((fd = socket(addr->sa_family, type, 0)) < 0)
        return -1;
    if (bind(fd, addr, alen) < 0)
        goto errout;
    if (type == SOCK_STREAM || type == SOCK_SEQPACKET)
        if (listen(fd, qlen) < 0)
            goto errout;

errout:
    err = errno;
    close(fd);
    errno = err;
    return -1;
}

void daemonize(const char *cmd) {
    int i, fd0, fd1, fd2;
    pid_t pid;

    struct rlimit r1;
    struct sigaction sa;

    // clear file creation mask
    umask(0);
    // get max number of file descriptors

    if (getrlimit(RLIMIT_NOFILE, &r1) < 0) {
        printf("%s: can't get file limit\n", cmd);
        exit(1);
    }

    // become session leader to close controlling TTY
    if ((pid = fork()) < 0) {
        printf("%s: can't fork", cmd);
        exit(1);
    } else if (pid != 0)
        exit(0); // parent
    setsid();

    // ensure future opens won't allocate controlling TTY

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        printf("%s: can't ignore SIGHUP", cmd);
        exit(1);
    }
    if ((pid = fork()) < 0) {
        printf("%s: can't fork", cmd);
        exit(1);
    } else if (pid != 0)
        exit(0);

    // change current working directory to the root
    // so we won't prevent file system from being unmounted

    if (chdir("/") < 0) {
        printf("%s: can't change directory", cmd);
        exit(1);
    }
    // close all file descriptors

    if (r1.rlim_max == RLIM_INFINITY)
        r1.rlim_max = 1024;
    for (i = 0; i < r1.rlim_max; i++)
        close(i);

    // attach file descriptors 0, 1, 2 to /dev/null

    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    // Initialize the log file.
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "Unexpected file descriptors %d %d %d", fd0, fd1, fd2);
        exit(1);
    }
}