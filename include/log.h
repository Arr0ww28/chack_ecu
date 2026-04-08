#ifndef LOG_H
#define LOG_H

#include "types.h"

/* Prints a full cycle summary covering inputs, mode, faults, and system state */
void log_cycle_summary(const VehicleInput *input, const VehicleStatus *status, const FaultStatus *faults);

#endif /* LOG_H */
