#include "args.h"
#include <getopt.h>
#include <errno.h>
#include <stdio.h>

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