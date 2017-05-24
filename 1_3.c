#include <stdio.h> // IO
#include <stdlib.h> //exit
#include <string.h> // for strlen
#include <stdarg.h> //variable list args
#include <dirent.h> //directory information
#define MAX_LINE 4096 //MAX_LINE

void err_doit(int errnoflag, int error, const char *fmt, va_list ap);
void err_quit(const char *fmt, ...);
void err_sys(const char *fmt, ...);

int main(int argc, char **argv) {
    DIR *dp;
    struct dirent *dirp;
    
    if (argc != 2)
        err_quit("usage: ls directory_name");

    if ((dp = opendir(argv[1])) == NULL)
        err_sys("can't open %s", argv[1]);
    while ((dirp = readdir(dp)) != NULL)
        printf("%s\n", dirp->d_name);

    closedir(dp);
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