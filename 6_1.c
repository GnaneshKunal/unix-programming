#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>

int main(void) {
	struct passwd * pw;
	pw = getpwuid(getuid());
	write(STDOUT_FILENO, pw->pw_name, strlen(pw->pw_name));
	write(STDIN_FILENO, "\n", 1);
	write(STDOUT_FILENO, pw->pw_passwd, strlen(pw->pw_passwd));
	exit(0);
}

