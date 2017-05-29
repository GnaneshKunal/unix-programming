#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define MAXLINE 2048

int main(void) {
	char name[L_tmpnam], line[MAXLINE];
	FILE *fp;

	printf("%s\n", tmpnam(NULL));

	tmpnam(name);
	printf("%s\n", name);

	if ((fp = tmpfile()) == NULL) {
		perror("Error");
		exit(1);
	}
	fputs("one line of output\n", fp);
	rewind(fp);
	if (fgets(line, sizeof(line), fp) == NULL) {
		perror("Error");
		exit(1);
	}
	fputs(line, stdout);

	exit(0);
}
