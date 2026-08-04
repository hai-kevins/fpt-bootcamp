#include "event_record.h"

#include "ak_port.h"

#include "runtime.h"

static ak_event_record_t g_records[AK_EVENT_RECORD_CAPACITY];
static size_t g_head;
static size_t g_count;
static uint32_t g_overwritten;

void ak_event_record_init(void)
{
    uint32_t key = ak_port_critical_enter();
    (void) memset(g_records, 0, sizeof(g_records));
    g_head = 0U;
    g_count = 0U;
    g_overwritten = 0U;
    ak_port_critical_exit(key);
}

void ak_event_record_write(const ak_event_record_t *record)
{
    uint32_t key;
    if (record == 0)
    {
        return;
    }

    key = ak_port_critical_enter();
    g_records[g_head] = *record;
    g_head = (g_head + 1U) % AK_EVENT_RECORD_CAPACITY;
    if (g_count < AK_EVENT_RECORD_CAPACITY)
    {
        g_count++;
    }
    else
    {
        g_overwritten++;
    }
    ak_port_critical_exit(key);
}

size_t ak_event_record_count(void)
{
    return g_count;
}

bool ak_event_record_read(size_t oldest_index, ak_event_record_t *record)
{
    size_t oldest;
    size_t position;
    if ((record == 0) || (oldest_index >= g_count))
    {
        return false;
    }

    oldest = (g_head + AK_EVENT_RECORD_CAPACITY - g_count) % AK_EVENT_RECORD_CAPACITY;
    position = (oldest + oldest_index) % AK_EVENT_RECORD_CAPACITY;
    *record = g_records[position];
    return true;
}

void ak_event_record_clear(void)
{
    uint32_t key = ak_port_critical_enter();
    g_head = 0U;
    g_count = 0U;
    g_overwritten = 0U;
    ak_port_critical_exit(key);
}

uint32_t ak_event_record_overwritten(void)
{
    return g_overwritten;
}
