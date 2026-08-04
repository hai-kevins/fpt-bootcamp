#include "memory_explorer.h"
#include "command_parser.h"
#include "heap.h"
#include "heap_stats.h"
#include "memory_layout.h"
#include "uart.h"
#include <stdbool.h>
#include <stddef.h>

#define SLOT_COUNT (16U)

static void *g_slots[SLOT_COUNT];
static uint8_t *g_heap_memory;
static size_t g_heap_size;

static bool string_equal(const char *a, const char *b)
{
    if ((a == (const char *)0) || (b == (const char *)0))
    {
        return false;
    }
    while ((*a != '\0') && (*b != '\0'))
    {
        if (*a != *b)
        {
            return false;
        }
        a++;
        b++;
    }
    return (*a == '\0') && (*b == '\0');
}

static void clear_slots(void)
{
    size_t index;
    for (index = 0U; index < SLOT_COUNT; index++)
    {
        g_slots[index] = (void *)0;
    }
}

bool memory_explorer_init(uint8_t *heap_memory, size_t heap_size)
{
    g_heap_memory = heap_memory;
    g_heap_size = heap_size;
    clear_slots();
    return heap_init(heap_memory, heap_size) == HEAP_STATUS_OK;
}

void memory_explorer_print_help(void)
{
    uart_write_line("help");
    uart_write_line("memory");
    uart_write_line("alloc <bytes>");
    uart_write_line("free <id>");
    uart_write_line("blocks");
    uart_write_line("stats");
    uart_write_line("validate");
    uart_write_line("fragment-demo");
    uart_write_line("reset");
}

static void print_stats(void)
{
    heap_stats_t s;
    if (heap_get_stats(&s) != HEAP_STATUS_OK)
    {
        uart_write_line("stats error");
        return;
    }
    uart_write_string("heap=");
    uart_write_size(s.heap_size);
    uart_write_string(" used=");
    uart_write_size(s.used_payload_bytes);
    uart_write_string(" free=");
    uart_write_size(s.free_payload_bytes);
    uart_write_string(" largest=");
    uart_write_size(s.largest_free_block);
    uart_write_string(" internal-frag=");
    uart_write_size(s.internal_fragmentation_bytes);
    uart_write_string(" blocks=");
    uart_write_size(s.block_count);
    uart_write_string(" free-blocks=");
    uart_write_size(s.free_block_count);
    uart_write_string(" external-frag-per-mille=");
    uart_write_u32(heap_external_fragmentation_per_mille(&s));
    uart_write_string("\r\n");
}

static void print_blocks(void)
{
    size_t sizes[32];
    bool free_flags[32];
    size_t count;
    size_t index;
    count = heap_dump_blocks(sizes, free_flags, 32U);
    for (index = 0U; index < count && index < 32U; index++)
    {
        uart_write_char('[');
        uart_write_size(index);
        uart_write_string("] ");
        uart_write_string(free_flags[index] ? "FREE " : "USED ");
        uart_write_size(sizes[index]);
        uart_write_string("\r\n");
    }
}

static void allocate_slot(uint32_t size)
{
    size_t index;
    for (index = 0U; index < SLOT_COUNT; index++)
    {
        if (g_slots[index] == (void *)0)
        {
            g_slots[index] = heap_alloc((size_t) size);
            if (g_slots[index] == (void *)0)
            {
                uart_write_line("allocation failed");
                return;
            }
            uart_write_string("id=");
            uart_write_size(index);
            uart_write_string(" ptr=");
            uart_write_pointer(g_slots[index]);
            uart_write_string("\r\n");
            return;
        }
    }
    uart_write_line("slot table full");
}

static void free_slot(uint32_t id)
{
    heap_status_t status;
    if ((id >= SLOT_COUNT) || (g_slots[id] == (void *)0))
    {
        uart_write_line("invalid id");
        return;
    }
    status = heap_free(g_slots[id]);
    if (status == HEAP_STATUS_OK)
    {
        g_slots[id] = (void *)0;
    }
    uart_write_string("status=");
    uart_write_u32((uint32_t) status);
    uart_write_string("\r\n");
}

static void reset_heap(void)
{
    clear_slots();
    uart_write_line(heap_init(g_heap_memory, g_heap_size) == HEAP_STATUS_OK ? "heap reset: PASS" : "heap reset: FAIL");
}

static void fragment_demo(void)
{
    void *a = heap_alloc(24U);
    void *b = heap_alloc(160U);
    void *c = heap_alloc(24U);
    void *d = heap_alloc(160U);
    (void) a;
    (void) c;
    (void) heap_free(b);
    (void) heap_free(d);
    print_blocks();
    print_stats();
}

void memory_explorer_execute(const char *line)
{
    command_t command;
    if (!command_parse(line, &command))
    {
        uart_write_line("parse error");
        return;
    }
    if (string_equal(command.name, "help"))
    {
        memory_explorer_print_help();
    }
    else if (string_equal(command.name, "memory"))
    {
        memory_layout_print();
    }
    else if (string_equal(command.name, "alloc") && command.has_argument)
    {
        allocate_slot(command.argument);
    }
    else if (string_equal(command.name, "free") && command.has_argument)
    {
        free_slot(command.argument);
    }
    else if (string_equal(command.name, "blocks"))
    {
        print_blocks();
    }
    else if (string_equal(command.name, "stats"))
    {
        print_stats();
    }
    else if (string_equal(command.name, "validate"))
    {
        uart_write_line(heap_validate() ? "PASS" : "FAIL");
    }
    else if (string_equal(command.name, "fragment-demo"))
    {
        fragment_demo();
    }
    else if (string_equal(command.name, "reset"))
    {
        reset_heap();
    }
    else
    {
        uart_write_line("unknown command");
    }
}
