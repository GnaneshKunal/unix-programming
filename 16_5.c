#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

struct protoent *buf;

int main(void) {
    int i;
    char *s;
    const char *d;

    setprotoent(1);

    while ((buf = getprotoent()) != NULL) {
        printf("Protocol Name: %s\n", buf->p_name);
        printf("Protocol Number: %d\n", buf->p_proto);
        printf("Alternate Protocol Name: ");
        for (i = 0; buf->p_aliases[i] != NULL; i++)
            printf("%s\n", buf->p_aliases[i]);
    }

    endprotoent();
    exit(0);
}