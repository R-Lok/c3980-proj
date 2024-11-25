#include "io.h"
#include <errno.h>

void *pickle_player_info(const struct PlayerInfo *my_player, uint16_t *pickled_array) {

    pickled_array[0] = htons(my_player->playing);
    pickled_array[1] = htons(my_player->seq_counter);
    pickled_array[2] = htons(my_player->x);
    pickled_array[3] = htons(my_player->y);

}

int send_full(int sock_fd, uint16_t *pickled_player, size_t pickle_bytelength, const struct sockaddr_in *peer_addr) {
    size_t twrote;
    ssize_t nwrote;

    do {
        nwrote = sendto(sock_fd, pickled_player + (twrote / 2), pickle_bytelength - twrote, 0, (struct sockaddr *)peer_addr, sizeof(*peer_addr));
        if(nwrote == -1) {
            if (errno = EINTR) {
                errno = 0;
                continue;
            }
            return 1;
        }
        twrote += (size_t) nwrote;
    } while (twrote < pickle_bytelength);
    return 0;
}

int send_player_info(int sock_fd, const struct PlayerInfo *my_player, const struct sockaddr_in peer_addr) {
    uint16_t pickled_player[4];

    pickle_player_info(my_player, pickled_player);

    if(send_full(sock_fd, pickled_player, sizeof(pickled_player), &peer_addr) == -1) {
        return 1;
    }
    return 0;
}

