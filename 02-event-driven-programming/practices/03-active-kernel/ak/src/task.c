#include "task.h"

#include "ak_port.h"
#include "event_record.h"

#include "runtime.h"

typedef struct
{
    bool registered;
    uint8_t id;
    uint8_t priority;
    const char *name;
    ak_task_handler_t handler;
    ak_message_t *queue[AK_TASK_QUEUE_CAPACITY];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
    uint8_t count_max;
    uint32_t posted;
    uint32_t dispatched;
    uint32_t dropped;
    uint32_t handler_time_max;
} task_control_t;

static task_control_t g_tasks[AK_TASK_CAPACITY];
static uint8_t g_current_task;
static uint32_t g_interrupt_nesting;

static task_control_t *find_task(uint8_t id)
{
    for (uint8_t i = 0U; i < AK_TASK_CAPACITY; i++)
    {
        if (g_tasks[i].registered && (g_tasks[i].id == id))
        {
            return &g_tasks[i];
        }
    }
    return 0;
}

void ak_task_system_init(void)
{
    (void) memset(g_tasks, 0, sizeof(g_tasks));
    g_current_task = 0U;
    g_interrupt_nesting = 0U;
}

bool ak_task_register(uint8_t task_id, uint8_t priority, const char *name, ak_task_handler_t handler)
{
    if ((task_id == 0U) || (handler == 0) || (find_task(task_id) != 0))
    {
        return false;
    }

    for (uint8_t i = 0U; i < AK_TASK_CAPACITY; i++)
    {
        if (!g_tasks[i].registered)
        {
            g_tasks[i].registered = true;
            g_tasks[i].id = task_id;
            g_tasks[i].priority = priority;
            g_tasks[i].name = name;
            g_tasks[i].handler = handler;
            return true;
        }
    }

    return false;
}

bool ak_task_post(ak_message_t *message)
{
    task_control_t *task;
    uint32_t key;
    bool result = false;

    if (message == 0)
    {
        return false;
    }

    task = find_task(message->destination);
    if (task == 0)
    {
        return false;
    }

    key = ak_port_critical_enter();

    if (task->count < AK_TASK_QUEUE_CAPACITY)
    {
        task->queue[task->tail] = message;
        task->tail = (uint8_t)((task->tail + 1U) % AK_TASK_QUEUE_CAPACITY);
        task->count++;
        task->posted++;
        if (task->count > task->count_max)
        {
            task->count_max = task->count;
        }
        result = true;
    }
    else
    {
        task->dropped++;
    }

    ak_port_critical_exit(key);

    ak_event_record_t record =
    {
        .timestamp = ak_port_time_now_ms(),
        .signal = message->signal,
        .value = task->count,
        .type = result ? AK_RECORD_MESSAGE_POST : AK_RECORD_MESSAGE_DROP,
        .source = message->source,
        .destination = message->destination,
        .state = task->priority
    };
    ak_event_record_write(&record);

    return result;
}

bool ak_task_post_from_isr(ak_message_t *message)
{
    return ak_task_post(message);
}

bool ak_task_run_once(void)
{
    task_control_t *selected = 0;
    ak_message_t *message;
    uint32_t start;
    uint32_t elapsed;
    uint32_t key;

    for (uint8_t i = 0U; i < AK_TASK_CAPACITY; i++)
    {
        if (g_tasks[i].registered && (g_tasks[i].count > 0U))
        {
            if ((selected == 0) || (g_tasks[i].priority > selected->priority))
            {
                selected = &g_tasks[i];
            }
        }
    }

    if (selected == 0)
    {
        return false;
    }

    key = ak_port_critical_enter();
    message = selected->queue[selected->head];
    selected->head = (uint8_t)((selected->head + 1U) % AK_TASK_QUEUE_CAPACITY);
    selected->count--;
    ak_port_critical_exit(key);

    g_current_task = selected->id;
    start = ak_port_time_now_ms();

    ak_event_record_t begin =
    {
        .timestamp = start,
        .signal = message->signal,
        .value = selected->count,
        .type = AK_RECORD_DISPATCH_START,
        .source = message->source,
        .destination = selected->id,
        .state = selected->priority
    };
    ak_event_record_write(&begin);

    selected->handler(message);

    elapsed = ak_port_time_now_ms() - start;
    if (elapsed > selected->handler_time_max)
    {
        selected->handler_time_max = elapsed;
    }
    selected->dispatched++;

    ak_event_record_t end = begin;
    end.timestamp = ak_port_time_now_ms();
    end.value = (uint16_t) elapsed;
    end.type = AK_RECORD_DISPATCH_END;
    ak_event_record_write(&end);

    ak_message_release(message);
    g_current_task = 0U;
    return true;
}

uint32_t ak_task_run_until_idle(uint32_t maximum_steps)
{
    uint32_t steps = 0U;
    while ((steps < maximum_steps) && ak_task_run_once())
    {
        steps++;
    }
    return steps;
}

bool ak_task_get_stats(uint8_t task_id, ak_task_stats_t *stats)
{
    task_control_t *task = find_task(task_id);
    if ((task == 0) || (stats == 0))
    {
        return false;
    }

    stats->id = task->id;
    stats->priority = task->priority;
    stats->name = task->name;
    stats->queue_depth = task->count;
    stats->queue_depth_max = task->count_max;
    stats->posted = task->posted;
    stats->dispatched = task->dispatched;
    stats->dropped = task->dropped;
    stats->handler_time_max = task->handler_time_max;
    return true;
}

uint8_t ak_task_current_id(void)
{
    return g_current_task;
}

uint32_t ak_task_interrupt_nesting(void)
{
    return g_interrupt_nesting;
}

void ak_task_entry_interrupt(void)
{
    g_interrupt_nesting++;
}

void ak_task_exit_interrupt(void)
{
    if (g_interrupt_nesting > 0U)
    {
        g_interrupt_nesting--;
    }
}
