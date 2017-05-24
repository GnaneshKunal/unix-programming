#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <errno.h>

#define MAXLINE 4096
#define MAX_LINE 4096
void err_doit(int errnoflag, int error, const char *fmt, va_list ap);
void err_quit(const char *fmt, ...);
void err_sys(const char *fmt, ...);
void err_ret(const char *fmt, ...);

int main(void) {
    char buf[MAXLINE];
    pid_t pid;
    int status;

    printf("%% ");
    while(fgets(buf, MAXLINE, stdin) != NULL) {
        if (buf[strlen(buf) -1] == '\n')
            buf[strlen(buf) -1] = 0; //replace newline with null
        if ((pid = fork()) < 0)
            puts("fork error");
        else if (pid == 0) {
            execlp(buf, buf, (char *) 0);
            err_ret("Couldn't execute: %s", buf);
            exit(127);
        }
        if ((pid = waitpid(pid, &status, 0)) < 0) {
            puts("waitpid error");
            exit(1);
        }
        printf("%% ");
    }
    exit(0);
}

void
err_ret(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
}

void err_doit(int errnoflag, int error, const char *fmt, va_list ap) {
    char buf[MAX_LINE];
    vsnprintf(buf,
        MAX_LINE - 1, fmt, ap); //Write formatted data from variable argument list to sized buffer
    if (errnoflag)
        snprintf(buf + strlen(buf), MAX_LINE + strlen(buf) -1, " : %s", strerror(error)); 
        // strerror() Get pointer to error message string
        strcat(buf, "\n"); //end of string
        fflush(stdout); //Flush stream
        fputs(buf, stderr);
        fflush(NULL);
}

void err_quit(const char *fmt, ...) {
    va_list ap; //variable list arguments
    va_start(ap, fmt);
    err_doit(0, 0, fmt, ap);
    va_end(ap);
    exit(1);
}

void err_sys(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    err_doit(0, 0, fmt, ap);
    va_end(ap);
    exit(1);
}