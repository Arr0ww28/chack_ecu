#ifndef FAULT_H
#define FAULT_H

#include "types.h"

#define FAULT_PERSISTENCE_THRESHOLD (3U)

//total number of fault bits
#define FAULT_MAX_TRACKED_BITS      (5U)
#define FAULT_BIT_WARNING_TEMP (1U << 5)
#define FAULT_BIT_WARNING_SPEED (1U << 6)

/**
 * @brief Initializes the fault status structure and clears all counters.
 * @param faults Pointer to the fault status structure.
 */
void fault_init(FaultStatus *faults);

/**
 * @brief Sets a specific fault bit in the current cycle flags.
 * @param faults Pointer to the fault status structure.
 * @param fault_bit The bitmask of the fault to set.
 */
void set_fault(FaultStatus *faults, uint16_t fault_bit);

/**
 * @brief Clears a specific fault bit from the current cycle flags.
 * @param faults Pointer to the fault status structure.
 * @param fault_bit The bitmask of the fault to clear.
 */
void clear_fault(FaultStatus *faults, uint16_t fault_bit);

/**
 * @brief Safely increments the counter for a specific fault index.
 * @param faults Pointer to the fault status structure.
 * @param fault_index The array index corresponding to the fault bit position.
 */
void increment_fault_counter(FaultStatus *faults, uint8_t fault_index);

/**
 * @brief Evaluates current cycle flags, updates counters, and latches persistent faults.
 * @param faults Pointer to the fault status structure.
 */
void update_fault_status(FaultStatus *faults);

#endif /* FAULT_H */