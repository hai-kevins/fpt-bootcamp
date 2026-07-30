#ifndef AK_EVENT_RECORD_H
#define AK_EVENT_RECORD_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "ak_config.h"

typedef enum
{
    AK_RECORD_MESSAGE_ALLOC = 0,
    AK_RECORD_MESSAGE_POST,
    AK_RECORD_DISPATCH_START,
    AK_RECORD_DISPATCH_END,
    AK_RECORD_MESSAGE_DROP,
    AK_RECORD_TIMER_START,
    AK_RECORD_TIMER_EXPIRE,
    AK_RECORD_STATE_TRANSITION,
    AK_RECORD_FATAL
} ak_record_type_t;

typedef struct
{
    uint32_t timestamp;
    uint16_t signal;
    uint16_t value;
    uint8_t type;
    uint8_t source;
    uint8_t destination;
    uint8_t state;
} ak_event_record_t;

void ak_event_record_init(void);
void ak_event_record_write(const ak_event_record_t *record);
size_t ak_event_record_count(void);
bool ak_event_record_read(size_t oldest_index, ak_event_record_t *record);
void ak_event_record_clear(void);
uint32_t ak_event_record_overwritten(void);

#endif
