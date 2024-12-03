#include "../include/args.h"
#include "../include/game.h"
#include "../include/screen.h"
#include "../include/socket.h"
#include <ncurses.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_PORT 9999
#define NCURSES_ERROR 1001

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
static volatile sig_atomic_t playing = 1;

void printError(int err, const char *msg);
void handle_signal(int signal);

int main(int argc, char **argv)
{
    char              *peer_str_addr;
    input_handler      thread_func;
    int                err;
    in_port_t          port;
    struct sockaddr_in my_addr;
    struct sockaddr_in peer_addr;
    int                ret;
    int                socket_fd;
    WINDOW            *win;

    struct PlayerInfo my_player;
    struct PlayerInfo peer_player;
    struct GameData   game_data;

    ret           = EXIT_SUCCESS;
    port          = DEFAULT_PORT;
    peer_str_addr = NULL;
    thread_func   = NULL;
    win           = NULL;

    signal(SIGINT, handle_signal);

    if(parse_args(argc, argv, &peer_str_addr, &thread_func, &err))
    {
        printError(err, "Invalid/Missing arguments");
        usage();
        ret = EXIT_FAILURE;
        goto exit_label;
    }
    err = 0;
    if(setup_addrs(&my_addr, &peer_addr, peer_str_addr, port, &err))
    {
        printError(err, "Supplied address is not an IPv4 address");
        usage();
        ret = EXIT_FAILURE;
        goto exit_label;
    }
    err = 0;

    socket_fd = setup_socket(&my_addr, &err);
    if(socket_fd == -1)
    {
        printError(err, "");
        goto exit_label;
    }
    if(init_screen(&win, X_BOUNDARY, Y_BOUNDARY))
    {
        printError(NCURSES_ERROR, "Failed to allocate window using newwin()\n");
        goto fail_screen;
    }
    // printf("win: %i\n", win == NULL);

    setup_player_structs(&my_player, &peer_player);

    // printf("%u, %u, %u, %u\n", my_player.playing, my_player.seq_counter, my_player.x, my_player.y);
    // printf("%u, %u, %u, %u", peer_player.playing, peer_player.seq_counter, peer_player.x, peer_player.y);

    populate_game_data(&game_data, socket_fd, &peer_addr, &my_player, &peer_player, win, &playing);

    ret = wait_for_connection(&game_data);
    if(playing == 0 || ret != 0)
    {
        goto done;
    }

    ret = play_game(&game_data, thread_func);
done:
    delwin(win);
    endwin();
fail_screen:
    close(socket_fd);
exit_label:
    return ret;
}

void printError(int err, const char *msg)
{
    if(err == NCURSES_ERROR)
    {
        fprintf(stderr, "Error: %s", msg);
        return;
    }
    fprintf(stderr, "Error: %s :%s\n", msg, strerror(err));
}

void handle_signal(int signal)
{
    if(signal == SIGINT)
    {
        playing = 0;
    }
}
