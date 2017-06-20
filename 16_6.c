#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

static struct servent *buf;

int main(void) {
    int i;

    setservent(1);

    // buf = getservbyname("localhost", "80");
    // puts(buf->s_name);

    while ((buf = getservent()) != NULL) {
        printf("Service Name: %s\n", buf->s_name);
        printf("Port Number: %d\n", buf->s_port);
    }
    exit(0);
}