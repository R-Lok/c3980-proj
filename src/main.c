#include "../include/args.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    char              *peer_str_addr;
    enum ControlScheme control_scheme;
    int                err;

    peer_str_addr  = NULL;
    control_scheme = NOT_SET;

    if(parse_args(argc, argv, &peer_str_addr, control_scheme, &err))
    {
        fprintf(stderr, "Error: %s\n", strerror(err));
        usage();
    }

    return EXIT_SUCCESS;
}
