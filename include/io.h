#ifndef IO_H
#define IO_H
#include "game.h"

int send_player_info(int sock_fd, const struct PlayerInfo *my_player, const struct sockaddr_in *peer_addr);
void pickle_player_info(const struct PlayerInfo *my_player, uint16_t *pickled_array);

#endif