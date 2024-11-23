#ifndef ARGS_H
#define ARGS_H

enum ControlScheme
{
    KEYBOARD,
    CONTROLLER,
    TIMER,
};

int parse_args(int argc, char **argv, char *peer_str_addr, enum ControlScheme control_scheme, int *err);

#endif