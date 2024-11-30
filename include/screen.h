#ifndef SCREEN_H
#define SCREEN_H
#include "../include/game.h"
#include <ncurses.h>
#include <pthread.h>

int  init_screen(WINDOW **win, int x_boundary, int y_boundary);
int  print_message_window(WINDOW *win, const char *msg);
int  clear_window(WINDOW *win);
int  print_player_position(struct PlayerInfo *player, uint16_t *prev_y, uint16_t *prev_x, WINDOW *win, pthread_mutex_t *lock);
void print_end_message(WINDOW *win, uint16_t is_peer_playing);

#endif
