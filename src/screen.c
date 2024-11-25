#include "screen.h"

int init_screen(WINDOW *win, int x_boundary, int y_boundary)
{
    initscr();
    win = newwin(y_boundary + 1, x_boundary + 1, 0, 0);
    if(win == NULL)
    {
        return 1;
    }
    refresh();

    box(win, 0, 0);
    wrefresh(win);
    return 0;
}
