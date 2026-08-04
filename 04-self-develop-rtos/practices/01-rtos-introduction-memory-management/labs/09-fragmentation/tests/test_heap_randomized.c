#include "test.h"
#include "heap.h"
#include <stdint.h>

#define SLOT_COUNT (32U)
#define STEP_COUNT (3000U)

static uint32_t next_random(uint32_t *state)
{
    *state = (*state * UINT32_C(1664525)) + UINT32_C(1013904223);
    return *state;
}

int test_randomized_allocator_sequence(void)
{
    uint8_t memory[4096];
    void *slots[SLOT_COUNT] =
    {
        0
    };
    uint32_t state = UINT32_C(0x12345678);
    size_t step;
    TEST_ASSERT(heap_init(memory, sizeof(memory)) == HEAP_STATUS_OK);
    for (step = 0U; step < STEP_COUNT; step++)
    {
        const size_t slot = next_random(&state) % SLOT_COUNT;
        if (slots[slot] == (void *)0)
        {
            const size_t size = 1U + (next_random(&state) % 160U);
            slots[slot] = heap_alloc(size);
        }
        else
        {
            TEST_ASSERT(heap_free(slots[slot]) == HEAP_STATUS_OK);
            slots[slot] = (void *)0;
        }
        TEST_ASSERT(heap_validate());
    }
    for (step = 0U; step < SLOT_COUNT; step++)
    {
        if (slots[step] != (void *)0)
        {
            TEST_ASSERT(heap_free(slots[step]) == HEAP_STATUS_OK);
        }
    }
    TEST_ASSERT(heap_validate());
    return 0;
}
