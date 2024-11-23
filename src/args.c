#include "args.h"
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

int parse_input_type(const char *str, enum ControlScheme *control_scheme);
int check_args(const char *peer_str_addr, const enum ControlScheme *control_scheme, int *err);

int parse_args(int argc, char **argv, char **peer_str_addr, enum ControlScheme control_scheme, int *err)
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
                if(parse_input_type(optarg, &control_scheme))
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
    return check_args(*peer_str_addr, &control_scheme, err);
}

int parse_input_type(const char *str, enum ControlScheme *control_scheme)
{
    if(strcasecmp(str, "keyboard") == 0)
    {
        *control_scheme = KEYBOARD;
        return 0;
    }
    if(strcasecmp(str, "controller") == 0)
    {
        *control_scheme = CONTROLLER;
        return 0;
    }
    if(strcasecmp(str, "timer") == 0)
    {
        *control_scheme = TIMER;
        return 0;
    }
    return 1;
}

int check_args(const char *peer_str_addr, const enum ControlScheme *control_scheme, int *err)
{
    if(peer_str_addr == NULL || *control_scheme == NOT_SET)
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
