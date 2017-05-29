#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BSZ 4096

int main(void) {
	FILE *fp;
	char buf[BSZ];

	memset(buf, 'a', BSZ-2);
	buf[BSZ - 2] = '\0';
	buf[BSZ - 1] = 'X';

	if ((fp = fmemopen(buf, BSZ, "w+")) == NULL){
		perror("Error");
		exit(1);
	}
	printf("Inital contents of buffer: %s\n", buf);
	fprintf(fp, "Hello, world");
	printf("before fflush: %s\n", buf);
	fflush(fp);
	printf("after fflush: %s\n", buf);
	printf("len of string in buf = %ld\n", (long)strlen(buf));
	
	memset(buf, 'b', BSZ-2);
	buf[BSZ-2] = '\0';
	buf[BSZ-1] = 'X';
	fprintf(fp, "hello, world");
	fseek(fp, 0, SEEK_SET);
	printf("after fseek: %s\n", buf);
	printf("len of string in buf = %ld\n", (long)strlen(buf));
	memset(buf, 'c', BSZ-2);
	buf[BSZ-2] = '\0';
	buf[BSZ-1] = 'X';
	fprintf(fp, "hello, world");
	fclose(fp);
	printf("after fclose: %s\n", buf);
	printf("len of string in buf = %ld\n", (long)strlen(buf));
	return(0);
}
