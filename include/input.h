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

void input_init(void);

int input_read(VehicleInput *out);

const VehicleInput *read_input(void);

#endif 
