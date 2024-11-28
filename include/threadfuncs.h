#ifndef THREADFUNCS_H
#define THREADFUNCS_H

typedef void *(*input_handler)(void *);

void *keyboard_routine(void *thread_args);

#endif
