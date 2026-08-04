#include <stdint.h>
#include <stdio.h>

#define TASK_COUNT            (4UL)
#define TASK_STACK_BYTES      (512UL)
#define TCB_BYTES             (96UL)
#define QUEUE_STORAGE_BYTES   (512UL)
#define MUTEX_BYTES           (3UL * 32UL)
#define TIMER_BYTES           (4UL * 40UL)
#define EVENT_POOL_BYTES      (512UL)
#define MAIN_STACK_BYTES      (1024UL)
#define SRAM_BYTES            (20UL * 1024UL)

int main(void)
{
    const uint32_t task_stacks = TASK_COUNT * TASK_STACK_BYTES;
    const uint32_t tcbs = TASK_COUNT * TCB_BYTES;
    const uint32_t total = task_stacks + tcbs + QUEUE_STORAGE_BYTES + MUTEX_BYTES + TIMER_BYTES + EVENT_POOL_BYTES + MAIN_STACK_BYTES;

    printf("Lab 04 - Static memory budget\n");
    printf("Task stacks : %lu bytes\n", (unsigned long) task_stacks);
    printf("TCBs        : %lu bytes\n", (unsigned long) tcbs);
    printf("Total       : %lu bytes\n", (unsigned long) total);
    printf("Remaining   : %lu bytes\n", (unsigned long)(SRAM_BYTES - total));
    return total < SRAM_BYTES ? 0 : 1;
}
