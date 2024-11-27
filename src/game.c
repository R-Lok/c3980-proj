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

int wait_for_connection(struct GameData *gd)
{
    int         x_boundary;
    int         y_boundary;
    const char *msg = "Waiting for connection...";
    // printf("%i\n", win == NULL);
    getmaxyx(gd->win, y_boundary, x_boundary);
    if(print_message_window(gd->win, msg, y_boundary / 2, (x_boundary - (int)strlen(msg)) / 2))
    {
        // print an err message
        return 1;
    }
    if(send_player_info(gd->sock_fd, gd->my_player, gd->peer_addr))
    {
        // print an err message
        return 1;
    }
    if(receive_player_info(gd->sock_fd, gd->peer_addr, gd->peer_player))    // needs to deal with playing var
    {
        // print an err message
        return 1;
    }
    if(send_player_info(gd->sock_fd, gd->my_player, gd->peer_addr))    // redunant sending of my_player again in case peer opened program after first send
    {
        // print an err message
        return 1;
    }
    if(gd->playing == 0)    // deal with this (SIGINT later)
    {
        return 2;
    }
    return 0;
}

void populate_game_data(struct GameData *game_data, int sock_fd, struct sockaddr_in *peer_addr, struct PlayerInfo *my_player, struct PlayerInfo *peer_player, WINDOW *win, uint16_t *playing)
{
    game_data->sock_fd     = sock_fd;
    game_data->peer_addr   = peer_addr;
    game_data->my_player   = my_player;
    game_data->peer_player = peer_player;
    game_data->win         = win;
    game_data->playing     = playing;
}
