#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

enum
{
    POOL_FAIL = 1U << 0,
    CRC_ERROR = 1U << 1,
    SENSOR_TIMEOUT = 1U << 2,
    FLASH_FAIL = 1U << 3
};

typedef struct
{
    uint32_t enabled;
    uint32_t triggers;
} faults_t;

static bool trigger(faults_t *faults, uint32_t fault)
{
    if ((faults->enabled & fault) == 0U)
    {
        return false;
    }

    faults->triggers++;
    return true;
}

int main(void)
{
    faults_t faults =
    {
        .enabled = POOL_FAIL | CRC_ERROR | SENSOR_TIMEOUT | FLASH_FAIL
    };

    const bool pool = trigger(&faults, POOL_FAIL);
    const bool crc = trigger(&faults, CRC_ERROR);
    const bool sensor = trigger(&faults, SENSOR_TIMEOUT);
    const bool flash = trigger(&faults, FLASH_FAIL);

    const bool pass = pool && crc && sensor && flash && (faults.triggers == 4U);

    (void) printf("pool=%u crc=%u sensor=%u flash=%u ""triggers=%lu %s\n", pool ? 1U : 0U, crc ? 1U : 0U, sensor ? 1U : 0U,
        flash ? 1U : 0U, (unsigned long) faults.triggers, pass ? "PASS" : "FAIL");

    return pass ? 0 : 1;
}
