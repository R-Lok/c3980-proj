#ifndef IO_H
#define IO_H
#include "../include/game.h"

int send_player_info(int sock_fd, const struct PlayerInfo *my_player, const struct sockaddr_in *peer_addr);
int receive_player_info(int sock_fd, struct sockaddr_in *peer_addr, struct PlayerInfo *peer_player);

#endif
