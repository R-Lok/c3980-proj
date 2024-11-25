#ifndef IO_H
#define IO_H
#include "game.h"

int send_player_info(int sock_fd, const struct PlayerInfo *my_player, const struct sockaddr_in peer_addr);

#endif