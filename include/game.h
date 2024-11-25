#ifndef GAME_H
#define GAME_H
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

#endif
