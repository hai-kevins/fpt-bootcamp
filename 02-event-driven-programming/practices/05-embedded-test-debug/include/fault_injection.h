#ifndef FAULT_INJECTION_H
#define FAULT_INJECTION_H

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    FAULT_INJECTION_NONE = 0U,
    FAULT_INJECTION_POOL_FAILURE = 1U << 0,
    FAULT_INJECTION_CRC_ERROR = 1U << 1,
    FAULT_INJECTION_SENSOR_TIMEOUT = 1U << 2,
    FAULT_INJECTION_FLASH_FAILURE = 1U << 3
} fault_injection_id_t;

typedef struct
{
    uint32_t enabled_mask;
    uint32_t trigger_count;
} fault_injection_t;

void fault_injection_init(fault_injection_t *faults);
void fault_injection_enable(fault_injection_t *faults, fault_injection_id_t fault);
void fault_injection_disable(fault_injection_t *faults, fault_injection_id_t fault);
bool fault_injection_is_enabled(const fault_injection_t *faults, fault_injection_id_t fault);
bool fault_injection_trigger(fault_injection_t *faults, fault_injection_id_t fault);

#endif
