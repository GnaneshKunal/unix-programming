#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

void make_temp(char *);

/*
char *mkdtemp(char *template); creates a directory - returns pointer to directory
int mkstemp(char *template); returns a file descriptor to the temp file
*/

int main(void) {
	char good_template[] = "/tmp/dirXXXXXX"; //right way
	char *bad_template = "/tmp/dirXXXXXX"; //wrong way

	puts("Trying to create first temp file");
	make_temp(good_template);
	puts("trying to create sencond temp file");
	make_temp(bad_template);
	exit(0);
}

void make_temp(char * template) {
	int fd;
	struct stat buf;

	if ((fd = mkstemp(template)) < 0) {
		perror("Error");
		exit(1);
	}
	printf("temp name = %s\n", template);
	close(fd);
	if (stat(template, &buf) < 0)
		if (errno == ENOENT)
			puts("file doesn't exist");
		else {
			perror("Error");
			exit(1);
		}
	else {
		printf("file exists\n");
		unlink(template); //mkstemp doesn't unlink itself, we have to do it
	}
}
