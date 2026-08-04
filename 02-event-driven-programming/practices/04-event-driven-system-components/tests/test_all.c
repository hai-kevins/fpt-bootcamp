#include "test_support.h"

#include "active_object.h"
#include "crc16.h"
#include "datalink.h"
#include "event_pool.h"
#include "event_trace.h"
#include "fsm.h"
#include "hsm.h"
#include "parser.h"
#include "pubsub.h"
#include "router.h"
#include "scheduler.h"
#include "serializer.h"

#include <string.h>

static uint32_t g_handler_count;
static uint8_t g_order[8];
static size_t g_order_count;

static void count_handler(ed_active_object_t *object, const ed_event_t *event)
{
    (void) event;
    g_handler_count++;
    if (g_order_count < sizeof(g_order))
    {
        g_order[g_order_count] = object->id;
        g_order_count++;
    }
}

static bool test_mailbox(void)
{
    ed_mailbox_t mailbox;
    ed_event_t events[3];
    TEST_ASSERT(ed_mailbox_init(&mailbox, 2U, ED_MAILBOX_DROP_NEWEST));
    ed_event_init_static(&events[0], 1U, 0U, 0U);
    ed_event_init_static(&events[1], 2U, 0U, 0U);
    ed_event_init_static(&events[2], 3U, 0U, 0U);
    TEST_ASSERT(ed_mailbox_post(&mailbox, &events[0]));
    TEST_ASSERT(ed_mailbox_post(&mailbox, &events[1]));
    TEST_ASSERT(!ed_mailbox_post(&mailbox, &events[2]));
    TEST_ASSERT(mailbox.high_water_mark == 2U);
    TEST_ASSERT(ed_mailbox_get(&mailbox)->signal == 1U);
    TEST_ASSERT(ed_mailbox_get(&mailbox)->signal == 2U);
    TEST_ASSERT(ed_mailbox_get(&mailbox) == NULL);
    return true;
}

static bool test_event_pool(void)
{
    ed_event_pool_t pool;
    ed_event_pool_init(&pool);
    ed_event_t *event = ed_event_pool_allocate(&pool, 7U, 1U, 2U);
    TEST_ASSERT(event != NULL);
    TEST_ASSERT(pool.used_count == 1U);
    TEST_ASSERT(ed_event_pool_retain(&pool, event));
    TEST_ASSERT(event->ref_count == 2U);
    TEST_ASSERT(ed_event_pool_release(&pool, event));
    TEST_ASSERT(pool.used_count == 1U);
    TEST_ASSERT(ed_event_pool_release(&pool, event));
    TEST_ASSERT(pool.used_count == 0U);
    TEST_ASSERT(!ed_event_pool_release(&pool, event));
    return true;
}

static bool test_scheduler_priority(void)
{
    ed_event_pool_t pool;
    ed_scheduler_t scheduler;
    ed_active_object_t low;
    ed_active_object_t high;
    ed_event_t low_event;
    ed_event_t high_event;
    ed_event_pool_init(&pool);
    ed_scheduler_init(&scheduler, &pool);
    g_order_count = 0U;
    TEST_ASSERT(ed_active_object_init(&low, 1U, 1U, "low", 4U, count_handler, NULL));
    TEST_ASSERT(ed_active_object_init(&high, 2U, 3U, "high", 4U, count_handler, NULL));
    TEST_ASSERT(ed_scheduler_register(&scheduler, &low));
    TEST_ASSERT(ed_scheduler_register(&scheduler, &high));
    ed_event_init_static(&low_event, 1U, 0U, 1U);
    ed_event_init_static(&high_event, 1U, 0U, 2U);
    TEST_ASSERT(ed_active_object_post(&low, &low_event));
    TEST_ASSERT(ed_active_object_post(&high, &high_event));
    TEST_ASSERT(ed_scheduler_run_until_idle(&scheduler, 4U) == 2U);
    TEST_ASSERT(g_order[0] == 2U);
    TEST_ASSERT(g_order[1] == 1U);
    return true;
}

