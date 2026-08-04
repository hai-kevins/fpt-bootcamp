#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define BLOCKS 4U
#define SIZE 96U
static uint8_t pool[BLOCKS][SIZE];
static bool used[BLOCKS];
static uint8_t current, max_used;
static void *alloc(void)
{
    for (uint8_t i = 0; i < BLOCKS; i++)
    {
        if (!used[i])
        {
            used[i] = true;
            current++;
            if (current > max_used)
            {
                max_used = current;
            }
            return pool[i];
        }
    }
    return 0;
}

static void release(void *p)
{
    for (uint8_t i = 0; i < BLOCKS; i++)
    {
        if (p == pool[i] && used[i])
        {
            used[i] = false;
            current--;
            return;
        }
    }
}

int main(void)
{
    void *p[BLOCKS];
    for (uint8_t i = 0; i < BLOCKS; i++)
    {
        p[i] = alloc();
        assert(p[i]);
        strcpy(p[i], "dynamic message payload longer than common message");
    }
    assert(alloc() == 0);
    for (uint8_t i = 0; i < BLOCKS; i++)
    {
        release(p[i]);
    }
    assert(current == 0U);
    printf("pool used max=%u\n", max_used);
    return 0;
}
