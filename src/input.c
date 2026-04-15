#include "input.h"
#include "types.h"
#include "fault.h"

#include <stdio.h>
#include <string.h>

#define INPUT_LINE_BUF 256

static uint16_t s_last_speed = 0;
static int16_t s_last_temp  = 0;
static uint8_t s_last_gear  = 0;
static Mode    s_last_mode  = MODE_OFF;

//Populate input struct from raw signal sources; resets fields to zero on entry
void read_inputs(VehicleInput *input)
{
    if (input == NULL)
    {
        fprintf(stderr, "[INPUT] read_inputs: NULL pointer\n");
        return;
    }

    char line[INPUT_LINE_BUF];
    int speed = 0, temp = 0, gear = 0, mode = 0, extra = 0;

    while (1)
    {
        printf("\n[Input] Enter Speed, Temp, Gear, Mode (0=OFF, 1=ACC, 2=IGN_ON, 3=FAULT): ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            fprintf(stderr, "[INPUT] EOF on stdin, defaulting to 0 / MODE_OFF\n");
            input->speed       = 0;
            input->temperature = 0;
            input->gear        = 0;
            input->mode        = MODE_OFF;
            return;
        }

        int parsed = sscanf(line, "%d %d %d %d %d", &speed, &temp, &gear, &mode, &extra);

        if (parsed == 4)
        {
            // Exactly 4 valid inputs — accept
            input->speed       = (uint16_t)speed;
            input->temperature = (int16_t)temp;
            input->gear        = (uint8_t)gear;
            input->mode        = (Mode)mode;
            return;
        }
        else if (parsed > 4)
        {
            fprintf(stderr, "[INPUT] Too many inputs (expected 4). Try again.\n");
        }
        else
        {
            fprintf(stderr, "[INPUT] Invalid input (expected 4). Try again.\n");
        }
    }
}

//Validate all fields in input against bounds; sets fault bits on detection, then corrects to last valid
void validate_inputs(VehicleInput *input, VehicleStatus *status, FaultStatus *faults)
{
    if (input == NULL || status == NULL || faults == NULL)
    {
        fprintf(stderr, "[INPUT] NULL pointer\n");
        return;
    }

    faults->current_cycle_flags &= ~(FAULT_BIT_OVERSPEED | 
                                     FAULT_BIT_CRITICAL_OVERHEAT | 
                                     FAULT_BIT_HIGH_TEMP | FAULT_BIT_INVALID_GEAR | FAULT_BIT_INVALID_MODE);


    if (input->speed > INPUT_SPEED_MAX)
    {
        fprintf(stderr, "[INPUT] Invalid speed: %d - retaining last valid (%d)\n",
                input->speed, s_last_speed);
        input->speed = s_last_speed;
    }
    else
    {
        s_last_speed = input->speed;
    }

    //temperature validation
    if (input->temperature < INPUT_TEMP_MIN || input->temperature > INPUT_TEMP_MAX)
    {
        fprintf(stderr, "[INPUT] Invalid temperature: %d - retaining last valid (%d)\n",
                input->temperature, s_last_temp);
        input->temperature = s_last_temp;
    }
    else
    {
        s_last_temp = input->temperature;
    }

    if (input->gear > INPUT_GEAR_MAX)
    {
        faults->current_cycle_flags |= FAULT_BIT_INVALID_GEAR;
        fprintf(stderr, "[INPUT] Invalid gear: %u - retaining last valid (%u)\n",
                input->gear, s_last_gear);
        input->gear = s_last_gear;
    }
    else
    {
        s_last_gear = input->gear;
    }

    if (input->mode >= MODE_INVALID)
    {
        faults->current_cycle_flags |= FAULT_BIT_INVALID_MODE;
        fprintf(stderr, "[INPUT] Invalid mode: %d - retaining last valid (%d)\n",
                (int)input->mode, (int)s_last_mode);
        input->mode = s_last_mode;
    }
    else
    {
        s_last_mode    = input->mode;
    }
}
