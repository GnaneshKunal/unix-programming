#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void) {
	char * buf = malloc(20 * sizeof(char));
	gethostname(buf, 20);
	puts(buf);
	free(buf);	
	exit(0);
}
