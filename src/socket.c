#include "socket.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>

int setup_addrs(struct sockaddr_in *my_addr, struct sockaddr_in *peer_addr, char *peer_str_addr, in_port_t port, int *err)
{
    const char *addr = "0.0.0.0";    // Listen to all incoming connections on all availalbe network interfaces

    memset(my_addr, 0, sizeof(struct sockaddr_in));
    memset(peer_addr, 0, sizeof(struct sockaddr_in));

    my_addr->sin_family = AF_INET;
    my_addr->sin_port   = htons(port);

    if(inet_pton(AF_INET, addr, &(my_addr->sin_addr)) != 1)
    {
        *err = EINVAL;
        return 1;
    }

    peer_addr->sin_family = AF_INET;
    peer_addr->sin_port   = htons(port);

    if(inet_pton(AF_INET, peer_str_addr, &(peer_addr->sin_addr)) != 1)
    {
        *err = EINVAL;
        return 1;
    }
    return 0;
}
