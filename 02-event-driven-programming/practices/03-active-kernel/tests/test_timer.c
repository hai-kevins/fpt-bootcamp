#include "test.h"
#include "event_record.h"
#include "message.h"
#include "task.h"
#include "timer.h"

extern uint32_t g_fake_time;
static uint32_t g_count;
static void handler(const ak_message_t *message) { (void)message; g_count++; }

bool test_timer(void)
{
    g_fake_time = 0U;
    g_count = 0U;
    ak_event_record_init();
    ak_message_system_init();
    ak_task_system_init();
    ak_timer_system_init();
    TEST_ASSERT_TRUE(ak_task_register(1U, 1U, "task", handler));
    TEST_ASSERT_TRUE(ak_timer_start_one_shot(0U, 1U, 7U, 20U) != AK_TIMER_ID_INVALID);
    ak_timer_process(19U);
    TEST_ASSERT_EQ(0U, g_count);
    ak_timer_process(20U);
    (void)ak_task_run_until_idle(8U);
    TEST_ASSERT_EQ(1U, g_count);
    return true;
}
