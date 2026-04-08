#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

typedef enum
{
    MODE_OFF     = 0,
    MODE_NORMAL  = 1,
    MODE_SPORT   = 2,
    MODE_ECO     = 3,
    MODE_INVALID = 4   
} Mode;

typedef struct
{
    int16_t speed;        
    int16_t temperature;  
    uint8_t gear;         
    Mode    mode;         
} VehicleInput;

#endif 
