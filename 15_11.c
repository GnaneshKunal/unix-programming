#include <stdio.h>
#include <stdlib.h>

#define PAGER "${PAGER:-more}" // environment variable by default
#define MAXLINE 4096

int main(int argc, char **argv) {
    char line[MAXLINE];
    FILE *fpin, *fpout;

    if (argc != 2) {
        puts("usage: a.out <pathname>");
        exit(1);
    }

    if ((fpin = fopen(argv[1], "r")) == NULL) {
        printf("can't open %s\n", argv[1]);
    }

    if ((fpout = popen(PAGER, "w")) == NULL) {
        puts("popen error");
        exit(1);
    }

    while (fgets(line, MAXLINE, fpin) != NULL) {
        if (fputs(line, fpout) == EOF) {
            puts("fputs error to pipe");
            exit(1);
        }
    }

    if (ferror(fpin)) {
        puts("fgets error");
        exit(1);
    }

    if (pclose(fpout) == -1) {
        puts("pclose error");
        exit(1);
    }
    exit(0);
}