#include "input.h"
#include "types.h"

#include <stdio.h>

//private state — one variable per validated signal
static int16_t s_last_speed = 0;
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

    input->speed       = 0;
    input->temperature = 0;
    input->gear        = 0;
    input->mode        = MODE_OFF;
}

//Validate all fields in input against bounds; preserve last valid and update status on rejection
void validate_inputs(VehicleInput *input, VehicleStatus *status)
{
    if (input == NULL || status == NULL)
    {
        fprintf(stderr, "[INPUT] validate_inputs: NULL pointer\n");
        return;
    }

    //speed validation
    if (input->speed < INPUT_SPEED_MIN || input->speed > INPUT_SPEED_MAX)
    {
        fprintf(stderr, "[INPUT] Invalid speed: %d — retaining last valid (%d)\n",
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
        fprintf(stderr, "[INPUT] Invalid temperature: %d — retaining last valid (%d)\n",
                input->temperature, s_last_temp);
        input->temperature = s_last_temp;
    }
    else
    {
        s_last_temp = input->temperature;
    }

    //gear validation
    if (input->gear < INPUT_GEAR_MIN || input->gear > INPUT_GEAR_MAX)
    {
        fprintf(stderr, "[INPUT] Invalid gear: %u — retaining last valid (%u)\n",
                input->gear, s_last_gear);
        input->gear = s_last_gear;
    }
    else
    {
        s_last_gear = input->gear;
    }

    //mode validation
    if (input->mode >= MODE_INVALID)
    {
        fprintf(stderr, "[INPUT] Invalid mode: %d — retaining last valid (%d)\n",
                (int)input->mode, (int)s_last_mode);
        input->mode = s_last_mode;
    }
    else
    {
        s_last_mode    = input->mode;
        status->active_mode = input->mode;
    }
}
