#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int globvar = 6;

int main(void) {
	int var;
	pid_t pid;
	var = 88;
	puts("before fork");
	if ((pid = vfork()) < 0) {
		perror("Error");
		exit(1);
	} else if (pid == 0) {
		globvar++;
		var++;
		_exit(0);
	}

	printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);
	exit(0);
}
