#include "task_inspector.h"
#include "critical_section.h"
#include "ready_queue.h"
#include "scheduler.h"
#include "task_registry.h"
#include "uart.h"
#include <stddef.h>
#include <stdint.h>

#define SNAPSHOT_MAX_TASKS (8U)

typedef struct
{
    const char *name;
    uint32_t saved_sp;
    uint32_t unused_words;
    uint32_t runtime_ticks;
    uint32_t switch_count;
    uint8_t id;
    uint8_t base_priority;
    uint8_t effective_priority;
    uint8_t state;
    bool guard_ok;
} task_snapshot_t;

static void newline(void) { uart1_write_string("\r\n"); }
static size_t snapshot_tasks(task_snapshot_t *out, size_t capacity)
{
    size_t count;
    const hr_irq_state_t state = hr_critical_enter();
    count = hr_task_registry_count();
    if (count > capacity) { count = capacity; }
    for (size_t i = 0U; i < count; ++i)
    {
        const hr_task_t *task = hr_task_registry_at(i);
        out[i].name = task->name;
        out[i].saved_sp = (uint32_t)(uintptr_t)task->saved_sp;
        out[i].unused_words = (uint32_t)hr_task_stack_unused_words(task);
        out[i].runtime_ticks = task->runtime_ticks;
        out[i].switch_count = task->switch_count;
        out[i].id = task->id;
        out[i].base_priority = task->base_priority;
        out[i].effective_priority = task->effective_priority;
        out[i].state = (uint8_t)task->state;
        out[i].guard_ok = hr_task_stack_guard_ok(task);
    }
    hr_critical_exit(state);
    return count;
}

void hr_task_inspector_print_help(void)
{
    uart1_write_string("h help | t tasks | r ready | c current | s stacks | v validate\r\n");
}

void hr_task_inspector_print_tasks(void)
{
    task_snapshot_t snapshots[SNAPSHOT_MAX_TASKS];
    const size_t count = snapshot_tasks(snapshots, SNAPSHOT_MAX_TASKS);
    uart1_write_string("task_count="); uart1_write_u32((uint32_t)count); newline();
    for (size_t i = 0U; i < count; ++i)
    {
        uart1_write_string("id="); uart1_write_u32(snapshots[i].id);
        uart1_write_string(" name="); uart1_write_string(snapshots[i].name);
        uart1_write_string(" state="); uart1_write_u32(snapshots[i].state);
        uart1_write_string(" base="); uart1_write_u32(snapshots[i].base_priority);
        uart1_write_string(" effective="); uart1_write_u32(snapshots[i].effective_priority);
        uart1_write_string(" saved_sp="); uart1_write_hex32(snapshots[i].saved_sp);
        uart1_write_string(" runtime="); uart1_write_u32(snapshots[i].runtime_ticks);
        uart1_write_string(" switches="); uart1_write_u32(snapshots[i].switch_count);
        newline();
    }
}

void hr_task_inspector_print_ready(void)
{
    uart1_write_string("ready_bitmap="); uart1_write_hex32(hr_ready_bitmap()); newline();
    for (uint8_t p = 0U; p < HR_PRIORITY_COUNT; ++p)
    {
        const hr_irq_state_t state = hr_critical_enter();
        const hr_list_t *list = hr_ready_list(p);
        const hr_list_node_t *node = (list != (const hr_list_t *)0) ? list->head : (const hr_list_node_t *)0;
        const char *names[SNAPSHOT_MAX_TASKS];
        size_t count = 0U;
        while ((node != (const hr_list_node_t *)0) && (count < SNAPSHOT_MAX_TASKS))
        {
            const hr_task_t *task = HR_CONTAINER_OF(node, hr_task_t, ready_node);
            names[count++] = task->name;
            node = node->next;
        }
        hr_critical_exit(state);
        uart1_write_string("P"); uart1_write_u32(p); uart1_write_string(": ");
        if (count == 0U) { uart1_write_string("empty"); }
        for (size_t i = 0U; i < count; ++i)
        {
            if (i != 0U) { uart1_write_string(" -> "); }
            uart1_write_string(names[i]);
        }
        newline();
    }
}

void hr_task_inspector_print_current(void)
{
    const hr_irq_state_t state = hr_critical_enter();
    const hr_task_t *task = hr_scheduler_current();
    const char *name = (task != (const hr_task_t *)0) ? task->name : "none";
    const uint32_t tick = g_kernel_tick;
    const uint32_t switches = g_context_switch_count;
    hr_critical_exit(state);
    uart1_write_string("current="); uart1_write_string(name);
    uart1_write_string(" tick="); uart1_write_u32(tick);
    uart1_write_string(" context_switches="); uart1_write_u32(switches); newline();
}

void hr_task_inspector_print_stacks(void)
{
    task_snapshot_t snapshots[SNAPSHOT_MAX_TASKS];
    const size_t count = snapshot_tasks(snapshots, SNAPSHOT_MAX_TASKS);
    for (size_t i = 0U; i < count; ++i)
    {
        uart1_write_string(snapshots[i].name);
        uart1_write_string(" guard="); uart1_write_string(snapshots[i].guard_ok ? "OK" : "FAIL");
        uart1_write_string(" unused_words="); uart1_write_u32(snapshots[i].unused_words); newline();
    }
}

void hr_task_inspector_print_validation(void)
{
    uart1_write_string("registry="); uart1_write_string(hr_task_registry_validate() ? "PASS" : "FAIL"); newline();
    uart1_write_string("ready="); uart1_write_string(hr_ready_validate() ? "PASS" : "FAIL"); newline();
    uart1_write_string("scheduler="); uart1_write_string(hr_scheduler_validate() ? "PASS" : "FAIL"); newline();
}
