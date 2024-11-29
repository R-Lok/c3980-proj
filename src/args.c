#include "args.h"
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

int parse_input_type(const char *str, input_handler *thread_func);
int check_args(const char *peer_str_addr, const input_handler *thread_func, int *err);

int parse_args(int argc, char **argv, char **peer_str_addr, input_handler *thread_func, int *err)
{
    int opt;

    while((opt = getopt(argc, argv, ":a:i:")) != -1)
    {
        switch(opt)
        {
            case 'a':
                *peer_str_addr = optarg;
                break;
            case 'i':
                if(parse_input_type(optarg, thread_func))
                {
                    *err = EINVAL;
                    return 1;
                }
                break;
            case ':':
                *err = EINVAL;
                return 1;
            default:
                *err = EINVAL;
                fprintf(stderr, "Unrecognized flag: %c, terminating..", optopt);
                return 1;
        }
    }
    return check_args(*peer_str_addr, thread_func, err);
}

int parse_input_type(const char *str, input_handler *thread_func)
{
    if(strcasecmp(str, "keyboard") == 0)
    {
        *thread_func = keyboard_routine;
        return 0;
    }
    if(strcasecmp(str, "controller") == 0)
    {
        // *thread_func = controller_routine;
        return 0;
    }
    if(strcasecmp(str, "timer") == 0)
    {
        *thread_func = timer_routine;
        return 0;
    }
    return 1;
}

int check_args(const char *peer_str_addr, const input_handler *thread_func, int *err)
{
    if(peer_str_addr == NULL || *thread_func == NULL)
    {
        *err = EINVAL;
        return 1;
    }
    return 0;
}

void usage(void)
{
    printf("Usage: ");    // remember to fill in
}
