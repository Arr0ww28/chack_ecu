#include "mode.h"
#include "types.h"

#include <stdio.h>

static Mode s_current_mode  = MODE_OFF;
static Mode s_previous_mode = MODE_OFF;

//Initialise mode controller to OFF
void mode_init(void)
{
    s_current_mode  = MODE_OFF;
    s_previous_mode = MODE_OFF;
}

//Check if next_mode is a legal transition from current; returns 1 if allowed, 0 if not
static int is_transition_allowed(Mode current, Mode next)
{
    int allowed = 0;

    switch (current)
    {
        case MODE_OFF:
            switch (next)
            {
                case MODE_ACC:
                    allowed = 1;
                    break;
                default:
                    allowed = 0;
                    break;
            }
            break;

        case MODE_ACC:
            switch (next)
            {
                case MODE_OFF:
                case MODE_IGNITION_ON:
                    allowed = 1;
                    break;
                default:
                    allowed = 0;
                    break;
            }
            break;

        case MODE_IGNITION_ON:
            switch (next)
            {
                case MODE_OFF:
                    allowed = 1;
                    break;
                default:
                    allowed = 0;
                    break;
            }
            break;

        case MODE_FAULT:
            allowed = 0;
            break;

        default:
            allowed = 0;
            break;
    }

    return allowed;
}

//Request transition to next_mode; forces FAULT on illegal or out-of-range request
void mode_update(Mode next_mode)
{
    if (next_mode >= MODE_INVALID)
    {
        fprintf(stderr, "[MODE] Out-of-range mode requested: %d — forcing FAULT\n",
                (int)next_mode);
        s_previous_mode = s_current_mode;
        s_current_mode  = MODE_FAULT;
        return;
    }

    if (is_transition_allowed(s_current_mode, next_mode) == 0)
    {
        fprintf(stderr, "[MODE] Illegal transition %d -> %d — forcing FAULT\n",
                (int)s_current_mode, (int)next_mode);
        s_previous_mode = s_current_mode;
        s_current_mode  = MODE_FAULT;
        return;
    }

    s_previous_mode = s_current_mode;
    s_current_mode  = next_mode;
}

//Return the currently active mode
Mode mode_get_current(void)
{
    return s_current_mode;
}

//Return the mode active before the last transition
Mode mode_get_previous(void)
{
    return s_previous_mode;
}
