#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef enum
{
    DEST_A = 0,
    DEST_B,
    DEST_C,
    DEST_COUNT
} destination_t;

typedef struct
{
    uint8_t destination;
    uint16_t signal;
    uint32_t parameter;
} event_t;

typedef void (*handler_t)(const event_t *event);

static uint32_t g_a_count;
static uint32_t g_b_count;
static uint32_t g_c_count;
static uint32_t g_invalid_count;

static void handler_a(const event_t *event)
{
    g_a_count++;
    printf("A,%u,%lu\n", (unsigned int) event->signal, (unsigned long) event->parameter);
}

static void handler_b(const event_t *event)
{
    g_b_count++;
    printf("B,%u,%lu\n", (unsigned int) event->signal, (unsigned long) event->parameter);
}

static void handler_c(const event_t *event)
{
    g_c_count++;
    printf("C,%u,%lu\n", (unsigned int) event->signal, (unsigned long) event->parameter);
}

static bool dispatch(const event_t *event, const handler_t *table)
{
    if ((event == NULL) || (event->destination >= (uint8_t) DEST_COUNT) || (table[event->destination] == NULL))
    {
        g_invalid_count++;
        return false;
    }

    table[event->destination](event);
    return true;
}

int main(void)
{
    const handler_t table[DEST_COUNT] =
    {
        handler_a,
        handler_b,
        handler_c
    };

    const event_t events[] =
    {
        {
            DEST_A, 10U, 100UL
        },
        {
            DEST_B,
            20U,
            200UL
        },
        {
            DEST_C,
            30U,
            300UL
        },
        {
            99U,
            40U,
            400UL
        }
    };

    for (size_t i = 0U; i < (sizeof(events) / sizeof(events[0])); i++)
    {
        (void) dispatch(&events[i], table);
    }

    printf("counts=%lu,%lu,%lu invalid=%lu\n", (unsigned long) g_a_count, (unsigned long) g_b_count, (unsigned long) g_c_count,
        (unsigned long) g_invalid_count);

    return ((g_a_count == 1UL) && (g_b_count == 1UL) && (g_c_count == 1UL) && (g_invalid_count == 1UL)) ? 0 : 1;
}
