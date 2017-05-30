#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

static void my_exit1(void);
static void my_exit2(void);

int main(void) {
	if (atexit(my_exit2) != 0) {
		perror("Error");
		exit(0);
	}
	if (atexit(my_exit1) != 0) {
		perror("Error");
		exit(0);
	}

	if (atexit(my_exit1) != 0) {
		perror("Error");
		exit(0);
	}

	puts("Main is done");
	return(0);
}

static void my_exit1(void) {
	puts("first exit handler");
}

static void my_exit2(void) {
	puts("second exit handler");
}
