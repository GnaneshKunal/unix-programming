#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int main(void) {
	
	int fd1;
	FILE * f1;
	char *buf = malloc(4096 * sizeof(char));

	if ((fd1 = open("tempfile", O_RDWR, O_APPEND)) < 0) {
		perror("Error");
		exit(1);
	}
	if (read(fd1, buf, 4096) < 0){
		perror("Error");
		exit(1);
	}
	
	if (write(STDOUT_FILENO, buf, strlen(buf)) < 0) {
		perror("Error");
		exit(1);
	}
	putchar('\n');
	if ((f1 = fopen("tempfile2", "r+")) != NULL) {
		fprintf(f1, "This is not a %s test\n", "fucking");
	}
	lseek(fd1, 0, SEEK_SET);
	long tell = ftell(f1); 
	rewind(f1);
	if(fseeko(f1, 2, SEEK_CUR) == -1){
		perror("Error");
		exit(1);
	}
	dprintf(fd1, "Who do you think I %s \? \n", "am");
	sprintf(buf, "You are Mr. %s\n", "Durden");
	snprintf(buf + 18, 4096 - 18, ", You are the one who game me this %s.\n", "scar");
	printf("%s", buf);
	fclose(f1);
	close(fd1);
	free(buf);
	exit(0);
}
