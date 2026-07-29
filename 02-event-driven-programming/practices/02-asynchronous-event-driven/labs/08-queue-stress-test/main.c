#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#define CAPACITY (64U)
#define ATTEMPTS (100000UL)
typedef struct
{
    uint32_t buffer[CAPACITY];
    uint16_t head, tail, count, high;
    uint32_t posted, dispatched, dropped;
} queue_t;
static bool post(queue_t *q, uint32_t value)
{
    if (q->count>=CAPACITY)
    {
        q->dropped++;
        return false;
    }
    q->buffer[q->head]=value;
    q->head=(uint16_t)((q->head+1U)%CAPACITY);
    q->count++;
    q->posted++;
    if (q->count>q->high) q->high=q->count;
    return true;
}
static bool get(queue_t *q, uint32_t *value)
{
    if (q->count==0U) return false;
    *value=q->buffer[q->tail];
    q->tail=(uint16_t)((q->tail+1U)%CAPACITY);
    q->count--;
    q->dispatched++;
    return true;
}
int main(void)
{
    queue_t q={0};
    uint32_t output=0UL;
    for (uint32_t i=0UL; i<ATTEMPTS; i++)
    {
        (void)post(&q,i);
        if ((i%4UL)==0UL)
            (void)get(&q,&output);
    }
    while (get(&q,&output)) {}
    printf("attempts=%lu posted=%lu dispatched=%lu dropped=%lu high=%u\n",
           (unsigned long)ATTEMPTS,
           (unsigned long)q.posted,
           (unsigned long)q.dispatched,
           (unsigned long)q.dropped,
           (unsigned int)q.high);
    return ((q.posted+q.dropped==ATTEMPTS) &&
            (q.posted==q.dispatched) &&
            (q.high==CAPACITY)) ? 0 : 1;
}
