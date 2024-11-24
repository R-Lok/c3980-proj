#include "../include/args.h"
#include "../include/socket.h"
#include <ncurses.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_PORT 9999

void printError(int err, const char *msg);

int main(int argc, char **argv)
{
    char              *peer_str_addr;
    enum ControlScheme control_scheme;
    int                err;
    in_port_t          port;
    struct sockaddr_in my_addr;
    struct sockaddr_in peer_addr;

    port           = DEFAULT_PORT;
    peer_str_addr  = NULL;
    control_scheme = NOT_SET;

    if(parse_args(argc, argv, &peer_str_addr, &control_scheme, &err))
    {
        printError(err, "Error parsing arguments");
        usage();
    }
    if(setup_addrs(&my_addr, &peer_addr, peer_str_addr, port, &err))
    {
        printError(err, "Error setting up address structs");
        usage();
    }

    return EXIT_SUCCESS;
}

void printError(int err, const char *msg)
{
    fprintf(stderr, "Error: %s :%s", msg, strerror(err));
}
