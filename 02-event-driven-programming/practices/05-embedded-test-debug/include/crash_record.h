#ifndef CRASH_RECORD_H
#define CRASH_RECORD_H

#include <stdbool.h>
#include <stdint.h>

#define CRASH_RECORD_MAGIC (0x43525348UL)
#define CRASH_RECORD_VERSION (1U)

typedef struct
{
    uint32_t magic;
    uint16_t version;
    uint16_t size;

    uint32_t build_id;
    uint32_t reset_reason;
    uint32_t fatal_code;
    uint32_t uptime_ms;

    uint16_t last_signal;
    uint8_t current_state;
    uint8_t current_component;

    uint32_t queue_overflow_count;
    uint32_t pool_failure_count;
    uint32_t checksum;
} crash_record_t;

void crash_record_prepare(crash_record_t *record);

void crash_record_finalize(crash_record_t *record);

bool crash_record_is_valid(const crash_record_t *record);

#endif
