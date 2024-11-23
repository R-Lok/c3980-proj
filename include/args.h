#ifndef ARGS_H
#define ARGS_H

enum ControlScheme
{
    NOT_SET,
    KEYBOARD,
    CONTROLLER,
    TIMER,
};

int  parse_args(int argc, char **argv, char **peer_str_addr, enum ControlScheme control_scheme, int *err);
void usage(void);

#endif
