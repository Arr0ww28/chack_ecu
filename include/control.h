#ifndef CONTROL_H
#define CONTROL_H

#include "types.h"

#define CONTROL_OVERSPEED_THRESHOLD     (120)
#define CONTROL_TEMP_CRITICAL_THRESHOLD (110)
#define CONTROL_TEMP_HIGH_THRESHOLD     (95)
#define CONTROL_GEAR_MAX                (5)

/**
 * @brief 
 * @param input 
 * @param status 
 * @param faults 
 */
void run_control_checks(const VehicleInput *input, VehicleStatus *status, FaultStatus *faults);

#endif //control.h