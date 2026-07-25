#include "clock.h"
#include "cortex_m3_port.h"
#include "gpio.h"
#include "panic.h"
#include "scheduler.h"
#include "semaphore.h"
#include "stm32f1.h"
#include "systick.h"
#include "task.h"
#include "uart.h"
#include <stdbool.h>
#include <stdint.h>

static rtos_task_t high_task_tcb, low_task_tcb, idle_task_tcb;
static uint32_t high_stack[128] __attribute__((aligned(8)));
static uint32_t low_stack[128] __attribute__((aligned(8)));
static uint32_t idle_stack[96] __attribute__((aligned(8)));
static rtos_semaphore_t event_sem;
volatile uint32_t high_runs, low_runs, irq_count;

static void high_task(void *a){(void)a;for(;;){if(rtos_semaphore_take(&event_sem,RTOS_WAIT_FOREVER)==RTOS_WAIT_SUCCESS){++high_runs;gpio_led_toggle();}}}
static void low_task(void *a){(void)a;for(;;){++low_runs;}}
static void idle_task(void *a){(void)a;for(;;)rtos_port_wait_for_interrupt();}
void EXTI0_IRQHandler(void){bool higher=false;EXTI_PR=1UL;++irq_count;(void)rtos_semaphore_give_from_isr(&event_sem,&higher);(void)higher;}
int main(void){clock_init_hsi_8mhz();gpio_led_init();gpio_button_exti0_init();uart1_init_9600_hsi8();rtos_scheduler_init();RTOS_ASSERT(rtos_semaphore_init(&event_sem,"event",0,1));RTOS_ASSERT(rtos_task_create_static(&high_task_tcb,"high",0,0,high_task,0,high_stack,128));RTOS_ASSERT(rtos_task_create_static(&low_task_tcb,"low",1,2,low_task,0,low_stack,128));RTOS_ASSERT(rtos_task_create_static(&idle_task_tcb,"idle",2,3,idle_task,0,idle_stack,96));RTOS_ASSERT(rtos_scheduler_add_task(&high_task_tcb));RTOS_ASSERT(rtos_scheduler_add_task(&low_task_tcb));RTOS_ASSERT(rtos_scheduler_add_task(&idle_task_tcb));systick_init_1khz();rtos_scheduler_start();}
