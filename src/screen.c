#include "screen.h"

int init_screen(WINDOW **win, int x_boundary, int y_boundary)
{
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    cbreak();
    curs_set(0);
    *win = newwin(y_boundary + 1, x_boundary + 1, 0, 0);
    if(*win == NULL)
    {
        return 1;
    }
    refresh();

    box(*win, 0, 0);
    wrefresh(*win);
    return 0;
}

int print_message_window(WINDOW *win, const char *msg)
{
    int x;
    int y;

    x = (X_BOUNDARY - (int)strlen(msg)) / 2;
    y = Y_BOUNDARY / 2;
    if(mvwprintw(win, y, x, "%s", msg))
    {
        fprintf(stderr, "mvwprintw error (ncurses)\n");
        return 1;
    }
    if(wrefresh(win))
    {
        fprintf(stderr, "print_message_window: wrefresh error (ncurses)\n");
        return 1;
    }
    return 0;
}

int clear_window(WINDOW *win)
{
    if(wclear(win))
    {
        fprintf(stderr, "wclear error (ncurses)\n");
        return 1;
    }
    if(wrefresh(win))
    {
        fprintf(stderr, "clear_window: wrefresh error (ncurses)\n");
        return 1;
    }
    return 0;
}

int print_player_position(struct PlayerInfo *player, uint16_t *prev_y, uint16_t *prev_x, WINDOW *win, pthread_mutex_t *lock)
{
    if(pthread_mutex_lock(lock))
    {
        fprintf(stdout, "pthread_mutex_lock error\n");
        return 1;
    }
    mvwprintw(win, *prev_y, *prev_x, " ");
    mvwprintw(win, player->y, player->x, "*");
    *prev_y = player->y;
    *prev_x = player->x;
    if(pthread_mutex_unlock(lock))
    {
        fprintf(stdout, "pthread_mutex_unlock error\n");
        return 1;
    }
    return 0;
}

void print_end_message(WINDOW *win, uint16_t is_peer_playing)
{
    if(is_peer_playing == 0)
    {
        print_message_window(win, "Other player disconnected - Press any key to exit.");
    }
    else
    {
        print_message_window(win, "Ended session - Press any key to exit.");
    }
}
