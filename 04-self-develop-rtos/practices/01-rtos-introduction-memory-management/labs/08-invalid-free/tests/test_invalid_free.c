#include "test.h"
#include "heap.h"
#include <stdint.h>

int test_invalid_free_foreign_pointer(void)
{
    uint8_t memory[512];
    uint8_t foreign;
    TEST_ASSERT(heap_init(memory, sizeof(memory)) == HEAP_STATUS_OK);
    TEST_ASSERT(heap_free(&foreign) == HEAP_STATUS_POINTER_OUT_OF_RANGE);
    TEST_ASSERT(heap_validate());
    return 0;
}

int test_invalid_free_middle_pointer(void)
{
    uint8_t memory[512];
    uint8_t *pointer;
    TEST_ASSERT(heap_init(memory, sizeof(memory)) == HEAP_STATUS_OK);
    pointer = (uint8_t *) heap_alloc(64U);
    TEST_ASSERT(pointer != (uint8_t *)0);
    TEST_ASSERT(heap_free(pointer + 4U) == HEAP_STATUS_INVALID_POINTER);
    TEST_ASSERT(heap_free(pointer) == HEAP_STATUS_OK);
    return 0;
}
