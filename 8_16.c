#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

char * env_init[] = { "USER=unknown", "PATH=/tmp", NULL };

int main(void) {
	pid_t pid;
	if ((pid = fork()) < 0) {
		puts("fork error");
		exit(1);
	} else if (pid == 0) {
		if (execle("/home/monster/git/unix-programming/echoall", "echoall", "myarg1", "MY ARG@", (char *) 0, env_init) < 0) {
			puts("execle error");
			exit(1);
		}
	}

	if (waitpid(pid, NULL, 0) < 0) {
		puts("wait error");
		exit(1);
	}

	if ((pid = fork()) < 0) {
		puts("fork error");
		exit(1);
	} else if (pid == 0) {
		if (execlp("echoall", "echoall", "only 1 arg", (char *)0) < 0) {
			puts("execlp error");
			exit(1);
		}
	}
	exit(0);
}
