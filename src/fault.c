#include "fault.h"
#include "types.h"
#include <stddef.h>
#include <stdio.h>

void fault_init(FaultStatus *faults)
{
    /* CERT MEM10-C: Pointer validation */
    if (faults == NULL)
    {
        return;
    }

    faults->current_cycle_flags = 0U;
    faults->persistent_flags    = 0U;

    for (uint8_t i = 0U; i < 16U; i++)
    {
        faults->counters[i] = 0U;
    }
}

void set_fault(FaultStatus *faults, uint16_t fault_bit)
{
    if (faults != NULL)
    {
        faults->current_cycle_flags |= fault_bit;
    }
}

void clear_fault(FaultStatus *faults, uint16_t fault_bit)
{
    if (faults != NULL)
    {
        /* MISRA-C: Safe bitwise clearing using AND + NOT */
        faults->current_cycle_flags &= ~fault_bit;
    }
}

void increment_fault_counter(FaultStatus *faults, uint8_t fault_index)
{
    if (faults != NULL)
    {
        /* Defensive Programming: Prevent out-of-bounds array access */
        if (fault_index < 16U)
        {
            /* MISRA-C / CERT INT32-C: Prevent integer overflow of the uint8_t counter */
            if (faults->counters[fault_index] < 255U)
            {
                faults->counters[fault_index]++;
            }
        }
    }
}

void update_fault_status(FaultStatus *faults)
{
    if (faults == NULL)
    {
        fprintf(stderr, "[FAULT] ERR: Null pointer provided to update_fault_status\n");
        return;
    }

    /* Iterate dynamically through all supported fault bit positions */
    for (uint8_t i = 0U; i < FAULT_MAX_TRACKED_BITS; i++)
    {
        uint16_t current_mask = (uint16_t)(1U << i);

        /* 1. Check if the fault is active in the CURRENT cycle */
        if ((faults->current_cycle_flags & current_mask) != 0U)
        {
            /* 2. Increment the associated counter */
            increment_fault_counter(faults, i);

            /* 3. Check for Persistence Tracking (Mandatory Test Case 8) */
            if (faults->counters[i] >= FAULT_PERSISTENCE_THRESHOLD)
            {
                /* Latch the fault into the persistent tracker */
                faults->persistent_flags |= current_mask;
            }
        }
        else
        {
            /* * Debouncing Logic: If the fault is NOT present this cycle, 
             * reset the consecutive occurrence counter. 
             * Note: The persistent_flag remains latched until a formal system reset.
             */
            faults->counters[i] = 0U;
        }
    }
}