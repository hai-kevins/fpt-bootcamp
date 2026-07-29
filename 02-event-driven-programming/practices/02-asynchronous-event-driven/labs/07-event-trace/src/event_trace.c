#include "event_trace.h"
#include <stddef.h>
static trace_record_t g_records[TRACE_CAPACITY];
static size_t g_head;
static size_t g_count;
static uint32_t g_overwritten;
void trace_init(void)
{
    g_head=0U; g_count=0U; g_overwritten=0UL;
}
void trace_write(const trace_record_t *record)
{
    if (record == NULL) return;
    g_records[g_head]=*record;
    g_head=(g_head+1U)%TRACE_CAPACITY;
    if (g_count<TRACE_CAPACITY) g_count++;
    else g_overwritten++;
}
size_t trace_count(void) { return g_count; }
bool trace_get(size_t index, trace_record_t *record)
{
    size_t oldest;
    if ((record==NULL)||(index>=g_count)) return false;
    oldest=(g_head+TRACE_CAPACITY-g_count)%TRACE_CAPACITY;
    *record=g_records[(oldest+index)%TRACE_CAPACITY];
    return true;
}
uint32_t trace_overwritten(void) { return g_overwritten; }
