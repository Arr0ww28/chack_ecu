#include "fault.h"
#include "types.h"
#include <stddef.h>
#include <stdio.h>

void fault_init(FaultStatus *faults)
{
    //pointer vaildation
    if (faults == NULL)
    {
        return;
    }

    faults->current_cycle_flags = 0U;
    faults->persistent_flags    = 0U;

    for (uint8_t i = 0U; i < 5U; i++)
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
        //Bitwise clearing using AND and NOT
        faults->current_cycle_flags &= ~fault_bit;
    }
}

void increment_fault_counter(FaultStatus *faults, uint8_t fault_index)
{
    if (faults != NULL)
    {
    
        faults->counters[fault_index]++;        
        
    }
}

void update_fault_status(FaultStatus *faults)
{
    if (faults == NULL)
    {
        fprintf(stderr, "[FAULT] ERR: Null pointer provided\n");
        return;
    }

    //Dynamic iteration through fault bits
    for (uint8_t i = 0U; i < FAULT_MAX_TRACKED_BITS; i++)
    {
        uint16_t current_mask = (uint16_t)(1U << i);

        //check if fault bit is set in current cycle
        if ((faults->current_cycle_flags & current_mask) != 0U)
        {
            //increment counter
            increment_fault_counter(faults, i);

            //check persistence threshold
            if (faults->counters[i] >= FAULT_PERSISTENCE_THRESHOLD)
            {
                faults->persistent_flags |= current_mask;
            }
        }
        else
        {
            faults->counters[i] = 0U;
        }
    }
}