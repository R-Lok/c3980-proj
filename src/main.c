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
    int                ret;

    ret            = EXIT_SUCCESS;
    port           = DEFAULT_PORT;
    peer_str_addr  = NULL;
    control_scheme = NOT_SET;

    if(parse_args(argc, argv, &peer_str_addr, &control_scheme, &err))
    {
        printError(err, "Error parsing arguments\n");
        usage();
        ret = EXIT_FAILURE;
        goto arg_error;
    }
    if(setup_addrs(&my_addr, &peer_addr, peer_str_addr, port, &err))
    {
        printError(err, "Supplied address is not an IPv4 address");
        usage();
        ret = EXIT_FAILURE;
        goto arg_error;
    }

arg_error:
    return ret;
}

void printError(int err, const char *msg)
{
    fprintf(stderr, "Error: %s :%s\n", msg, strerror(err));
}
