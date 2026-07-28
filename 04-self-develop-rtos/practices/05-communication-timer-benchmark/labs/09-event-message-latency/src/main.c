#include "benchmark_clock.h"
#include "benchmark_stats.h"
#include "clock.h"
#include "cortex_m3_port.h"
#include "gpio.h"
#include "panic.h"
#include "queue.h"
#include "scheduler.h"
#include "stm32f1.h"
#include "systick.h"
#include "task.h"
#include "uart.h"
typedef struct{uint32_t stamp;uint32_t sequence;}msg_t;static hr_queue_t q;static msg_t storage[8];static hr_task_t consumer,monitor,idle;static uint32_t cs[160]__attribute__((aligned(8))),ms[256]__attribute__((aligned(8))),is[96]__attribute__((aligned(8)));static hr_benchmark_stats_t stats;static const uint32_t bins[8]={16,32,64,128,256,512,1024,4096};
static void ct(void*x){(void)x;msg_t m;for(;;)if(hr_queue_receive(&q,&m,HR_WAIT_FOREVER)==HR_WAIT_SUCCESS)hr_benchmark_stats_add(&stats,hr_benchmark_now()-m.stamp);}
static void mt(void*x){(void)x;for(;;){uart1_write_string("samples=");uart1_write_u32(stats.count);uart1_write_string(" max=");uart1_write_u32(stats.maximum);uart1_write_string(" avg=");uart1_write_u32(hr_benchmark_stats_average(&stats));uart1_write_string("\r\n");hr_task_delay(1000);}}
static void it(void*x){(void)x;for(;;)hr_port_wait_for_interrupt();}
void EXTI0_IRQHandler(void){static uint32_t n;bool higher=false;msg_t m={hr_benchmark_now(),++n};EXTI_PR=1UL;(void)hr_queue_send_from_isr(&q,&m,&higher);}
int main(void){clock_init_hsi_8mhz();gpio_button_exti0_init();uart1_init_9600_hsi8();hr_benchmark_clock_init();hr_benchmark_stats_init(&stats,bins,8);hr_scheduler_init();HR_ASSERT(hr_queue_init_static(&q,"events",storage,8,sizeof(msg_t)));HR_ASSERT(hr_task_create_static(&consumer,"consumer",0,0,ct,0,cs,160));HR_ASSERT(hr_task_create_static(&monitor,"monitor",1,2,mt,0,ms,256));HR_ASSERT(hr_task_create_static(&idle,"idle",2,3,it,0,is,96));HR_ASSERT(hr_scheduler_add_task(&consumer));HR_ASSERT(hr_scheduler_add_task(&monitor));HR_ASSERT(hr_scheduler_add_task(&idle));systick_init_1khz();hr_scheduler_start();}
