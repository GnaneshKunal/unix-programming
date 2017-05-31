#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
	puts("Start");
	printf("ID: %d\n", getpid());
	
	pid_t pid;
	if ((pid = fork()) < 0) {
		perror("Error");
		exit(1);
	}
	else if (pid == 0) {
		puts("I'm child");
		printf("My parent ID: %d\n", getppid());
		printf("My ID: %d\n", getpid());
		return 0;
	}
	puts("Exiting");
	exit(0);
}
