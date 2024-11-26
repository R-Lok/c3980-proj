#include "screen.h"

int init_screen(WINDOW **win, int x_boundary, int y_boundary)
{
    initscr();
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

int print_message_window(WINDOW *win, const char *msg, int y, int x)
{
    // printf("%i\n", win == NULL);
    if(mvwprintw(win, y, x, msg))
    {    // add err message/print for both
        return 1;
    }
    if(wrefresh(win))
    {
        return 1;
    }
    return 0;
}

int clear_window(WINDOW *win)
{
    if(wclear(win))
    {    // add err message/print for both
        return 1;
    }
    if(wrefresh(win))
    {
        return 1;
    }
    return 0;
}
