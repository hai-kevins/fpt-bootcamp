#include "test.h"
#include "heap.h"
#include <stdint.h>

int test_coalesce_all_blocks(void)
{
    uint8_t memory[512];
    void *a;
    void *b;
    void *c;
    TEST_ASSERT(heap_init(memory, sizeof(memory)));
    a = heap_alloc(32U);
    b = heap_alloc(48U);
    c = heap_alloc(24U);
    TEST_ASSERT(a && b && c);
    TEST_ASSERT(heap_free(b));
    TEST_ASSERT(heap_free(a));
    TEST_ASSERT(heap_free(c));
    TEST_ASSERT(heap_block_count() == 1U);
    TEST_ASSERT(heap_validate());
    return 0;
}

int test_allocate_after_coalesce(void)
{
    uint8_t memory[512];
    void *a;
    void *b;
    void *large;
    TEST_ASSERT(heap_init(memory, sizeof(memory)));
    a = heap_alloc(120U);
    b = heap_alloc(120U);
    TEST_ASSERT(a && b);
    TEST_ASSERT(heap_free(a));
    TEST_ASSERT(heap_free(b));
    large = heap_alloc(300U);
    TEST_ASSERT(large != (void *)0);
    TEST_ASSERT(heap_validate());
    return 0;
}
