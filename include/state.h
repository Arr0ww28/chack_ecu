#ifndef STATE_H
#define STATE_H
#include <stdio.h>
#include "types.h"

//Thresholds that drive state transitions
#define STATE_MAJOR_FAULT_THRESHOLD    (1U)
#define STATE_WARNING_REPEAT_THRESHOLD (3U)
#define STATE_CRITICAL_FAULT_THRESHOLD (2U)

//Initialise the state manager; set system state to NORMAL
void state_init(void);

//Evaluate fault context and update system safe-state; log every transition
void evaluate_system_state(const FaultContext *ctx);

//Return the current system safe-state
SystemState state_get_current(void);

//Request a reset from SAFE state; clears counters if ctx permits
void state_request_reset(FaultContext *ctx);

#endif /* STATE_H */
