#include "test.h"
#include "event_record.h"
#include "message.h"
#include "task.h"

static uint16_t g_last_signal;
static void handler(const ak_message_t *message)
{
    g_last_signal = message->signal;
}

bool test_task_scheduler(void)
{
    ak_event_record_init();
    ak_message_system_init();
    ak_task_system_init();
    g_last_signal = 0U;
    TEST_ASSERT_TRUE(ak_task_register(1U, 1U, "low", handler));
    TEST_ASSERT_TRUE(ak_task_register(2U, 5U, "high", handler));
    ak_message_t *low = ak_message_create_pure(0U, 1U, 11U);
    ak_message_t *high = ak_message_create_pure(0U, 2U, 22U);
    TEST_ASSERT_TRUE(ak_task_post(low));
    TEST_ASSERT_TRUE(ak_task_post(high));
    TEST_ASSERT_TRUE(ak_task_run_once());
    TEST_ASSERT_EQ(22U, g_last_signal);
    TEST_ASSERT_TRUE(ak_task_run_once());
    TEST_ASSERT_EQ(11U, g_last_signal);
    TEST_ASSERT_EQ(0U, ak_message_pool_stats().used);
    return true;
}
