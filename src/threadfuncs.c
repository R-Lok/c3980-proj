#include "../include/threadfuncs.h"
#include "../include/game.h"
#include "../include/io.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define FIFTY 50            // 50 ms
#define FIVE_HUNDRED 500    // 500 ms

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

        if(*(args->playing) == 0)
        {
            return return_val;
        }

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

    if((mov_x == 1 && my_player->x == X_BOUNDARY - 1) || (mov_x == -1 && my_player->x == X_BOUNDARY_MIN + 1))
    {
        return -1;    // no change in position, running into wall
    }

    if((mov_y == 1 && my_player->y == Y_BOUNDARY_MIN + 1) || (mov_y == -1 && my_player->y == Y_BOUNDARY - 1))
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
        (my_player->y)--;
    }
    else if(mov_y == -1)
    {
        (my_player->y)++;
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
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            *mov_y = 1;
            break;
        case 'S':
        case 's':
        case KEY_DOWN:
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            *mov_y = -1;
            break;
        case 'A':
        case 'a':
        case KEY_LEFT:
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            *mov_x = -1;
            break;
        case 'D':
        case 'd':
        case KEY_RIGHT:
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            *mov_x = 1;
            break;
        default:    // do nothing
            break;
    }
}

void *timer_routine(void *thread_args)
{
    struct GameSyncArgs *args;
    int                 *return_val;
    struct timespec      ts;
    const long           delay = 50000000;

    ts.tv_sec  = 0;
    ts.tv_nsec = delay;
    args       = (struct GameSyncArgs *)thread_args;
    return_val = (int *)malloc(sizeof(int));
    if(return_val == NULL)
    {
        fprintf(stderr, "Malloc failed to allocate memory\n");
        return NULL;
    }
    *return_val = EXIT_SUCCESS;

    while(*(args->playing))
    {
        const int choices[4] = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT};
        int       simulated_key_press;
        int       handle_char_res;

        srandom((unsigned int)time(NULL));              // NOLINT(cert-msc32-c,cert-msc51-cpp)
        simulated_key_press = choices[random() % 4];    // generates a warning, but we don't care about the security of how moves are generated.

        handle_char_res = handle_pressed_char(simulated_key_press, args->player, args->lock);
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
        nanosleep(&ts, NULL);
    }
    return return_val;
}

void process_thread_res(int thread_join_res, int thread_routine_res, int *ret)
{
    if(thread_join_res != 0)
    {
        perror("pthread_join error");
        *ret = EXIT_FAILURE;
    }
    if(thread_routine_res != 0)
    {
        *ret = EXIT_FAILURE;    // no error message here as it is already printed within thread routine.
    }
}

int init_mutex(pthread_mutex_t *lock)
{
    int ret;
    int res;
    ret = 0;

    res = pthread_mutex_init(lock, NULL);
    if(res != 0)
    {
        fprintf(stderr, "pthread_mutex_init err\n");
        ret = 1;
    }
    return ret;
}

void *handle_peer_routine(void *thread_args)
{
    struct GameSyncArgs *args;
    int                 *return_val = (int *)malloc(sizeof(int));
    if(return_val == NULL)
    {
        fprintf(stderr, "Malloc failed to allocate memory\n");
        return NULL;
    }
    *return_val = EXIT_SUCCESS;

    args = (struct GameSyncArgs *)thread_args;

    while(*(args->playing) != 0)
    {
        if(receive_player_info(args->sock_fd, args->peeraddr, args->player, args->lock, args->playing))
        {
            *(args->playing) = false;
            *return_val      = EXIT_FAILURE;
        }
    }
    return return_val;
}

void *controller_routine(void *thread_args)
{
    struct GameSyncArgs *args;
    int                 *return_val;
    SDL_GameController  *controller;
    SDL_Event            event;
    uint8_t              button;

    return_val = (int *)malloc(sizeof(int));
    if(return_val == NULL)
    {
        fprintf(stderr, "Malloc failed to allocate memory\n");
        return NULL;
    }

    *return_val = EXIT_SUCCESS;
    args        = (struct GameSyncArgs *)thread_args;

    if(SDL_Init(SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        *return_val = -1;
    }

    controller = NULL;
    if(SDL_NumJoysticks() > 0)
    {
        controller = SDL_GameControllerOpen(0);
        if(!controller)
        {
            printf("Could not open game controller: %s\n", SDL_GetError());
            *return_val = -1;
            SDL_Quit();
            *(args->playing) = 0;
        }
    }
    else
    {
        printf("No game controllers connected.\n");
        *return_val = -1;
        SDL_Quit();
        *(args->playing) = 0;
    }

    button = 0;
    while(*(args->playing))
    {
        int  received_char;
        int  handle_char_res;
        bool first_delay;

        first_delay   = FALSE;
        received_char = -1;
        if(button != 0)
        {
            SDL_Delay(FIFTY);
            received_char = button;
        }
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                SDL_Quit();
                SDL_GameControllerClose(controller);
                *(args->playing) = 0;
            }
            if(event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP)
            {
                // printf("Button event: button %d %s\n", event.cbutton.button, event.type == SDL_CONTROLLERBUTTONDOWN ? "pressed" : "released");
                if(event.type == SDL_CONTROLLERBUTTONDOWN)
                {
                    received_char = event.cbutton.button;
                    button        = event.cbutton.button;
                    first_delay   = TRUE;
                }
                if(event.type == SDL_CONTROLLERBUTTONUP)
                {
                    button = 0;
                }
            }
            if(event.type == SDL_CONTROLLERAXISMOTION)
            {
                // printf("Axis event: axis %d  position %d\n", event.caxis.axis, event.caxis.value);
            }
        }

        handle_char_res = handle_pressed_char(received_char, args->player, args->lock);
        if(handle_char_res == 1)
        {
            goto error;
        }
        if(handle_char_res == 0 && send_player_info(args->sock_fd, args->player, args->peeraddr))
        {
            goto error;
        }
        if(first_delay)
        {
            SDL_Delay(FIVE_HUNDRED);
        }
    }
    goto done;

error:
    *(args->playing) = 0;
    *return_val      = EXIT_FAILURE;
done:
    SDL_Quit();
    SDL_GameControllerClose(controller);
    return return_val;
}
