#ifndef MODE_H
#define MODE_H

#include "types.h"

//Initialise the mode controller to OFF with no previous mode
void mode_init(void);

//Request a transition to next_mode; forces FAULT on illegal transition
void mode_update(Mode next_mode);

//Return the currently active mode
Mode mode_get_current(void);

//Return the mode that was active before the last transition
Mode mode_get_previous(void);

#endif /* MODE_H */
