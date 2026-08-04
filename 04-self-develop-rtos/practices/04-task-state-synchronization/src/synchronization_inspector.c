#include "synchronization_inspector.h"
#include "critical_section.h"
#include "ready_queue.h"
#include "scheduler.h"
#include "task_registry.h"
#include "timeout.h"
#include "uart.h"
#include <stddef.h>
#include <stdint.h>

#define SNAPSHOT_MAX_TASKS (10U)

static rtos_semaphore_t *g_event_sem;
static rtos_semaphore_t *g_count_sem;
static rtos_mutex_t *g_resource_mutex;

static void newline(void)
{
    uart1_write_string("\r\n");
}

void rtos_sync_inspector_bind(rtos_semaphore_t *event_sem, rtos_semaphore_t *count_sem, rtos_mutex_t *resource_mutex)
{
    g_event_sem = event_sem;
    g_count_sem = count_sem;
    g_resource_mutex = resource_mutex;
}

void rtos_sync_inspector_print_help(void)
{
    uart1_write_string("h help | t tasks | d delayed | s semaphores | m mutex | v validate\r\n");
}

void rtos_sync_inspector_print_tasks(void)
{
    const rtos_irq_state_t state = rtos_critical_enter();
    const size_t count = rtos_task_registry_count();
    const size_t limit = (count < SNAPSHOT_MAX_TASKS) ? count : SNAPSHOT_MAX_TASKS;
    const rtos_task_t *tasks[SNAPSHOT_MAX_TASKS];
    for (size_t i = 0U; i < limit; ++i)
    {
        tasks[i] = rtos_task_registry_at(i);
    }
    rtos_critical_exit(state);

    uart1_write_string("task_count=");
    uart1_write_u32((uint32_t) limit);
    newline();
    for (size_t i = 0U; i < limit; ++i)
    {
        const rtos_task_t *task = tasks[i];
        uart1_write_string(task->name);
        uart1_write_string(" state=");
        uart1_write_string(rtos_task_state_name(task->state));
        uart1_write_string(" base=");
        uart1_write_u32(task->base_priority);
        uart1_write_string(" effective=");
        uart1_write_u32(task->effective_priority);
        uart1_write_string(" wait=");
        uart1_write_string(rtos_wait_result_name(task->wait_result));
        uart1_write_string(" wake=");
        uart1_write_u32(task->wake_tick);
        newline();
    }
}

void rtos_sync_inspector_print_delayed(void)
{
    const rtos_irq_state_t state = rtos_critical_enter();
    const rtos_list_t *list = rtos_timeout_list();
    const rtos_list_node_t *node = list->head;
    const char *names[SNAPSHOT_MAX_TASKS];
    uint32_t wakes[SNAPSHOT_MAX_TASKS];
    size_t count = 0U;
    while ((node != (const rtos_list_node_t *)0) && (count < SNAPSHOT_MAX_TASKS))
    {
        const rtos_task_t *task = RTOS_CONTAINER_OF(node, rtos_task_t, timeout_node);
        names[count] = task->name;
        wakes[count] = task->wake_tick;
        ++count;
        node = node->next;
    }
    rtos_critical_exit(state);
    uart1_write_string("delayed_count=");
    uart1_write_u32((uint32_t) count);
    newline();
    for (size_t i = 0U; i < count; ++i)
    {
        uart1_write_string(names[i]);
        uart1_write_string(" wake=");
        uart1_write_u32(wakes[i]);
        newline();
    }
}

static void print_sem(const rtos_semaphore_t *sem)
{
    if (sem == (const rtos_semaphore_t *)0)
    {
        return;
    }
    uart1_write_string(sem->name);
    uart1_write_string(" count=");
    uart1_write_u32(sem->count);
    uart1_write_string(" max=");
    uart1_write_u32(sem->max_count);
    uart1_write_string(" waiters=");
    uart1_write_u32((uint32_t) rtos_list_count(&sem->waiters));
    newline();
}

void rtos_sync_inspector_print_semaphores(void)
{
    const rtos_irq_state_t state = rtos_critical_enter();
    print_sem(g_event_sem);
    print_sem(g_count_sem);
    rtos_critical_exit(state);
}

void rtos_sync_inspector_print_mutex(void)
{
    const rtos_irq_state_t state = rtos_critical_enter();
    if (g_resource_mutex != (rtos_mutex_t *)0)
    {
        uart1_write_string(g_resource_mutex->name);
        uart1_write_string(" owner=");
        uart1_write_string((g_resource_mutex->owner != (rtos_task_t *)0) ? g_resource_mutex->owner->name : "none");
        uart1_write_string(" waiters=");
        uart1_write_u32((uint32_t) rtos_list_count(&g_resource_mutex->waiters));
        newline();
    }
    rtos_critical_exit(state);
}

void rtos_sync_inspector_print_validation(void)
{
    bool sem_ok;
    bool mutex_ok;
    const rtos_irq_state_t state = rtos_critical_enter();
    sem_ok = rtos_semaphore_validate(g_event_sem) && rtos_semaphore_validate(g_count_sem);
    mutex_ok = rtos_mutex_validate(g_resource_mutex);
    rtos_critical_exit(state);
    uart1_write_string("scheduler=");
    uart1_write_string(rtos_scheduler_validate() ? "PASS" : "FAIL");
    newline();
    uart1_write_string("semaphores=");
    uart1_write_string(sem_ok ? "PASS" : "FAIL");
    newline();
    uart1_write_string("mutex=");
    uart1_write_string(mutex_ok ? "PASS" : "FAIL");
    newline();
}
