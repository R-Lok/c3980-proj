#include "../include/game.h"
#include "../include/io.h"
#include "../include/screen.h"
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

int wait_for_connection(int sock_fd, struct sockaddr_in *peer_addr, const struct PlayerInfo *my_player, struct PlayerInfo *peer_player, WINDOW *win, const uint16_t *playing)
{
    int         x_boundary;
    int         y_boundary;
    const char *msg = "Waiting for connection...";
    // printf("%i\n", win == NULL);
    getmaxyx(win, y_boundary, x_boundary);
    if(print_message_window(win, msg, y_boundary / 2, (x_boundary - (int)strlen(msg)) / 2))
    {
        // print an err message
        return 1;
    }
    if(send_player_info(sock_fd, my_player, peer_addr))
    {
        // print an err message
        return 1;
    }
    if(receive_player_info(sock_fd, peer_addr, peer_player))    // needs to deal with playing var
    {
        // print an err message
        return 1;
    }
    if(send_player_info(sock_fd, my_player, peer_addr))    // redunant sending of my_player again in case peer opened program after first send
    {
        // print an err message
        return 1;
    }
    if(playing == 0)    // deal with this (SIGINT later)
    {
        return 2;
    }
    return 0;
}
