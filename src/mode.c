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

//Check if next is a legal transition from current; returns 1 if allowed, 0 if not
static int is_transition_allowed(Mode current, Mode next)
{
    int allowed = 0;

    switch (current)
    {
        case MODE_OFF:
            switch (next)
            {
                case MODE_OFF:
                    allowed = 1;
                    break;
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
                case MODE_ACC:
                    allowed = 1;
                    break;
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
                case MODE_IGNITION_ON:
                    allowed = 1;
                    break;
                case MODE_ACC:
                    allowed = 1;
                    break;
                case MODE_OFF:
                    allowed = 1;
                    break;
                default:
                    allowed = 0;
                    break;
            }
            break;

        default:
            allowed = 0;
            break;
    }

    return allowed;
}

//Read requested mode from input; validate transition; update status and flag fault on illegal request
void update_mode(VehicleStatus *status, VehicleInput *input, FaultStatus *faults)
{
    Mode next_mode;

    if (status == NULL || input == NULL || faults == NULL)
    {
        fprintf(stderr, "[MODE] update_mode: NULL pointer\n");
        return;
    }

    next_mode = input->mode;

    if (next_mode >= MODE_INVALID)
    {
        fprintf(stderr, "[MODE] Out-of-range mode: %d — forcing FAULT\n",
                (int)next_mode);
        s_previous_mode      = s_current_mode;
        s_current_mode       = MODE_FAULT;
        faults->major_fault_count++;
    }
    else if (is_transition_allowed(s_current_mode, next_mode) == 0)
    {
        fprintf(stderr, "[MODE] Illegal transition %d -> %d — forcing FAULT\n",
                (int)s_current_mode, (int)next_mode);
        s_previous_mode      = s_current_mode;
        s_current_mode       = MODE_FAULT;
        faults->major_fault_count++;
    }
    else
    {
        s_previous_mode = s_current_mode;
        s_current_mode  = next_mode;
    }

    status->active_mode   = s_current_mode;
    status->current_mode  = s_current_mode;
    status->previous_mode = s_previous_mode;
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
