#include "fatal.h"

#include "ak_port.h"
#include "event_record.h"

#define AK_FATAL_MAGIC (0x414B4654UL)

__attribute__((section(".noinit"))) static ak_fatal_record_t g_fatal_record;

static uint32_t fatal_checksum(const ak_fatal_record_t *record)
{
    return record->magic ^ record->error_code ^ record->timestamp ^ record->restart_count ^ (uint32_t) record->signal ^ (uint32_t) record->task_id ^ 0x5A5AA5A5UL;
}

bool ak_fatal_record_valid(void)
{
    return (g_fatal_record.magic == AK_FATAL_MAGIC) && (g_fatal_record.checksum == fatal_checksum(&g_fatal_record));
}

void ak_fatal_boot(void)
{
    if (ak_fatal_record_valid())
    {
        g_fatal_record.restart_count++;
        g_fatal_record.checksum = fatal_checksum(&g_fatal_record);
    }
}

void ak_fatal_clear(void)
{
    g_fatal_record.magic = 0U;
    g_fatal_record.checksum = 0U;
}

ak_fatal_record_t ak_fatal_record_get(void)
{
    return g_fatal_record;
}

void ak_fatal_capture(uint32_t error_code, uint8_t task_id, uint16_t signal)
{
    g_fatal_record.magic = AK_FATAL_MAGIC;
    g_fatal_record.error_code = error_code;
    g_fatal_record.timestamp = ak_port_time_now_ms();
    g_fatal_record.task_id = task_id;
    g_fatal_record.signal = signal;
    if (!ak_fatal_record_valid())
    {
        g_fatal_record.restart_count = 0U;
    }
    g_fatal_record.checksum = fatal_checksum(&g_fatal_record);

    ak_event_record_t record =
    {
        .timestamp = g_fatal_record.timestamp,
        .signal = signal,
        .value = (uint16_t)(error_code & 0xFFFFU),
        .type = AK_RECORD_FATAL,
        .source = task_id,
        .destination = 0U,
        .state = 0U
    };
    ak_event_record_write(&record);
}

void ak_fatal_raise(uint32_t error_code, uint8_t task_id, uint16_t signal)
{
    ak_fatal_capture(error_code, task_id, signal);
    ak_port_reset();
}
