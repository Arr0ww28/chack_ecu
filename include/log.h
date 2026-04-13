#ifndef LOG_H
#define LOG_H

#include "types.h"

//full log cycle summary prints
void log_cycle_summary(const VehicleInput *input, const VehicleStatus *status, const FaultStatus *faults);

#endif 