static void increment_action(void *context, const ed_event_t *event)
{
    (void) event;
    uint32_t *counter = context;
    (*counter)++;
}

static bool test_fsm(void)
{
    enum
    {
        IDLE = 0,
        ACTIVE = 1
    };
    const ed_fsm_transition_t transitions[] =
    {
        {
            IDLE, 1U, ACTIVE, increment_action
        },
        {
            ACTIVE,
            2U,
            IDLE,
            increment_action
        }
    };
    uint32_t actions = 0U;
    ed_fsm_t fsm;
    ed_event_t event;
    ed_fsm_init(&fsm, IDLE, transitions, sizeof(transitions) / sizeof(transitions[0]), &actions);
    ed_event_init_static(&event, 1U, 0U, 0U);
    TEST_ASSERT(ed_fsm_dispatch(&fsm, &event));
    TEST_ASSERT(fsm.state == ACTIVE);
    event.signal = 2U;
    TEST_ASSERT(ed_fsm_dispatch(&fsm, &event));
    TEST_ASSERT(fsm.state == IDLE);
    TEST_ASSERT(actions == 2U);
    return true;
}

typedef struct
{
    uint32_t parent_hits;
} hsm_context_t;

static bool hsm_handler(void *context, uint8_t state, const ed_event_t *event, uint8_t *next_state)
{
    hsm_context_t *ctx = context;
    if ((state == 0U) && (event->signal == 9U))
    {
        ctx->parent_hits++;
        *next_state = 1U;
        return true;
    }
    return false;
}

static bool test_hsm(void)
{
    const uint8_t parents[] =
    {
        ED_HSM_NO_STATE,
        0U,
        0U
    };
    hsm_context_t context =
    {
        0U
    };
    ed_hsm_t hsm;
    ed_event_t event;
    TEST_ASSERT(ed_hsm_init(&hsm, 2U, parents, 3U, hsm_handler, &context));
    ed_event_init_static(&event, 9U, 0U, 0U);
    TEST_ASSERT(ed_hsm_dispatch(&hsm, &event));
    TEST_ASSERT(context.parent_hits == 1U);
    TEST_ASSERT(hsm.current_state == 1U);
    TEST_ASSERT(hsm.bubbled_count >= 1U);
    return true;
}

static bool test_pubsub(void)
{
    ed_event_pool_t pool;
    ed_pubsub_t pubsub;
    ed_scheduler_t scheduler;
    ed_active_object_t a;
    ed_active_object_t b;
    ed_event_t prototype;
    ed_event_pool_init(&pool);
    ed_pubsub_init(&pubsub, &pool);
    ed_scheduler_init(&scheduler, &pool);
    g_handler_count = 0U;
    TEST_ASSERT(ed_active_object_init(&a, 1U, 1U, "a", 4U, count_handler, NULL));
    TEST_ASSERT(ed_active_object_init(&b, 2U, 1U, "b", 4U, count_handler, NULL));
    TEST_ASSERT(ed_scheduler_register(&scheduler, &a));
    TEST_ASSERT(ed_scheduler_register(&scheduler, &b));
    TEST_ASSERT(ed_pubsub_subscribe(&pubsub, 5U, &a));
    TEST_ASSERT(ed_pubsub_subscribe(&pubsub, 5U, &b));
    ed_event_init_static(&prototype, 5U, 0U, 0U);
    TEST_ASSERT(ed_pubsub_publish(&pubsub, &prototype) == 2U);
    TEST_ASSERT(ed_scheduler_run_until_idle(&scheduler, 4U) == 2U);
    TEST_ASSERT(g_handler_count == 2U);
    TEST_ASSERT(pool.used_count == 0U);
    return true;
}

