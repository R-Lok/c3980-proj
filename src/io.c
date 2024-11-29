#include "io.h"
#include <errno.h>
#include <string.h>

void pickle_player_info(const struct PlayerInfo *my_player, uint16_t *pickled_array);
int  send_full(int sock_fd, const uint16_t *pickled_player, size_t pickle_bytelength, const struct sockaddr_in *peer_addr);
int  read_full(int sock_fd, struct sockaddr_in *peer_addr, uint16_t *pickled_player, size_t bytes_to_read);
void unpickle_player(const uint16_t *pickled_player, struct PlayerInfo *received_player);
int  is_stale_data(const struct PlayerInfo *received_player, const struct PlayerInfo *peer_player);

void pickle_player_info(const struct PlayerInfo *my_player, uint16_t *pickled_array)
{
    pickled_array[0] = htons(my_player->playing);
    pickled_array[1] = htons(my_player->seq_counter);
    pickled_array[2] = htons(my_player->x);
    pickled_array[3] = htons(my_player->y);
}

int send_full(int sock_fd, const uint16_t *pickled_player, size_t pickle_bytelength, const struct sockaddr_in *peer_addr)
{
    size_t twrote;
    twrote = 0;

    do
    {
        ssize_t nwrote;
        nwrote = sendto(sock_fd, (const char *)pickled_player + twrote, pickle_bytelength - twrote, 0, (const struct sockaddr *)peer_addr, sizeof(*peer_addr));
        if(nwrote == -1)
        {
            if(errno == EINTR)
            {
                errno = 0;
                continue;
            }
            return -1;
        }
        twrote += (size_t)nwrote;
    } while(twrote < pickle_bytelength);
    return 0;
}

int send_player_info(int sock_fd, const struct PlayerInfo *my_player, const struct sockaddr_in *peer_addr)
{
    uint16_t pickled_player[4];

    pickle_player_info(my_player, pickled_player);

    if(send_full(sock_fd, pickled_player, sizeof(pickled_player), peer_addr) == -1)
    {
        return 1;
    }
    return 0;
}

int read_full(int sock_fd, struct sockaddr_in *peer_addr, uint16_t *pickled_player, size_t bytes_to_read)
{
    size_t    tread;
    socklen_t addr_len;
    tread    = 0;
    addr_len = sizeof(*peer_addr);

    do
    {
        ssize_t nread;
        nread = recvfrom(sock_fd, (char *)pickled_player + tread, bytes_to_read - tread, 0, (struct sockaddr *)peer_addr, &addr_len);
        if(nread == -1)
        {
            if(errno == EINTR)
            {
                errno = 0;
                continue;
            }
            return -1;
        }
        tread += (size_t)nread;
    } while(tread < bytes_to_read);
    return 0;
}

void unpickle_player(const uint16_t *pickled_player, struct PlayerInfo *received_player)
{
    received_player->playing     = ntohs(pickled_player[0]);
    received_player->seq_counter = ntohs(pickled_player[1]);
    received_player->x           = ntohs(pickled_player[2]);
    received_player->y           = ntohs(pickled_player[3]);
}

int is_stale_data(const struct PlayerInfo *received_player, const struct PlayerInfo *peer_player)
{
    if(received_player->seq_counter == 0 && peer_player->seq_counter == UINT16_MAX)
    {
        return 0;
    }
    if(received_player->seq_counter < peer_player->seq_counter)
    {
        return 1;
    }
    return 0;
}

int receive_player_info(int sock_fd, struct sockaddr_in *peer_addr, struct PlayerInfo *peer_player, pthread_mutex_t *lock)
{
    uint16_t          pickled_player[4];
    struct PlayerInfo received_player;
    memset(&received_player, 0, sizeof(struct PlayerInfo));

    if(read_full(sock_fd, peer_addr, pickled_player, sizeof(pickled_player)) == -1)
    {
        return 1;
    }

    unpickle_player(pickled_player, &received_player);
    if(is_stale_data(&received_player, peer_player) == 0)
    {
        if(lock != NULL && pthread_mutex_lock(lock))
        {
            fprintf(stderr, "Error locking mutex\n");
            return 1;
        }
        peer_player->playing     = received_player.playing;
        peer_player->seq_counter = received_player.seq_counter;
        peer_player->x           = received_player.x;
        peer_player->y           = received_player.y;
        if(lock != NULL && pthread_mutex_unlock(lock))
        {
            fprintf(stderr, "Error unlocking mutex\n");
            return 1;
        }
    }
    return 0;
}
