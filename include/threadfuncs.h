#ifndef THREADFUNCS_H
#define THREADFUNCS_H
#include <pthread.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-default"
#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wreserved-macro-identifier"
    #pragma clang diagnostic ignored "-Wreserved-identifier"
    #pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif
#include <SDL2/SDL.h>
#pragma GCC diagnostic pop
#ifdef __clang__
    #pragma clang diagnostic pop
#endif

typedef void *(*input_handler)(void *);

void *keyboard_routine(void *thread_args);
void *timer_routine(void *thread_args);
void  process_thread_res(int thread_join_res, int thread_routine_res, int *ret);
int   init_mutex(pthread_mutex_t *lock);
void *handle_peer_routine(void *thread_args);
void *controller_routine(void *thread_args);

#endif
