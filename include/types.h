#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

typedef enum
{
    MODE_OFF     = 0,
    MODE_ACC  = 1,
    MODE_IGNITION_ON   = 2,
    MODE_FAULT     = 3,
    MODE_INVALID = 4   
} Mode;

typedef struct
{
    uint16_t speed;        
    int16_t temperature;  
    uint8_t gear;         
    Mode    mode;         
} VehicleInput;

#endif 
