#ifndef MODE_H
#define MODE_H

#include "types.h"

//Initialise mode controller to OFF with no previous mode
void mode_init(void);

//Read requested mode from input and drive transition; updates status and flags faults
void update_mode(VehicleStatus *status, VehicleInput *input, FaultStatus *faults);

//Return the currently active mode
Mode mode_get_current(void);

//Return the mode active before the last transition
Mode mode_get_previous(void);

#endif /* MODE_H */
