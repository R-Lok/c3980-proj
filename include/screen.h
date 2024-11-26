#ifndef SCREEN_H
#define SCREEN_H
#include <ncurses.h>

int init_screen(WINDOW **win, int x_boundary, int y_boundary);
int print_message_window(WINDOW *win, const char *msg, int y, int x);
int clear_window(WINDOW *win);

#endif
