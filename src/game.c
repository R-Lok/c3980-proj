#include "game.h"
#include "io.h"
#include "screen.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void setup_player_structs(struct PlayerInfo *my_player, struct PlayerInfo *peer_player, const int x_boundary, const int y_boundary)
{
    memset(my_player, 0, sizeof(struct PlayerInfo));
    memset(peer_player, 0, sizeof(struct PlayerInfo));

    my_player->x       = (uint16_t)(arc4random_uniform((uint32_t)x_boundary + 1));
    my_player->y       = (uint16_t)(arc4random_uniform((uint32_t)y_boundary + 1));
    my_player->playing = 1;
}

int wait_for_connection(int sock_fd, struct sockaddr_in peer_addr, struct PlayerInfo *my_player, struct PlayerInfo *peer_player, WINDOW *win, uint16_t *playing)
{
    int x_boundary;
    int y_boundary;

    getmaxyx(win, y_boundary, x_boundary);
    if(print_message_window(win, "Waiting for connection...", y_boundary, x_boundary))
    {
        // print an err message
        return 1;
    }
    if(send_player_info(sock_fd, my_player, peer_addr))
    {
        // print an err message
        return 1;
    }
    //WIP receive_player_info

    return 0;
}
