#include "state.h"
#include "types.h"
#include "mode.h"
#include "fault.h"

#include <stdio.h>

static SystemState s_current_state = NORMAL;

//Log a state transition with a reason string
static void log_transition(SystemState from, SystemState to, const char *reason)
{
    fprintf(stderr, "[STATE] Transition %d -> %d : %s\n",
            (int)from, (int)to, reason);
}

//Initialise all system modules; seed status and faults with safe defaults
void init_system(VehicleStatus *status, FaultStatus *faults)
{
    if (status == NULL || faults == NULL)
    {
        fprintf(stderr, "[STATE] init_system: NULL pointer\n");
        return;
    }

    s_current_state = NORMAL;

    status->system_state          = NORMAL;
    status->active_mode           = MODE_OFF;
    status->current_mode          = MODE_OFF;
    status->previous_mode         = MODE_OFF;
    status->highest_priority_issue = PRIORITY_NONE;

    mode_init();
    fault_init(faults);

    faults->major_fault_count    = 0U;
    faults->warning_count        = 0U;
    faults->critical_fault_count = 0U;
    faults->reset_requested      = 0U;

    fprintf(stderr, "[STATE] System initialised: state=NORMAL, mode=OFF\n");
}

//Evaluate fault counts and update system_state in status; log every transition
void evaluate_system_state(VehicleStatus *status, FaultStatus *faults)
{
    SystemState next_state;

    if (status == NULL || faults == NULL)
    {
        fprintf(stderr, "[STATE] evaluate_system_state: NULL pointer\n");
        return;
    }

    switch (s_current_state)
    {
        case NORMAL:
            if (faults->critical_fault_count >= STATE_CRITICAL_FAULT_THRESHOLD)
            {
                next_state = SAFE;
                log_transition(s_current_state, next_state,
                               "critical fault threshold reached");
                s_current_state      = next_state;
                status->system_state = next_state;
            }
            else if (faults->major_fault_count >= STATE_MAJOR_FAULT_THRESHOLD || faults->warning_count >= STATE_WARNING_REPEAT_THRESHOLD ||
                    faults->critical_fault_count>= 1)
            {
                next_state = DEGRADED;
                log_transition(s_current_state, next_state,
                               "major fault or repeated warnings");
                s_current_state      = next_state;
                status->system_state = next_state;
            }
            else
            {
                //no change — system remains NORMAL
            }
            break;

        case DEGRADED:
            if (faults->critical_fault_count >= STATE_CRITICAL_FAULT_THRESHOLD)
            {
                next_state = SAFE;
                log_transition(s_current_state, next_state,
                               "critical fault count escalated from DEGRADED");
                s_current_state      = next_state;
                status->system_state = next_state;
            }
            else if (faults->major_fault_count == 0U && faults->warning_count == 0U)
            {
                next_state = NORMAL;
                log_transition(s_current_state, next_state,
                               "faults cleared — recovering to NORMAL");
                s_current_state      = next_state;
                status->system_state = next_state;
            }
            else
            {
                //no change — remains DEGRADED
            }
            break;

        case SAFE:
            if (faults->reset_requested      == 1U &&
                faults->critical_fault_count == 0U &&
                faults->major_fault_count    == 0U)
            {
                next_state = NORMAL;
                log_transition(s_current_state, next_state,
                               "explicit reset accepted — all faults cleared");
                s_current_state      = next_state;
                status->system_state = next_state;
            }
            else
            {
                //SAFE is terminal without valid reset — no change
            }
            break;

        default:
            fprintf(stderr, "[STATE] Unknown state %d — forcing SAFE\n",
                    (int)s_current_state);
            s_current_state      = SAFE;
            status->system_state = SAFE;
            break;
    }
}

//Return the current system state
SystemState state_get_current(void)
{
    return s_current_state;
}

//Clear fault counters and arm reset flag so the next evaluate call can exit SAFE
void state_request_reset(FaultStatus *faults)
{
    if (faults == NULL)
    {
        fprintf(stderr, "[STATE] state_request_reset: NULL pointer\n");
        return;
    }

    faults->major_fault_count    = 0U;
    faults->warning_count        = 0U;
    faults->critical_fault_count = 0U;
    faults->reset_requested      = 1U;
}
