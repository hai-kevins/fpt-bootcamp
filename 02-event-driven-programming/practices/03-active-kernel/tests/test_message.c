#include "test.h"
#include "message.h"

#include <string.h>

bool test_message_pool(void)
{
    ak_message_system_init();
    const uint32_t payload = 0x12345678UL;
    ak_message_t *message = ak_message_create_common(1U, 2U, 10U, &payload, sizeof(payload));
    TEST_ASSERT_TRUE(message != 0);
    TEST_ASSERT_EQ(AK_MESSAGE_COMMON, message->kind);
    TEST_ASSERT_EQ(sizeof(payload), ak_message_length(message));
    TEST_ASSERT_TRUE(memcmp(message->payload, &payload, sizeof(payload)) == 0);
    TEST_ASSERT_TRUE(ak_message_retain(message));
    ak_message_release(message);
    TEST_ASSERT_EQ(1U, ak_message_pool_stats().used);
    ak_message_release(message);
    TEST_ASSERT_EQ(0U, ak_message_pool_stats().used);
    return true;
}
