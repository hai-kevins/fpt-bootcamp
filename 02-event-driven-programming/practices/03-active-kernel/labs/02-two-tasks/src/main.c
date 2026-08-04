#include <assert.h>
#include <stdint.h>
#include <stdio.h>

typedef struct
{
    uint8_t destination;
    uint16_t signal;
} message_t;
static message_t queue[4];
static uint8_t head, tail, count;
static void post(uint8_t dst, uint16_t sig)
{
    assert(count < 4U);
    queue[tail] = (message_t)
    {
        dst,
        sig
    };
    tail = (uint8_t)((tail + 1U) % 4U);
    count++;
}

static void task_a(uint16_t sig)
{
    if (sig == 1U)
    {
        post(2U, 2U);
    }
}

static void task_b(uint16_t sig)
{
    if (sig == 2U)
    {
        puts("Task B received HELLO_SIG");
    }
}

int main(void)
{
    post(1U, 1U);
    while (count)
    {
        message_t m = queue[head];
        head = (uint8_t)((head + 1U) % 4U);
        count--;
        if (m.destination == 1U)
        {
            task_a(m.signal);
        }
            else
            {
                task_b(m.signal);
            }
    }
    assert(count == 0U);
    return 0;
}
