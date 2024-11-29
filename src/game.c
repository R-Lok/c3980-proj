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

    my_player->x       = (uint16_t)(arc4random_uniform((uint32_t)x_boundary + 1));    // change to POSIX compliant random
    my_player->y       = (uint16_t)(arc4random_uniform((uint32_t)y_boundary + 1));    // change to POSIX compliant random
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
    // uncomment below when testing the peer communication
    //  if(receive_player_info(gd->sock_fd, gd->peer_addr, gd->peer_player, NULL))    // needs to deal with playing var
    //  {
    //      // print an err message
    //      return 1;
    //  }
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

int play_game(struct GameData *data, input_handler input_thread_func)
{
    int             ret;
    uint16_t        my_last_x;
    uint16_t        my_last_y;
    uint16_t        peer_last_x;
    uint16_t        peer_last_y;
    pthread_mutex_t my_player_mutex;
    pthread_mutex_t peer_mutex;
    pthread_t       ctrl_thread;
    pthread_t       peer_thread;
    void           *ctrl_thread_res;
    void           *peer_thread_res;

    struct GameSyncArgs input_handler_args;
    struct GameSyncArgs peer_handler_args;

    int thread_join_res;

    ret         = EXIT_SUCCESS;
    my_last_x   = data->my_player->x;
    my_last_y   = data->my_player->y;
    peer_last_x = data->peer_player->x;
    peer_last_y = data->peer_player->y;

    if(init_mutex(&my_player_mutex) || init_mutex(&peer_mutex))
    {
        return EXIT_FAILURE;
    }

    input_handler_args.player   = data->my_player;    // probably need to make a function for initalizing struct with args
    input_handler_args.sock_fd  = data->sock_fd;
    input_handler_args.peeraddr = data->peer_addr;
    input_handler_args.playing  = data->playing;
    input_handler_args.lock     = &my_player_mutex;

    peer_handler_args.player   = data->peer_player;
    peer_handler_args.sock_fd  = data->sock_fd;
    peer_handler_args.peeraddr = data->peer_addr;
    peer_handler_args.playing  = data->playing;
    peer_handler_args.lock     = &peer_mutex;

    if(pthread_create(&ctrl_thread, NULL, input_thread_func, (void *)&input_handler_args))
    {
        fprintf(stderr, "pthread_create err\n");
        *(data->playing) = 0;
        ret              = 1;
        goto thread_fail;
    }

    if(pthread_create(&peer_thread, NULL, handle_peer_routine, (void *)&peer_handler_args))
    {
        fprintf(stderr, "pthread_create err\n");
        *(data->playing) = 0;
        ret              = 1;
        goto thread_fail_2;
    }

    while(*(data->playing))
    {
        print_player_position(data->my_player, &my_last_y, &my_last_x, data->win, &my_player_mutex);
        print_player_position(data->peer_player, &peer_last_y, &peer_last_x, data->win, &peer_mutex);
        wrefresh(data->win);
    }

    thread_join_res = pthread_join(ctrl_thread, &ctrl_thread_res);
    process_thread_res(thread_join_res, *((int *)ctrl_thread_res), &ret);
thread_fail_2:
    thread_join_res = pthread_join(peer_thread, &peer_thread_res);
    process_thread_res(thread_join_res, *((int *)peer_thread_res), &ret);
thread_fail:
    *(data->playing)         = 0;
    data->my_player->playing = 0;
    send_player_info(data->sock_fd, data->my_player, data->peer_addr);
    return ret;
}
