#include "game.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void setup_player_structs(struct PlayerInfo *my_player, struct PlayerInfo *peer_player, const int x_boundary, const int y_boundary)
{
    struct timespec ts;
    memset(my_player, 0, sizeof(struct PlayerInfo));
    memset(peer_player, 0, sizeof(struct PlayerInfo));

    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((unsigned int)(ts.tv_nsec ^ ts.tv_sec));
    my_player->x       = (uint16_t)(rand() % x_boundary);
    my_player->y       = (uint16_t)(rand() % y_boundary);
    my_player->playing = 1;
}
