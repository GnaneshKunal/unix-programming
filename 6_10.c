#include <stdio.h>
#include <time.h>

int main(void) {
	time_t time_tim;
	time_tim = time(NULL);
	printf("%ld", time_tim);
	return 0;
}
