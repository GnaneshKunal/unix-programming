#include <string.h>
#include <stddef.h>
#include <pwd.h>

struct passwd *getpwnam(const char *name) {
	struct passwd *prt;

	setpwent();
	while ((ptr = getpwent()) != NULL)
		if (strcmp(name, ptr->pw_name) == 0)
			break;
	endpwent();
	return ptr;
}
