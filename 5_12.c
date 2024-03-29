#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

void pr_stdio(const char *, FILE *);
int is_unbuffered(FILE *);
int is_linebuffered(FILE *);
int buffer_size(FILE *);
int quit(char *);

int main(void) {
	FILE *fp;
	fputs("Enter any character\n", stdout);
	if (getchar() == EOF)
		quit("getchar error");

	fputs("one line to standard error\n", stderr);

	pr_stdio("stdin", stdin);
	pr_stdio("stdout", stdout);
	pr_stdio("stderr", stderr);
	
	if ((fp = fopen("/etc/passwd", "r")) == NULL)
		quit("fopen err");
	if (getc(fp) == EOF)
		quit("get c error");
	pr_stdio("/etc/passwd", fp);
	exit(0);
}

int quit(char * err) {
	perror(err);
	exit(1);
}

void pr_stdio(const char * name, FILE *fp) {
	printf("stream = %s ", name);
	if (is_unbuffered(fp))
		printf("is unbuffered");
	else if (is_linebuffered(fp))
		printf("is line buffered");
	else
		printf("fully buffered");
	printf(", buffered size = %d\n", buffer_size(fp));
}

#if defined(_IO_UNBUFFERED)
int is_unbuffered(FILE *fp) {
	return (fp->_flags & _IO_UNBUFFERED);
}

int is_linebuffered(FILE *fp) {
	return (fp->_flags & _IO_LINE_BUF);
}

int buffer_size(FILE *fp) {
	return (fp->_IO_buf_end - fp->_IO_buf_base);
}

#elif defined(__SNBF)
int is_unbuffered(FILE *fp) {
	return (fp->_flags & __SNBF);
}

int is_linebuffered(FILE *fp) {
	return (fp->_flags & __SLBF);
}

int buffer_size (FILE *fp) {
	return (fp->_bf.size);
}

#elif defined(_IONBF)

#ifdef _LP64
#define _flag __pad[4]
#define _ptr __pad[1]
#define _base __pad[2]
#endif

int is_unbuffered (FILE *fp) {
	return (fp->_flag & __IONBF);
}

int is_linebuffered (FILE *fp) {
	return (fp_>_flag & __IOLBF);
}

int buffer_size (FILE *fp) {
#ifdef _LP64
	return (fp->_base - fp->_ptr);
#else
	return (BUFSIZ);
#endif
}

#else

#error unknown stdio implementation!

#endif
