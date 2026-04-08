#include "state.h"
#include "types.h"

#include <stdio.h>

static SystemState s_current_state = SAFE_STATE_NORMAL;

//Initialise the state manager to NORMAL
void state_init(void)
{
    s_current_state = SAFE_STATE_NORMAL;
}

//Log a state transition with a human-readable reason string
static void log_transition(SystemState from, SystemState to, const char *reason)
{
    fprintf(stderr, "[STATE] Transition %d -> %d : %s\n",
            (int)from, (int)to, reason);
}

//Evaluate the fault context and drive the safe-state machine; log every change
void evaluate_system_state(const FaultContext *ctx)
{
    SystemState next_state;

    if (ctx == NULL)
    {
        fprintf(stderr, "[STATE] evaluate_system_state: NULL context pointer\n");
        return;
    }

    switch (s_current_state)
    {
        case SAFE_STATE_NORMAL:
            if (ctx->critical_fault_count >= STATE_CRITICAL_FAULT_THRESHOLD)
            {
                next_state = SAFE_STATE_SAFE;
                log_transition(s_current_state, next_state,
                               "critical fault threshold reached");
                s_current_state = next_state;
            }
            else if (ctx->major_fault_count >= STATE_MAJOR_FAULT_THRESHOLD ||
                     ctx->warning_count     >= STATE_WARNING_REPEAT_THRESHOLD)
            {
                next_state = SAFE_STATE_DEGRADED;
                log_transition(s_current_state, next_state,
                               "major fault or repeated warnings");
                s_current_state = next_state;
            }
            else
            {
                //no change — system remains NORMAL
            }
            break;

        case SAFE_STATE_DEGRADED:
            if (ctx->critical_fault_count >= STATE_CRITICAL_FAULT_THRESHOLD)
            {
                next_state = SAFE_STATE_SAFE;
                log_transition(s_current_state, next_state,
                               "critical fault count escalated from DEGRADED");
                s_current_state = next_state;
            }
            else if (ctx->major_fault_count == 0U && ctx->warning_count == 0U)
            {
                next_state = SAFE_STATE_NORMAL;
                log_transition(s_current_state, next_state,
                               "faults cleared — recovering to NORMAL");
                s_current_state = next_state;
            }
            else
            {
                //no change — remains DEGRADED
            }
            break;

        case SAFE_STATE_SAFE:
            if (ctx->reset_requested == 1U &&
                ctx->critical_fault_count == 0U &&
                ctx->major_fault_count    == 0U)
            {
                next_state = SAFE_STATE_NORMAL;
                log_transition(s_current_state, next_state,
                               "explicit reset accepted — all faults cleared");
                s_current_state = next_state;
            }
            else
            {
                //SAFE is terminal without valid reset — no change
            }
            break;

        default:
            fprintf(stderr, "[STATE] Unknown state %d — forcing SAFE\n",
                    (int)s_current_state);
            s_current_state = SAFE_STATE_SAFE;
            break;
    }
}

//Return the current system safe-state
SystemState state_get_current(void)
{
    return s_current_state;
}

//Clear fault counters and mark reset complete if conditions allow exit from SAFE
void state_request_reset(FaultContext *ctx)
{
    if (ctx == NULL)
    {
        fprintf(stderr, "[STATE] state_request_reset: NULL context pointer\n");
        return;
    }

    ctx->major_fault_count    = 0U;
    ctx->warning_count        = 0U;
    ctx->critical_fault_count = 0U;
    ctx->reset_requested      = 1U;
}
