#include "event_trace.h"
#include <stdio.h>
int main(void)
{
    trace_record_t record;
    trace_init();
    for (uint32_t i=0UL; i<12UL; i++)
    {
        const trace_record_t input =
        {
            i*10UL, 1U, 2U, (uint16_t)(100U+i), i
        };
        trace_write(&input);
    }
    printf("timestamp,source,destination,signal,parameter\n");
    for (size_t i=0U; i<trace_count(); i++)
    {
        (void)trace_get(i,&record);
        printf("%lu,%u,%u,%u,%lu\n",
               (unsigned long)record.timestamp,
               (unsigned int)record.source,
               (unsigned int)record.destination,
               (unsigned int)record.signal,
               (unsigned long)record.parameter);
    }
    printf("overwritten=%lu\n",
           (unsigned long)trace_overwritten());
    return (trace_overwritten()==4UL) ? 0 : 1;
}
