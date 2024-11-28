#ifndef THREADFUNCS_H
#define THREADFUNCS_H
#include <pthread.h>

typedef void *(*input_handler)(void *);

void *keyboard_routine(void *thread_args);
void *timer_routine(void *thread_args);
void  process_thread_res(int thread_join_res, int thread_routine_res, int *ret);
int   init_mutex(pthread_mutex_t *lock);

#endif
