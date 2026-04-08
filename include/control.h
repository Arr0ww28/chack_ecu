#ifndef CONTROL_H
#define CONTROL_H

#include "types.h"

/* MISRA-C: Thresholds defined as constants to avoid magic numbers */
#define CONTROL_OVERSPEED_THRESHOLD     (120)
#define CONTROL_TEMP_CRITICAL_THRESHOLD (110)
#define CONTROL_TEMP_HIGH_THRESHOLD     (95)
#define CONTROL_GEAR_MAX                (5)

/**
 * @brief Evaluates validated inputs against system thresholds.
 * @param input  Pointer to the validated input snapshot.
 * @param status Pointer to the vehicle status (updated with priority info).
 * @param faults Pointer to the fault status (updates current cycle flags).
 */
void control_run_checks(const VehicleInput *input, VehicleStatus *status, FaultStatus *faults);

#endif /* CONTROL_H */