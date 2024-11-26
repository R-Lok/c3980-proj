#include "../include/args.h"
#include "../include/game.h"
#include "../include/screen.h"
#include "../include/socket.h"
#include <ncurses.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_PORT 9999
#define X_BOUNDARY 100
#define Y_BOUNDARY 50
#define NCURSES_ERROR 1001

void printError(int err, const char *msg);

int main(int argc, char **argv)
{
    char              *peer_str_addr;
    enum ControlScheme control_scheme;
    int                err;
    in_port_t          port;
    struct sockaddr_in my_addr;
    struct sockaddr_in peer_addr;
    int                ret;
    int                socket_fd;
    WINDOW            *win;
    uint16_t           playing;

    struct PlayerInfo my_player;
    struct PlayerInfo peer_player;

    ret            = EXIT_SUCCESS;
    port           = DEFAULT_PORT;
    peer_str_addr  = NULL;
    control_scheme = NOT_SET;
    win            = NULL;
    playing        = 1;

    if(parse_args(argc, argv, &peer_str_addr, &control_scheme, &err))
    {
        printError(err, "Error parsing arguments\n");
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
    }
    // printf("win: %i\n", win == NULL);

    setup_player_structs(&my_player, &peer_player, X_BOUNDARY, Y_BOUNDARY);

    // printf("%u, %u, %u, %u\n", my_player.playing, my_player.seq_counter, my_player.x, my_player.y);
    // printf("%u, %u, %u, %u", peer_player.playing, peer_player.seq_counter, peer_player.x, peer_player.y);

    wait_for_connection(socket_fd, &peer_addr, &my_player, &peer_player, win, &playing);    // need var to get result, check result

    getch();

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
