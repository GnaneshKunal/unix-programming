#include <shadow.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
	struct spwd *sp;
	sp = getspnam("monster");
	write(STDOUT_FILENO, sp->sp_namp, strlen(sp->sp_namp));
	write(STDOUT_FILENO, "\n", 1);
	write(STDOUT_FILENO, sp->sp_pwdp, strlen(sp->sp_pwdp));
	exit(0);
}
