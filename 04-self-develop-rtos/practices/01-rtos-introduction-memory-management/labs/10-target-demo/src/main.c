#include "heap.h"
#include "compiler.h"
#include "gpio.h"
#include "systick.h"
#include "uart.h"
#include <stdint.h>

static uint8_t g_heap[2048] ATTR_ALIGNED(8);

static void print_stats(void)
{
    heap_stats_t stats;
    if (heap_get_stats(&stats) != HEAP_STATUS_OK)
    {
        return;
    }
    uart_write_string("blocks=");
    uart_write_size(stats.block_count);
    uart_write_string(" used=");
    uart_write_size(stats.used_payload_bytes);
    uart_write_string(" free=");
    uart_write_size(stats.free_payload_bytes);
    uart_write_string(" largest=");
    uart_write_size(stats.largest_free_block);
    uart_write_string("\r\n");
}

int main(void)
{
    void *a;
    void *b;
    void *c;
    uart_init(9600UL, 8000000UL);
    (void) systick_init(8000000UL, 1000UL);
    gpio_enable_clock(GPIO_PORT_C);
    gpio_config_output_push_pull(GPIO_PORT_C, 13UL);
    gpio_write(GPIO_PORT_C, 13UL, true);
    uart_write_line("Lab 10 - First-fit allocator target demo");
    if (heap_init(g_heap, sizeof(g_heap)) != HEAP_STATUS_OK)
    {
        uart_write_line("heap init FAIL");
    }
    a = heap_alloc(64U);
    b = heap_alloc(128U);
    c = heap_alloc(48U);
    print_stats();
    (void) heap_free(b);
    print_stats();
    b = heap_alloc(80U);
    print_stats();
    (void) heap_free(a);
    (void) heap_free(b);
    (void) heap_free(c);
    print_stats();
    uart_write_string("validator: ");
    uart_write_line(heap_validate() ? "PASS" : "FAIL");
    for (;;)
    {
        gpio_toggle(GPIO_PORT_C, 13UL);
        systick_delay_ms(500UL);
    }
}
