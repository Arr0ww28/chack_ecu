#include "input.h"
#include "types.h"
#include <stddef.h>
#include <stdio.h>

static int16_t s_last_speed = 0;
static int16_t s_last_temp  = 0;
static uint8_t s_last_gear  = 0;
static Mode    s_last_mode  = MODE_OFF;
static int     s_initialised = 0;

void input_init(void)
{
    s_last_speed  = 0;
    s_last_temp   = 0;
    s_last_gear   = 0;
    s_last_mode   = MODE_OFF;
    s_initialised = 1;
}

//read inputs and validate
int input_read(VehicleInput *out)
{
    int     status = 0;
    int16_t raw_speed;
    int16_t raw_temp;
    uint8_t raw_gear;
    Mode    raw_mode;

    if (out == NULL)
    {
        fprintf(stderr, "[INPUT] input_read: NULL output pointer\n");
        return -1;
    }

    raw_speed = out->speed;
    raw_temp  = out->temperature;
    raw_gear  = out->gear;
    raw_mode  = out->mode;

    //speed validation
    if (raw_speed < INPUT_SPEED_MIN || raw_speed > INPUT_SPEED_MAX)
    {
        fprintf(stderr, "[INPUT] Invalid speed: %d — retaining last valid (%d)\n",
                raw_speed, s_last_speed);
        out->speed = s_last_speed;
        status     = -1;
    }
    else
    {
        s_last_speed = raw_speed;
        out->speed   = raw_speed;
    }

    //temperature validation
    if (raw_temp < INPUT_TEMP_MIN || raw_temp > INPUT_TEMP_MAX)
    {
        fprintf(stderr, "[INPUT] Invalid temperature: %d — retaining last valid (%d)\n",
                raw_temp, s_last_temp);
        out->temperature = s_last_temp;
        status           = -1;
    }
    else
    {
        s_last_temp      = raw_temp;
        out->temperature = raw_temp;
    }

    if (raw_gear < INPUT_GEAR_MIN || raw_gear > INPUT_GEAR_MAX)
    {
        fprintf(stderr, "[INPUT] Invalid gear: %u — retaining last valid (%u)\n",
                raw_gear, s_last_gear);
        out->gear = s_last_gear;
        status    = -1;
    }
    else
    {
        s_last_gear = raw_gear;
        out->gear   = raw_gear;
    }

    //mode validation
    if (raw_mode >= MODE_INVALID)
    {
        fprintf(stderr, "[INPUT] Invalid mode: %d — retaining last valid (%d)\n",
                (int)raw_mode, (int)s_last_mode);
        out->mode = s_last_mode;
        status    = -1;
    }
    else
    {
        s_last_mode = raw_mode;
        out->mode   = raw_mode;
    }

    return status;
}

//return last valid input — compose snapshot from individual statics
const VehicleInput *input_get_last_valid(void)
{
    static VehicleInput s_snapshot;
    s_snapshot.speed       = s_last_speed;
    s_snapshot.temperature = s_last_temp;
    s_snapshot.gear        = s_last_gear;
    s_snapshot.mode        = s_last_mode;
    return &s_snapshot;
}
