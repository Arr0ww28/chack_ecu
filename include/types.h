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
    int16_t speed;        
    int16_t temperature;  
    uint8_t gear;         
    Mode    mode;         
} VehicleInput;

#endif 
/* Add to types.h */
typedef enum {
    PRIORITY_NONE = 0,
    PRIORITY_HIGH_TEMP = 1,
    PRIORITY_OVERSPEED = 2,
    PRIORITY_INVALID_GEAR_MODE = 3,
    PRIORITY_CRITICAL_OVERHEAT = 4
} FaultPriority;

typedef struct {
    Mode current_mode;
    Mode previous_mode;
    FaultPriority highest_priority_issue; /* Used for priority reporting */
} VehicleStatus;

typedef struct {
    uint16_t current_cycle_flags; /* Bits set during the current cycle */
    uint16_t persistent_flags;    /* Latched faults */
    uint8_t  counters[16];        /* Fault persistence tracking */
} FaultStatus;

/* Fault Bits for Bitwise flags */
#define FAULT_BIT_OVERSPEED         (1U << 0)
#define FAULT_BIT_CRITICAL_OVERHEAT (1U << 1)
#define FAULT_BIT_HIGH_TEMP         (1U << 2)
#define FAULT_BIT_INVALID_GEAR      (1U << 3)
#define FAULT_BIT_INVALID_MODE      (1U << 4)