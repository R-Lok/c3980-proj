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

struct GameData
{
    // cppcheck-suppress unusedStructMember
    int sock_fd;
    // cppcheck-suppress unusedStructMember
    struct sockaddr_in *peer_addr;
    // cppcheck-suppress unusedStructMember
    struct PlayerInfo *my_player;
    // cppcheck-suppress unusedStructMember
    struct PlayerInfo *peer_player;
    // cppcheck-suppress unusedStructMember
    WINDOW *win;
    // cppcheck-suppress unusedStructMember
    uint16_t *playing;
};

struct GameSyncArgs {
    struct PlayerInfo * player;
    int sock_fd;
    struct sockaddr_in *peeraddr;
    uint16_t *playing;
    pthread_mutex_t *lock;
};

void setup_player_structs(struct PlayerInfo *my_player, struct PlayerInfo *peer_player, int x_boundary, int y_boundary);
int  wait_for_connection(struct GameData *gd);
void populate_game_data(struct GameData *game_data, int sock_fd, struct sockaddr_in *peer_addr, struct PlayerInfo *my_player, struct PlayerInfo *peer_player, WINDOW *win, uint16_t *playing);

#endif
