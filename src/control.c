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

    //Temp
    if (input->temperature > CONTROL_TEMP_CRITICAL_THRESHOLD)
    {
        faults->current_cycle_flags |= FAULT_BIT_CRITICAL_OVERHEAT;
        faults->critical_fault_count++;
        faults->major_fault_count++;
        
    }

    else if (input->temperature > CONTROL_TEMP_HIGH_THRESHOLD)
    {
        faults->current_cycle_flags |= FAULT_BIT_HIGH_TEMP;
        faults->warning_count++;
        //faults->major_fault_count++;
    }
    //Overspeed
    if (input->speed > CONTROL_OVERSPEED_THRESHOLD)
    {
        faults->current_cycle_flags |= FAULT_BIT_OVERSPEED;
        faults->major_fault_count++;
    }
    //else if (input->speed >= CONTROL_WARNING_SPEED)
    //{
    //    faults->current_cycle_flags |= FAULT_BIT_WARNING_SPEED;
    //    faults->warning_count++;
    //}

    //Priority handling
    if ((faults->current_cycle_flags & FAULT_BIT_CRITICAL_OVERHEAT) != 0U)
    {
        status->highest_priority_issue = PRIORITY_CRITICAL_OVERHEAT;
    }
    else if (((faults->current_cycle_flags & FAULT_BIT_INVALID_GEAR) != 0U) || 
             ((faults->current_cycle_flags & FAULT_BIT_INVALID_MODE) != 0U))
    {
        status->highest_priority_issue = PRIORITY_INVALID_GEAR_MODE;
        faults->major_fault_count++;
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