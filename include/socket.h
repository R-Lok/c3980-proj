#ifndef SOCKET_H
#define SOCKET_H
#include <netinet/in.h>

int setup_addrs(struct sockaddr_in *my_addr, struct sockaddr_in *peer_addr, char *peer_str_addr, in_port_t port, int *err);
int setup_socket(struct sockaddr_in *my_addr, int *err);

#endif
