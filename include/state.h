#ifndef STATE_H
#define STATE_H

#include "types.h"

//Thresholds that drive state transitions
#define STATE_MAJOR_FAULT_THRESHOLD    (1U)
#define STATE_WARNING_REPEAT_THRESHOLD (3U)
#define STATE_CRITICAL_FAULT_THRESHOLD (2U)

//Initialise all system modules; seed status and faults with safe defaults
void init_system(VehicleStatus *status, FaultStatus *faults);

//Evaluate fault counts in faults and update system_state in status; log every change
void evaluate_system_state(VehicleStatus *status, FaultStatus *faults);

//Return the current system state
SystemState state_get_current(void);

//Clear fault counters and arm reset flag so the next evaluation can exit SAFE
void state_request_reset(FaultStatus *faults);

#endif /* STATE_H */
