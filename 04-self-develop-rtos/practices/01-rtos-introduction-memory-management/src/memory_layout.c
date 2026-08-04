#include "memory_layout.h"
#include "uart.h"
#include <stdint.h>

extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _end;
extern uint32_t _sstack;
extern uint32_t _estack;

static void print(const char *name, const void *value)
{
    uart_write_string(name);
    uart_write_string(": ");
    uart_write_pointer(value);
    uart_write_string("\r\n");
}

void memory_layout_print(void)
{
    print("text end", &_etext);
    print("data start", &_sdata);
    print("data end", &_edata);
    print("bss start", &_sbss);
    print("bss end", &_ebss);
    print("static end", &_end);
    print("stack low", &_sstack);
    print("stack top", &_estack);
}
