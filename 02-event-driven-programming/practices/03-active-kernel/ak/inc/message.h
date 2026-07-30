#ifndef AK_MESSAGE_H
#define AK_MESSAGE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "ak_config.h"

typedef enum
{
    AK_MESSAGE_PURE = 0,
    AK_MESSAGE_COMMON,
    AK_MESSAGE_DYNAMIC
} ak_message_kind_t;

typedef struct ak_message
{
    uint16_t signal;
    uint8_t source;
    uint8_t destination;
    uint8_t kind;
    uint8_t length;
    uint8_t reference_count;
    uint8_t pool_index;
    uint8_t payload[AK_DYNAMIC_PAYLOAD_CAPACITY];
} ak_message_t;

typedef struct
{
    uint16_t capacity;
    uint16_t used;
    uint16_t used_max;
    uint32_t allocations;
    uint32_t allocation_failures;
    uint32_t releases;
} ak_message_pool_stats_t;

void ak_message_system_init(void);

ak_message_t *ak_message_create_pure(
    uint8_t source,
    uint8_t destination,
    uint16_t signal
);

ak_message_t *ak_message_create_common(
    uint8_t source,
    uint8_t destination,
    uint16_t signal,
    const void *data,
    size_t length
);

ak_message_t *ak_message_create_dynamic(
    uint8_t source,
    uint8_t destination,
    uint16_t signal,
    const void *data,
    size_t length
);

bool ak_message_retain(ak_message_t *message);
void ak_message_release(ak_message_t *message);
const void *ak_message_data(const ak_message_t *message);
size_t ak_message_length(const ak_message_t *message);
ak_message_pool_stats_t ak_message_pool_stats(void);

#endif
