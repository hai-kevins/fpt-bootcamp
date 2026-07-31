#include "crash_record.h"

#include <stddef.h>
#include <string.h>

static uint32_t checksum_bytes(
    const uint8_t *data,
    size_t length
)
{
    uint32_t hash = 2166136261UL;

    for (size_t i = 0U; i < length; i++)
    {
        hash ^= data[i];
        hash *= 16777619UL;
    }

    return hash;
}

void crash_record_prepare(
    crash_record_t *record
)
{
    if (record == NULL)
    {
        return;
    }

    (void)memset(record, 0, sizeof(*record));
    record->magic = CRASH_RECORD_MAGIC;
    record->version = CRASH_RECORD_VERSION;
    record->size = (uint16_t)sizeof(*record);
}

void crash_record_finalize(
    crash_record_t *record
)
{
    if (record == NULL)
    {
        return;
    }

    record->checksum = 0U;
    record->checksum =
        checksum_bytes(
            (const uint8_t *)record,
            sizeof(*record)
        );
}

bool crash_record_is_valid(
    const crash_record_t *record
)
{
    if ((record == NULL) ||
        (record->magic != CRASH_RECORD_MAGIC) ||
        (record->version != CRASH_RECORD_VERSION) ||
        (record->size != sizeof(*record)))
    {
        return false;
    }

    crash_record_t copy = *record;
    const uint32_t expected = copy.checksum;
    copy.checksum = 0U;

    return checksum_bytes(
        (const uint8_t *)&copy,
        sizeof(copy)
    ) == expected;
}
