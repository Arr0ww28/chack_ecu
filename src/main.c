#include "types.h"
#include "input.h"
#include "mode.h"
#include "control.h"
#include "fault.h"
#include "state.h"
#include "log.h"

#include <stdio.h>

int main(void)
{
    VehicleInput  input  = {0};
    VehicleStatus status = {0};
    FaultStatus   faults = {0};

    uint8_t safe_mode_cycle_count = 0;

    printf("---  VEHICLE ECU SIMULATOR  ---\n");

    init_system(&status, &faults);
    printf("[MAIN] System initialisation complete.\n\n");

    while (1)
    {
        printf("--- NEW CYCLE ---\n");

        //read inputs
        read_inputs(&input);
        printf("[MAIN] Inputs read: speed=%d temp=%d gear=%u mode=%d\n",
               input.speed, input.temperature, (unsigned)input.gear, (int)input.mode);

        if ((input.speed == 0 && input.temperature == 0 && input.gear == 0 && input.mode == 0) || (safe_mode_cycle_count >= 2))
        {
            if (safe_mode_cycle_count >= 2)
            {
                printf("[WATCHDOG] 2 continuous SAFE cycles detected. Triggering system reset...\n");
            }
            else
            {
                printf("[WATCHDOG] 0 0 0 0 input detected. Triggering system reset...\n");
            }
            init_system(&status, &faults);
            faults.reset_requested = 1;
            safe_mode_cycle_count = 0;
            
            // Force inputs to safe defaults so the reset cycle evaluates cleanly as NORMAL
            input.speed = 0;
            input.temperature = 0;
            input.gear = 0;
            input.mode = MODE_OFF;
        }
        else
        {
            faults.reset_requested = 0;
        }

        //validate inputs
        validate_inputs(&input, &status, &faults);
        printf("[MAIN] Inputs validated: speed=%d temp=%d gear=%u mode=%d\n",
               input.speed, input.temperature, (unsigned)input.gear, (int)input.mode);

        //update mode
        update_mode(&status, &input, &faults);
        printf("[MAIN] Mode updated: active=%d previous=%d\n",
               (int)status.active_mode, (int)status.previous_mode);

        //control checks
        run_control_checks(&input, &status, &faults);
        printf("[MAIN] Control checks done: cycle_flags=%d%d%d%d%d priority=%d\n",
               (faults.current_cycle_flags & FAULT_BIT_INVALID_MODE) ? 1 : 0,
               (faults.current_cycle_flags & FAULT_BIT_INVALID_GEAR) ? 1 : 0,
               (faults.current_cycle_flags & FAULT_BIT_HIGH_TEMP) ? 1 : 0,
               (faults.current_cycle_flags & FAULT_BIT_CRITICAL_OVERHEAT) ? 1 : 0,
               (faults.current_cycle_flags & FAULT_BIT_OVERSPEED) ? 1 : 0,
               (int)status.highest_priority_issue);

        //fault status
        update_fault_status(&faults);
        printf("[MAIN] Fault status updated: persistent=%d%d%d%d%d\n",
               (faults.persistent_flags & FAULT_BIT_INVALID_MODE) ? 1 : 0,
               (faults.persistent_flags & FAULT_BIT_INVALID_GEAR) ? 1 : 0,
               (faults.persistent_flags & FAULT_BIT_HIGH_TEMP) ? 1 : 0,
               (faults.persistent_flags & FAULT_BIT_CRITICAL_OVERHEAT) ? 1 : 0,
               (faults.persistent_flags & FAULT_BIT_OVERSPEED) ? 1 : 0);

        /* Step 6: Evaluate overall system state */
        evaluate_system_state(&status, &faults);
        printf("[MAIN] State evaluated: system_state=%d\n",
               (int)status.system_state);

        if (status.system_state == SAFE)
        {
            safe_mode_cycle_count++;
        }
        else
        {
            safe_mode_cycle_count = 0;
        }

        //full cycle summary
        log_cycle_summary(&input, &status, &faults);
    }

    return 0;
}
