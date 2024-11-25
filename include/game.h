#ifndef GAME_H
#define GAME_H
#include <stdint.h>

struct PlayerInfo
{
    uint16_t playing;
    uint16_t seq_counter;
    uint16_t x;
    uint16_t y;
};

void setup_player_structs(struct PlayerInfo *my_player, struct PlayerInfo *peer_player, int x_boundary, int y_boundary);

#endif
