#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

static struct hostent *hostbuf;

int main(void) {
    int i;

    sethostent(1);
    while ((hostbuf = gethostent()) != NULL) {
        printf("Host Name: %s\n", hostbuf->h_name);
        puts("Aliases:");
        for (i = 0; hostbuf->h_aliases[i] != NULL; i++)
            printf("\t%s\n", hostbuf->h_aliases[i]);
        printf("Address Type: %d\n", hostbuf->h_addrtype);
        printf("Address Length: %d\n", hostbuf->h_length);
        puts("List of network address:");
        for (i = 0; hostbuf->h_addr_list[i] != NULL; i++)
            printf("\t%s\n", hostbuf->h_addr_list[i]);
    }

    endhostent();
    exit(0);
}