static bool test_serializer_parser(void)
{
    ed_frame_t frame =
    {
        0
    };
    ed_frame_t decoded;
    ed_frame_t parsed;
    ed_parser_t parser;
    uint8_t bytes[ED_FRAME_MAX_SIZE];
    size_t length = 0U;
    frame.source_node = 1U;
    frame.destination_node = 2U;
    frame.source_object = 3U;
    frame.destination_object = 4U;
    frame.signal = 0x1234U;
    frame.sequence = 77U;
    frame.payload_length = 3U;
    frame.payload[0] = 1U;
    frame.payload[1] = 2U;
    frame.payload[2] = 3U;
    TEST_ASSERT(ed_frame_encode(&frame, bytes, sizeof(bytes), &length));
    TEST_ASSERT(ed_frame_decode(bytes, length, &decoded));
    TEST_ASSERT(decoded.signal == frame.signal);
    TEST_ASSERT(decoded.sequence == frame.sequence);
    ed_parser_init(&parser);
    bool ready = false;
    for (size_t i = 0U; i < length; i++)
    {
        ready = ed_parser_feed(&parser, bytes[i], &parsed) || ready;
    }
    TEST_ASSERT(ready);
    TEST_ASSERT(parsed.destination_object == 4U);
    bytes[11] ^= 0x01U;
    TEST_ASSERT(!ed_frame_decode(bytes, length, &decoded));
    return true;
}

typedef struct
{
    uint32_t sends;
} send_spy_t;

static bool send_spy(void *context, const uint8_t *data, size_t length)
{
    send_spy_t *spy = context;
    if ((data == NULL) || (length == 0U))
    {
        return false;
    }
    spy->sends++;
    return true;
}

static bool test_datalink_retry(void)
{
    send_spy_t spy =
    {
        0U
    };
    ed_datalink_t link;
    ed_frame_t frame =
    {
        0
    };
    frame.sequence = 10U;
    TEST_ASSERT(ed_datalink_init(&link, send_spy, &spy, 5U, 2U));
    TEST_ASSERT(ed_datalink_send(&link, &frame, true));
    TEST_ASSERT(link.state == ED_LINK_WAIT_ACK);
    ed_datalink_tick(&link, 5U);
    ed_datalink_tick(&link, 5U);
    ed_datalink_tick(&link, 5U);
    TEST_ASSERT(link.state == ED_LINK_DOWN);
    TEST_ASSERT(spy.sends == 3U);
    TEST_ASSERT(link.failure_count == 1U);
    return true;
}

static bool test_event_trace(void)
{
    ed_trace_t trace;
    ed_trace_record_t record;
    ed_trace_record_t out;
    ed_trace_init(&trace);
    for (size_t i = 0U; i < ED_TRACE_CAPACITY + 3U; i++)
    {
        (void) memset(&record, 0, sizeof(record));
        record.timestamp = (uint32_t) i;
        record.signal = (ed_signal_t) i;
        ed_trace_write(&trace, &record);
    }
    TEST_ASSERT(trace.count == ED_TRACE_CAPACITY);
    TEST_ASSERT(trace.overwritten_count == 3U);
    TEST_ASSERT(ed_trace_get_oldest(&trace, 0U, &out));
    TEST_ASSERT(out.timestamp == 3U);
    return true;
}

int main(void)
{
    const test_case_t tests[] =
    {
        {
            "mailbox", test_mailbox
        },
        {
            "event_pool",
            test_event_pool
        },
        {
            "scheduler_priority",
            test_scheduler_priority
        },
        {
            "fsm",
            test_fsm
        },
        {
            "hsm",
            test_hsm
        },
        {
            "pubsub",
            test_pubsub
        },
        {
            "serializer_parser",
            test_serializer_parser
        },
        {
            "datalink_retry",
            test_datalink_retry
        },
        {
            "event_trace",
            test_event_trace
        }
    };
    size_t passed = 0U;

    (void) printf("04-event-driven-system-components tests\n");
    for (size_t i = 0U; i < sizeof(tests) / sizeof(tests[0]); i++)
    {
        const bool ok = tests[i].function();
        (void) printf("[%s] %s\n", ok ? "PASS" : "FAIL", tests[i].name);
        if (ok)
        {
            passed++;
        }
    }

    (void) printf("Summary: %zu/%zu PASS\n", passed, sizeof(tests) / sizeof(tests[0]));
    return (passed == (sizeof(tests) / sizeof(tests[0]))) ? 0 : 1;
}
