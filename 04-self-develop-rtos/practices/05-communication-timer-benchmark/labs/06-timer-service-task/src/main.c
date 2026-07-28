#include "clock.h"
#include "cortex_m3_port.h"
#include "gpio.h"
#include "panic.h"
#include "scheduler.h"
#include "semaphore.h"
#include "software_timer.h"
#include "systick.h"
#include "task.h"
#include "timer_service.h"
#include "uart.h"
static hr_semaphore_t signal;static hr_software_timer_t periodic,one;static hr_task_t service,idle;static uint32_t ss[192]__attribute__((aligned(8))),is[96]__attribute__((aligned(8)));volatile unsigned periodic_count,one_count;
static void pcb(void*a){(void)a;periodic_count++;gpio_led_toggle();}static void ocb(void*a){(void)a;one_count++;}
static void idle_task(void*a){(void)a;for(;;)hr_port_wait_for_interrupt();}
int main(void){clock_init_hsi_8mhz();gpio_led_init();uart1_init_9600_hsi8();hr_scheduler_init();HR_ASSERT(hr_semaphore_init(&signal,"timer",0,1));hr_software_timer_system_init(&signal);hr_timer_service_bind(&signal);HR_ASSERT(hr_software_timer_init(&periodic,"periodic",pcb,0));HR_ASSERT(hr_software_timer_init(&one,"one",ocb,0));HR_ASSERT(hr_task_create_static(&service,"timer-service",0,0,hr_timer_service_task,0,ss,192));HR_ASSERT(hr_task_create_static(&idle,"idle",1,3,idle_task,0,is,96));HR_ASSERT(hr_scheduler_add_task(&service));HR_ASSERT(hr_scheduler_add_task(&idle));HR_ASSERT(hr_software_timer_start(&periodic,250,250));HR_ASSERT(hr_software_timer_start(&one,1000,0));systick_init_1khz();hr_scheduler_start();}
