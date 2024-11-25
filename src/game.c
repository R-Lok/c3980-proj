#include "game.h"
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
