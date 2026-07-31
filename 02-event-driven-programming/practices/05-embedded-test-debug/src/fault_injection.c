#include "fault_injection.h"

#include <stddef.h>

void fault_injection_init(fault_injection_t *faults)
{
    if (faults != NULL)
    {
        faults->enabled_mask = 0U;
        faults->trigger_count = 0U;
    }
}

void fault_injection_enable(
    fault_injection_t *faults,
    fault_injection_id_t fault
)
{
    if (faults != NULL)
    {
        faults->enabled_mask |= (uint32_t)fault;
    }
}

void fault_injection_disable(
    fault_injection_t *faults,
    fault_injection_id_t fault
)
{
    if (faults != NULL)
    {
        faults->enabled_mask &= ~((uint32_t)fault);
    }
}

bool fault_injection_is_enabled(
    const fault_injection_t *faults,
    fault_injection_id_t fault
)
{
    return (faults != NULL) &&
           ((faults->enabled_mask & (uint32_t)fault) != 0U);
}

bool fault_injection_trigger(
    fault_injection_t *faults,
    fault_injection_id_t fault
)
{
    if (!fault_injection_is_enabled(faults, fault))
    {
        return false;
    }

    faults->trigger_count++;
    return true;
}
