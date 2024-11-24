#include "socket.h"
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

int setup_socket(struct sockaddr_in *my_addr, int *err)
{
    int socket_fd;
    int flags;

    socket_fd = socket(my_addr->sin_family, SOCK_DGRAM, 0);    // NOLINT(android-cloexec-socket)

    if(socket_fd == -1)
    {
        *err = errno;
        return -1;
    }

    flags = fcntl(socket_fd, F_GETFL, 0);
    if(flags == -1)
    {
        *err = errno;
        fprintf(stderr, "Failed to retrieve socket flags\n");
        goto fail;
    }

    if(fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        *err = errno;
        fprintf(stderr, "Failed to set socket to non-blocking\n");
        goto fail;
    }

    return socket_fd;
fail:
    close(socket_fd);
    return -1;
}
