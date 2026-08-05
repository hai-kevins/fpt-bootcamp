#include "memory_explorer.h"

#include "cmd_line.h"
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

static cmd_line_status_t command_help(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_memory(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_alloc(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_free(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_blocks(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_stats(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_validate(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_fragment_demo(
    const char *arguments,
    void *context
);
static cmd_line_status_t command_reset(
    const char *arguments,
    void *context
);

static const cmd_line_t g_command_table[] =
{
    {
        "help",
        command_help,
        "help                 - show this command list"
    },
    {
        "memory",
        command_memory,
        "memory               - show the MCU memory layout"
    },
    {
        "alloc",
        command_alloc,
        "alloc <bytes>        - allocate a heap block"
    },
    {
        "free",
        command_free,
        "free <id>            - release an allocated slot"
    },
    {
        "blocks",
        command_blocks,
        "blocks               - list heap blocks"
    },
    {
        "stats",
        command_stats,
        "stats                - show heap statistics"
    },
    {
        "validate",
        command_validate,
        "validate             - validate heap metadata"
    },
    {
        "fragment-demo",
        command_fragment_demo,
        "fragment-demo        - create external fragmentation"
    },
    {
        "reset",
        command_reset,
        "reset                - reset the heap"
    },
    {
        NULL,
        NULL,
        NULL
    }
};

static bool no_arguments(const char *arguments)
{
    return cmd_line_argument_is_empty(arguments);
}

static void clear_slots(void)
{
    size_t index;

    for (index = 0U; index < SLOT_COUNT; index++)
    {
        g_slots[index] = NULL;
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
    size_t index = 0U;

    while (g_command_table[index].cmd != NULL)
    {
        uart_write_line(g_command_table[index].info);
        index++;
    }
}

static void print_stats(void)
{
    heap_stats_t stats;

    if (heap_get_stats(&stats) != HEAP_STATUS_OK)
    {
        uart_write_line("stats error");
        return;
    }

    uart_write_string("heap=");
    uart_write_size(stats.heap_size);
    uart_write_string(" used=");
    uart_write_size(stats.used_payload_bytes);
    uart_write_string(" free=");
    uart_write_size(stats.free_payload_bytes);
    uart_write_string(" largest=");
    uart_write_size(stats.largest_free_block);
    uart_write_string(" internal-frag=");
    uart_write_size(stats.internal_fragmentation_bytes);
    uart_write_string(" blocks=");
    uart_write_size(stats.block_count);
    uart_write_string(" free-blocks=");
    uart_write_size(stats.free_block_count);
    uart_write_string(" external-frag-per-mille=");
    uart_write_u32(heap_external_fragmentation_per_mille(&stats));
    uart_write_string("\r\n");
}

static void print_blocks(void)
{
    size_t sizes[32];
    bool free_flags[32];
    size_t count;
    size_t index;

    count = heap_dump_blocks(sizes, free_flags, 32U);

    for (index = 0U;
         (index < count) && (index < 32U);
         index++)
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
        if (g_slots[index] == NULL)
        {
            g_slots[index] = heap_alloc((size_t)size);

            if (g_slots[index] == NULL)
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

    if ((id >= SLOT_COUNT) || (g_slots[id] == NULL))
    {
        uart_write_line("invalid id");
        return;
    }

    status = heap_free(g_slots[id]);

    if (status == HEAP_STATUS_OK)
    {
        g_slots[id] = NULL;
    }

    uart_write_string("status=");
    uart_write_u32((uint32_t)status);
    uart_write_string("\r\n");
}

static void reset_heap(void)
{
    clear_slots();
    uart_write_line(
        heap_init(g_heap_memory, g_heap_size) == HEAP_STATUS_OK
            ? "heap reset: PASS"
            : "heap reset: FAIL"
    );
}

static void fragment_demo(void)
{
    void *block_a = heap_alloc(24U);
    void *block_b = heap_alloc(160U);
    void *block_c = heap_alloc(24U);
    void *block_d = heap_alloc(160U);

    (void)block_a;
    (void)block_c;
    (void)heap_free(block_b);
    (void)heap_free(block_d);

    print_blocks();
    print_stats();
}

static cmd_line_status_t command_help(
    const char *arguments,
    void *context
)
{
    (void)context;

    if (!no_arguments(arguments))
    {
        return CMD_INVALID_ARG;
    }

    memory_explorer_print_help();

    return CMD_SUCCESS;
}

static cmd_line_status_t command_memory(
    const char *arguments,
    void *context
)
{
    (void)context;

    if (!no_arguments(arguments))
    {
        return CMD_INVALID_ARG;
    }

    memory_layout_print();

    return CMD_SUCCESS;
}

static cmd_line_status_t command_alloc(
    const char *arguments,
    void *context
)
{
    uint32_t size;

    (void)context;

    if (!cmd_line_parse_u32(arguments, &size))
    {
        return CMD_INVALID_ARG;
    }

    allocate_slot(size);

    return CMD_SUCCESS;
}

static cmd_line_status_t command_free(
    const char *arguments,
    void *context
)
{
    uint32_t id;

    (void)context;

    if (!cmd_line_parse_u32(arguments, &id))
    {
        return CMD_INVALID_ARG;
    }

    free_slot(id);

    return CMD_SUCCESS;
}

static cmd_line_status_t command_blocks(
    const char *arguments,
    void *context
)
{
    (void)context;

    if (!no_arguments(arguments))
    {
        return CMD_INVALID_ARG;
    }

    print_blocks();

    return CMD_SUCCESS;
}

static cmd_line_status_t command_stats(
    const char *arguments,
    void *context
)
{
    (void)context;

    if (!no_arguments(arguments))
    {
        return CMD_INVALID_ARG;
    }

    print_stats();

    return CMD_SUCCESS;
}

static cmd_line_status_t command_validate(
    const char *arguments,
    void *context
)
{
    (void)context;

    if (!no_arguments(arguments))
    {
        return CMD_INVALID_ARG;
    }

    uart_write_line(heap_validate() ? "PASS" : "FAIL");

    return CMD_SUCCESS;
}

static cmd_line_status_t command_fragment_demo(
    const char *arguments,
    void *context
)
{
    (void)context;

    if (!no_arguments(arguments))
    {
        return CMD_INVALID_ARG;
    }

    fragment_demo();

    return CMD_SUCCESS;
}

static cmd_line_status_t command_reset(
    const char *arguments,
    void *context
)
{
    (void)context;

    if (!no_arguments(arguments))
    {
        return CMD_INVALID_ARG;
    }

    reset_heap();

    return CMD_SUCCESS;
}

void memory_explorer_execute(const char *line)
{
    const cmd_line_status_t status = cmd_line_parser(
        g_command_table,
        line,
        NULL
    );

    switch (status)
    {
        case CMD_SUCCESS:
        case CMD_EMPTY:
        {
            break;
        }

        case CMD_NOT_FOUND:
        {
            uart_write_line("unknown command");
            break;
        }

        case CMD_TOO_LONG:
        {
            uart_write_line("command too long");
            break;
        }

        case CMD_INVALID_ARG:
        {
            uart_write_line("invalid argument");
            break;
        }

        default:
        {
            uart_write_line("command error");
            break;
        }
    }
}
