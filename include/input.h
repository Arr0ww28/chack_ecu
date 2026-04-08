#ifndef INPUT_H
#define INPUT_H
#include "types.h"

//Bounds for each variable
#define INPUT_SPEED_MIN   (0)
#define INPUT_SPEED_MAX   (200)
#define INPUT_TEMP_MIN    (-40)
#define INPUT_TEMP_MAX    (150)
#define INPUT_GEAR_MIN    (0)
#define INPUT_GEAR_MAX    (5)

//Populate input from raw signals; initialises internal state
void read_inputs(VehicleInput *input);

//Validate all fields in input against bounds; updates status on rejection
void validate_inputs(VehicleInput *input, VehicleStatus *status);

#endif
