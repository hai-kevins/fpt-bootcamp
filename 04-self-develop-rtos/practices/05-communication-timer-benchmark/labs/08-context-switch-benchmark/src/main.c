#include "benchmark_clock.h"
#include "benchmark_stats.h"
#include "clock.h"
#include "cortex_m3_port.h"
#include "panic.h"
#include "scheduler.h"
#include "systick.h"
#include "task.h"
#include "uart.h"
static hr_task_t a,b,monitor,idle;static uint32_t as[128]__attribute__((aligned(8))),bs[128]__attribute__((aligned(8))),ms[256]__attribute__((aligned(8))),is[96]__attribute__((aligned(8)));static volatile uint32_t turn,start;static hr_benchmark_stats_t stats;static const uint32_t bins[8]={16,32,64,128,256,512,1024,4096};
static void ta(void*x){(void)x;for(;;){if(turn==0){start=hr_benchmark_now();turn=1;hr_task_yield();}else hr_task_delay(1);}}
static void tb(void*x){(void)x;for(;;){if(turn==1){hr_benchmark_stats_add(&stats,hr_benchmark_now()-start);turn=0;hr_task_yield();}else hr_task_delay(1);}}
static void tm(void*x){(void)x;for(;;){uart1_write_string("samples=");uart1_write_u32(stats.count);uart1_write_string(" min=");uart1_write_u32(stats.minimum);uart1_write_string(" max=");uart1_write_u32(stats.maximum);uart1_write_string(" avg=");uart1_write_u32(hr_benchmark_stats_average(&stats));uart1_write_string("\r\n");hr_task_delay(1000);}}
static void ti(void*x){(void)x;for(;;)hr_port_wait_for_interrupt();}
int main(void){clock_init_hsi_8mhz();uart1_init_9600_hsi8();hr_benchmark_clock_init();hr_benchmark_stats_init(&stats,bins,8);hr_scheduler_init();HR_ASSERT(hr_task_create_static(&a,"a",0,1,ta,0,as,128));HR_ASSERT(hr_task_create_static(&b,"b",1,1,tb,0,bs,128));HR_ASSERT(hr_task_create_static(&monitor,"monitor",2,2,tm,0,ms,256));HR_ASSERT(hr_task_create_static(&idle,"idle",3,3,ti,0,is,96));HR_ASSERT(hr_scheduler_add_task(&a));HR_ASSERT(hr_scheduler_add_task(&b));HR_ASSERT(hr_scheduler_add_task(&monitor));HR_ASSERT(hr_scheduler_add_task(&idle));systick_init_1khz();hr_scheduler_start();}
