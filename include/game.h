#ifndef GAME_H
#define GAME_H
#include <ncurses.h>
#include <netinet/in.h>
#include <stdint.h>

struct PlayerInfo
{
    // cppcheck-suppress unusedStructMember
    uint16_t playing;
    // cppcheck-suppress unusedStructMember
    uint16_t seq_counter;
    // cppcheck-suppress unusedStructMember
    uint16_t x;
    // cppcheck-suppress unusedStructMember
    uint16_t y;
};

void setup_player_structs(struct PlayerInfo *my_player, struct PlayerInfo *peer_player, int x_boundary, int y_boundary);
int  wait_for_connection(int sock_fd, struct sockaddr_in peer_addr, struct PlayerInfo *my_player, struct PlayerInfo *peer_player, WINDOW *win, uint16_t *playing);

#endif
