#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

struct netent *buf;

int main(void) {
    int i;
    char *s;
    const char *d;

    setnetent(1);
    while ((buf = getnetent()) != NULL) {
        printf("Network Name: %s\n", buf->n_name);
        printf("Address Type: %d\n", buf->n_addrtype);
        printf("Network Number: %u\n", ntohl(buf->n_net));
        puts("Network Array pointers:");
        for (i = 0; buf->n_aliases[i] != NULL; i++) {
            printf("\t %s\n", buf->n_aliases[i]);
            d = inet_ntop(AF_INET, buf->n_aliases[i], s, 40);
            printf("\t%s\n", d);
        }
    }

    endnetent();
    exit(0);
}