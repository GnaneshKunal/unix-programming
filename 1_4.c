#include <stdio.h>
#include <unistd.h> //contains file descriptor for stdin, stdout
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#define MAX_LINE 4096
#define BUFFSIZE 4096

void err_doit(int errnoflag, int error, const char *fmt, va_list ap);
void err_sys(const char *fmt, ...);

int main(void) {
    int n;
    char buf[BUFFSIZE];
    // STDIN_FILENO - file descriptor for stdin
    // STDOUT_FILENO - file descriptor for stdout
    while((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
        if (write(STDOUT_FILENO, buf, n) != n)
            err_sys("write error");

        if (n < 0)
            err_sys("read error");
        exit(0);
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

void err_sys(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    err_doit(0, 0, fmt, ap);
    va_end(ap);
    exit(1);
}