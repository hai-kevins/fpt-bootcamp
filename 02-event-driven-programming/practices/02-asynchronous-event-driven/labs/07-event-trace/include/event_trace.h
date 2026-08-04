#ifndef LAB07_TRACE_H
#define LAB07_TRACE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define TRACE_CAPACITY (8U)

typedef struct
{
    uint32_t timestamp;
    uint8_t source;
    uint8_t destination;
    uint16_t signal;
    uint32_t parameter;
} trace_record_t;

void trace_init(void);
void trace_write(const trace_record_t *record);
size_t trace_count(void);
bool trace_get(size_t index, trace_record_t *record);
uint32_t trace_overwritten(void);

#endif
