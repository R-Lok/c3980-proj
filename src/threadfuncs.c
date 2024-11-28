#include "../include/threadfuncs.h"
#include "../include/game.h"
#include "../include/io.h"
#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>

int  handle_pressed_char(int pressed_char, struct PlayerInfo *my_player, pthread_mutex_t *lock);
void translate_key_to_movement(int pressed_char, int16_t *mov_y, int16_t *mov_x);

void *keyboard_routine(void *thread_args)
{
    struct GameSyncArgs *args;
    int                 *return_val;

    return_val = (int *)malloc(sizeof(int));
    if(return_val == NULL)
    {
        fprintf(stderr, "Malloc failed to allocate memory\n");
        return NULL;
    }

    *return_val = EXIT_SUCCESS;
    args        = (struct GameSyncArgs *)thread_args;

    while(*(args->playing))
    {
        int received_char;
        int handle_char_res;

        received_char = getch();

        handle_char_res = handle_pressed_char(received_char, args->player, args->lock);
        if(handle_char_res == 1)
        {
            *(args->playing) = 0;
            *return_val      = EXIT_FAILURE;
            return return_val;
        }
        if(handle_char_res == 0 && send_player_info(args->sock_fd, args->player, args->peeraddr))
        {
            *(args->playing) = 0;
            *return_val      = EXIT_FAILURE;
            return return_val;
        }
    }
    return return_val;
}

int handle_pressed_char(const int pressed_char, struct PlayerInfo *my_player, pthread_mutex_t *lock)
{
    int16_t mov_y;
    int16_t mov_x;

    mov_y = 0;
    mov_x = 0;

    translate_key_to_movement(pressed_char, &mov_y, &mov_x);

    if(mov_x == 0 && mov_y == 0)
    {
        return -1;    // return -1 to indicate no change in position
    }

    if((mov_x == 1 && my_player->x == X_BOUNDARY) || (mov_x == -1 && my_player->x == X_BOUNDARY_MIN))
    {
        return -1;    // no change in position, running into wall
    }

    if((mov_y == 1 && my_player->y == Y_BOUNDARY) || (mov_y == -1 && my_player->y == Y_BOUNDARY_MIN))
    {
        return -1;    // no change in position, running into wall
    }

    if(pthread_mutex_lock(lock) != 0)
    {
        perror("pthread_mutex_lock failed to lock");
        return 1;
    }
    if(mov_y == 1)
    {
        (my_player->y)++;
    }
    else if(mov_y == -1)
    {
        (my_player->y)--;
    }

    if(mov_x == 1)
    {
        (my_player->x)++;
    }
    else if(mov_x == -1)
    {
        (my_player->x)--;
    }

    my_player->seq_counter++;

    if(pthread_mutex_unlock(lock) != 0)
    {
        perror("pthread_mutex_unlock failed to unlock");
        return 1;
    }
    return 0;
}

void translate_key_to_movement(int pressed_char, int16_t *mov_y, int16_t *mov_x)
{
    switch(pressed_char)
    {
        case 'W':
        case 'w':
        case KEY_UP:
            *mov_y = 1;
            break;
        case 'S':
        case 's':
        case KEY_DOWN:
            *mov_y = -1;
            break;
        case 'A':
        case 'a':
        case KEY_LEFT:
            *mov_x = -1;
            break;
        case 'D':
        case 'd':
        case KEY_RIGHT:
            *mov_x = 1;
            break;
        default:    // do nothing
            break;
    }
}
