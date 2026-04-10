#include "control.h"
#include "fault.h"
#include "types.h"
#include <stddef.h>
#include <stdio.h>

void run_control_checks(const VehicleInput *input, VehicleStatus *status, FaultStatus *faults)
{
    if ((input == NULL) || (status == NULL) || (faults == NULL))
    {
        fprintf(stderr, "[CONTROL] ERR: Null pointer provided\n");
        return;
    }

    faults->current_cycle_flags &= ~(FAULT_BIT_OVERSPEED | 
                                     FAULT_BIT_CRITICAL_OVERHEAT | 
                                     FAULT_BIT_HIGH_TEMP);

    /* 1. Evaluate Temperature */
    if (input->temperature > CONTROL_TEMP_CRITICAL_THRESHOLD)
    {
        faults->current_cycle_flags |= FAULT_BIT_CRITICAL_OVERHEAT;
        faults->critical_fault_count++;
        
    }
    else if (input->temperature > CONTROL_TEMP_HIGH_THRESHOLD)
    {
        faults->current_cycle_flags |= FAULT_BIT_HIGH_TEMP;
        faults->major_fault_count++;
    }
    else if (input->temperature >= CONTROL_WARNING_TEMP)
    {
        faults->current_cycle_flags |= FAULT_BIT_WARNING_TEMP;
        faults->warning_count++;
    }
    /* 2. Evaluate Overspeed */
    if (input->speed > CONTROL_OVERSPEED_THRESHOLD)
    {
        faults->current_cycle_flags |= FAULT_BIT_OVERSPEED;
        faults->major_fault_count++;
    }
    else if (input->speed >= CONTROL_WARNING_SPEED)
    {
        faults->current_cycle_flags |= FAULT_BIT_WARNING_SPEED;
        faults->warning_count++;
    }

    /* * ---------------------------------------------------------
     * PRIORITY HANDLING (As required by Hackathon Guidelines)
     * ---------------------------------------------------------
     * If multiple issues occur, we determine the highest priority
     * for reporting/logging purposes.
     */
    
    if ((faults->current_cycle_flags & FAULT_BIT_CRITICAL_OVERHEAT) != 0U)
    {
        status->highest_priority_issue = PRIORITY_CRITICAL_OVERHEAT;
    }
    else if (((faults->current_cycle_flags & FAULT_BIT_INVALID_GEAR) != 0U) || 
             ((faults->current_cycle_flags & FAULT_BIT_INVALID_MODE) != 0U))
    {
        status->highest_priority_issue = PRIORITY_INVALID_GEAR_MODE;
    }
    else if ((faults->current_cycle_flags & FAULT_BIT_OVERSPEED) != 0U)
    {
        status->highest_priority_issue = PRIORITY_OVERSPEED;
    }
    else if ((faults->current_cycle_flags & FAULT_BIT_HIGH_TEMP) != 0U)
    {
        status->highest_priority_issue = PRIORITY_HIGH_TEMP;
    }
    else
    {
        status->highest_priority_issue = PRIORITY_NONE;
    }
}