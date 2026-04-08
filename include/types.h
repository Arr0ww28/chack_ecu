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

typedef enum
{
    SAFE_STATE_NORMAL   = 0,
    SAFE_STATE_DEGRADED = 1,
    SAFE_STATE_SAFE     = 2
} SystemState;

typedef struct
{
    uint8_t major_fault_count;
    uint8_t warning_count;
    uint8_t critical_fault_count;
    uint8_t reset_requested;
} FaultStatus;

#endif 
