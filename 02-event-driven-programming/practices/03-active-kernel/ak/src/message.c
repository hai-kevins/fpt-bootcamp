#include "message.h"

#include "ak_port.h"
#include "event_record.h"

#include "runtime.h"

static ak_message_t g_pool[AK_MESSAGE_POOL_CAPACITY];
static bool g_used[AK_MESSAGE_POOL_CAPACITY];
static ak_message_pool_stats_t g_stats;

static ak_message_t *message_allocate(
    ak_message_kind_t kind,
    uint8_t source,
    uint8_t destination,
    uint16_t signal,
    const void *data,
    size_t length,
    size_t maximum_length
)
{
    ak_message_t *message = 0;
    uint32_t key;

    if ((length > maximum_length) || ((length > 0U) && (data == 0)))
    {
        return 0;
    }

    key = ak_port_critical_enter();

    for (uint8_t i = 0U; i < AK_MESSAGE_POOL_CAPACITY; i++)
    {
        if (!g_used[i])
        {
            g_used[i] = true;
            message = &g_pool[i];
            message->pool_index = i;
            g_stats.used++;
            g_stats.allocations++;
            if (g_stats.used > g_stats.used_max)
            {
                g_stats.used_max = g_stats.used;
            }
            break;
        }
    }

    if (message == 0)
    {
        g_stats.allocation_failures++;
    }

    ak_port_critical_exit(key);

    if (message != 0)
    {
        message->signal = signal;
        message->source = source;
        message->destination = destination;
        message->kind = (uint8_t)kind;
        message->length = (uint8_t)length;
        message->reference_count = 1U;

        if (length > 0U)
        {
            (void)memcpy(message->payload, data, length);
        }

        ak_event_record_t record = {
            .timestamp = ak_port_time_now_ms(),
            .signal = signal,
            .value = (uint16_t)length,
            .type = AK_RECORD_MESSAGE_ALLOC,
            .source = source,
            .destination = destination,
            .state = (uint8_t)kind
        };
        ak_event_record_write(&record);
    }

    return message;
}

void ak_message_system_init(void)
{
    (void)memset(g_pool, 0, sizeof(g_pool));
    (void)memset(g_used, 0, sizeof(g_used));
    (void)memset(&g_stats, 0, sizeof(g_stats));
    g_stats.capacity = AK_MESSAGE_POOL_CAPACITY;
}

ak_message_t *ak_message_create_pure(
    uint8_t source,
    uint8_t destination,
    uint16_t signal
)
{
    return message_allocate(
        AK_MESSAGE_PURE, source, destination, signal,
        0, 0U, 0U
    );
}

ak_message_t *ak_message_create_common(
    uint8_t source,
    uint8_t destination,
    uint16_t signal,
    const void *data,
    size_t length
)
{
    return message_allocate(
        AK_MESSAGE_COMMON, source, destination, signal,
        data, length, AK_COMMON_PAYLOAD_CAPACITY
    );
}

ak_message_t *ak_message_create_dynamic(
    uint8_t source,
    uint8_t destination,
    uint16_t signal,
    const void *data,
    size_t length
)
{
    return message_allocate(
        AK_MESSAGE_DYNAMIC, source, destination, signal,
        data, length, AK_DYNAMIC_PAYLOAD_CAPACITY
    );
}

bool ak_message_retain(ak_message_t *message)
{
    uint32_t key;
    bool result = false;

    if (message == 0)
    {
        return false;
    }

    key = ak_port_critical_enter();
    if ((message->reference_count > 0U) &&
        (message->reference_count < UINT8_MAX))
    {
        message->reference_count++;
        result = true;
    }
    ak_port_critical_exit(key);
    return result;
}

void ak_message_release(ak_message_t *message)
{
    uint32_t key;

    if (message == 0)
    {
        return;
    }

    key = ak_port_critical_enter();

    if (message->reference_count > 0U)
    {
        message->reference_count--;
        if (message->reference_count == 0U)
        {
            const uint8_t index = message->pool_index;
            if ((index < AK_MESSAGE_POOL_CAPACITY) && g_used[index])
            {
                g_used[index] = false;
                if (g_stats.used > 0U)
                {
                    g_stats.used--;
                }
                g_stats.releases++;
            }
        }
    }

    ak_port_critical_exit(key);
}

const void *ak_message_data(const ak_message_t *message)
{
    return (message == 0) ? 0 : message->payload;
}

size_t ak_message_length(const ak_message_t *message)
{
    return (message == 0) ? 0U : (size_t)message->length;
}

ak_message_pool_stats_t ak_message_pool_stats(void)
{
    return g_stats;
}
