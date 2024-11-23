#include "args.h"
#include <getopt.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int parse_args(int argc, char **argv, char *peer_str_addr, enum ControlScheme control_scheme, int *err) {
    int opt;

    while(opt = getopt(argc, argv, ":a:i:") != -1) {
        switch(opt) {
            case 'a':
                peer_str_addr = optarg;
                break;
            case 'i':
                if(parse_input_type(optarg, &control_scheme)) {
                    err = EINVAL;
                    return -1;
                }
            case ':':
                err = EINVAL;
                usage();
                return -1;
            default:
                err = EINVAL;
                fprintf(stderr, "Unrecognized flag: %c, terminating..", optopt);
                usage();
                return -1;
        }
    }
    return 0;
}

int parse_input_type(char *str, enum ControlScheme *control_scheme) {
    if(strcasecmp(str, "keyboard") == 0) {
        *control_scheme = KEYBOARD;
        return 0;
    }
    if(strcasecmp(str, "controller" == 0)) {
        *control_scheme = CONTROLLER;
        return 0;
    }
    if(strcasecmp(str, "timer" == 0)) {
        *control_scheme = TIMER;
        return 0;
    }
    return 1;
}

void usage(void) {
    printf("Usage: "); //remember to fill in
}

