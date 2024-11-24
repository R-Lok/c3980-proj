#include "../include/args.h"
#include <ncurses.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_PORT 9999

int main(int argc, char **argv)
{
    char              *peer_str_addr;
    enum ControlScheme control_scheme;
    int                err;
    in_port_t          port;

    port           = DEFAULT_PORT;
    peer_str_addr  = NULL;
    control_scheme = NOT_SET;

    if(parse_args(argc, argv, &peer_str_addr, &control_scheme, &err))
    {
        fprintf(stderr, "Error: %s\n", strerror(err));
        usage();
    }

    return EXIT_SUCCESS;
}
