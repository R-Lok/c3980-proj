#ifndef ARGS_H
#define ARGS_H

#include "../include/threadfuncs.h"

int  parse_args(int argc, char **argv, char **peer_str_addr, input_handler *thread_func, int *err);
void usage(void);

#endif
