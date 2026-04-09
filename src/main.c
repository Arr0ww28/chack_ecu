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

    printf("========================================\n");
    printf("   VEHICLE ECU SIMULATOR      \n");
    printf("========================================\n");

    init_system(&status, &faults);
    printf("[MAIN] System initialisation complete.\n\n");

    while (1)
    {
        printf("--- NEW CYCLE ---\n");

        //read inputs
        read_inputs(&input);
        printf("[MAIN] Inputs read: speed=%d temp=%d gear=%u mode=%d\n",
               input.speed, input.temperature, (unsigned)input.gear, (int)input.mode);

        //validate inputs
        validate_inputs(&input, &status);
        printf("[MAIN] Inputs validated: speed=%d temp=%d gear=%u mode=%d\n",
               input.speed, input.temperature, (unsigned)input.gear, (int)input.mode);

        //update mode
        update_mode(&status, &input, &faults);
        printf("[MAIN] Mode updated a:ctive=%d previous=%d\n",
               (int)status.active_mode, (int)status.previous_mode);

        //control checks
        run_control_checks(&input, &status, &faults);
        printf("[MAIN] Control checks done: cycle_flags=0x%04X priority=%d\n",
               faults.current_cycle_flags, (int)status.highest_priority_issue);

        //fault status
        update_fault_status(&faults);
        printf("[MAIN] Fault status updated: persistent=0x%04X\n",
               faults.persistent_flags);

        /* Step 6: Evaluate overall system state */
        evaluate_system_state(&status, &faults);
        printf("[MAIN] State evaluated: system_state=%d\n",
               (int)status.system_state);

        /* Step 7: Log full cycle summary */
        log_cycle_summary(&input, &status, &faults);
    }

    return 0;
}
