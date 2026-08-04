#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct
{
    uint32_t magic;
    uint32_t fatal;
    uint16_t task;
    uint16_t signal;
    uint8_t state;
    uint8_t reset_reason;
    uint32_t uptime;
    uint32_t checksum;
} record_t;

static uint32_t checksum(record_t record)
{
    record.checksum = 0U;
    const uint8_t *bytes = (const uint8_t *)&record;
    uint32_t value = 2166136261UL;

    for (size_t i = 0U; i < sizeof(record); i++)
    {
        value ^= bytes[i];
        value *= 16777619UL;
    }

    return value;
}

int main(void)
{
    record_t record =
    {
        .magic = 0x43525348UL,
        .fatal = 0xE001U,
        .task = 3U,
        .signal = 42U,
        .state = 2U,
        .reset_reason = 1U,
        .uptime = 12345U,
        .checksum = 0U
    };

    record.checksum = checksum(record);
    const bool valid = (record.magic == 0x43525348UL) && (checksum(record) == record.checksum);

    (void) printf("valid=%u fatal=0x%04lX task=%u signal=%u ""state=%u uptime=%lu reset=%u\n", valid ? 1U : 0U,
        (unsigned long) record.fatal, record.task, record.signal, record.state, (unsigned long) record.uptime,
        record.reset_reason);

    return valid ? 0 : 1;
}
