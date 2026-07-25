# Chủ đề 4 - Các thành phần chính của hệ thống Event-Driven trong Embedded
## Active Object, Mailbox, Event Pool và Data-Link Layer

> Mục tiêu của chủ đề này là hiểu cấu trúc bên trong của một hệ thống Event-Driven hoàn chỉnh, từ một MCU đơn lẻ đến hệ thống nhiều MCU, nhiều process hoặc nhiều node.
>
> Nội dung trọng tâm gồm Active Object, Mailbox, Event Handler, State Machine, Event Pool, Event Routing, Data-Link Layer và kiến trúc phân tán.

---

## Mục lục

1. [Mục tiêu học tập](#1-mục-tiêu-học-tập)
2. [Tổng quan kiến trúc Event-Driven](#2-tổng-quan-kiến-trúc-event-driven)
3. [Active Object là gì?](#3-active-object-là-gì)
4. [Cấu trúc bên trong Active Object](#4-cấu-trúc-bên-trong-active-object)
5. [Mailbox](#5-mailbox)
6. [Event Handler](#6-event-handler)
7. [Run-to-Completion](#7-run-to-completion)
8. [Scheduler và Dispatcher](#8-scheduler-và-dispatcher)
9. [State Machine trong hệ thống Event-Driven](#9-state-machine-trong-hệ-thống-event-driven)
10. [Flat State Machine](#10-flat-state-machine)
11. [Hierarchical State Machine](#11-hierarchical-state-machine)
12. [Table-Driven State Machine](#12-table-driven-state-machine)
13. [State Pattern và Function-Based State Machine](#13-state-pattern-và-function-based-state-machine)
14. [So sánh các loại State Machine](#14-so-sánh-các-loại-state-machine)
15. [Entry, Exit và Transition Action](#15-entry-exit-và-transition-action)
16. [Event Pool](#16-event-pool)
17. [Vì sao Event Pool là xương sống của hệ thống](#17-vì-sao-event-pool-là-xương-sống-của-hệ-thống)
18. [Static Event, Common Event và Dynamic Event](#18-static-event-common-event-và-dynamic-event)
19. [Reference Count và Ownership](#19-reference-count-và-ownership)
20. [Memory Pool](#20-memory-pool)
21. [Kích thước pool và High-Water Mark](#21-kích-thước-pool-và-high-water-mark)
22. [Event Routing](#22-event-routing)
23. [Direct Post, Publish-Subscribe và Broadcast](#23-direct-post-publish-subscribe-và-broadcast)
24. [Data-Link Layer là gì?](#24-data-link-layer-là-gì)
25. [Vai trò của Data-Link Layer](#25-vai-trò-của-data-link-layer)
26. [Kiến trúc nhiều MCU](#26-kiến-trúc-nhiều-mcu)
27. [Kiến trúc nhiều process trên MPU](#27-kiến-trúc-nhiều-process-trên-mpu)
28. [Event qua network](#28-event-qua-network)
29. [Event Serialization](#29-event-serialization)
30. [Frame Format](#30-frame-format)
31. [Addressing và Routing giữa các node](#31-addressing-và-routing-giữa-các-node)
32. [Error Detection và Reliability](#32-error-detection-và-reliability)
33. [Ví dụ Event qua UART](#33-ví-dụ-event-qua-uart)
34. [Ví dụ Event qua CAN](#34-ví-dụ-event-qua-can)
35. [Ví dụ hệ thống nhiều MCU](#35-ví-dụ-hệ-thống-nhiều-mcu)
36. [Sequence Diagram tổng thể](#36-sequence-diagram-tổng-thể)
37. [Quản lý concurrency](#37-quản-lý-concurrency)
38. [Interrupt và Active Object](#38-interrupt-và-active-object)
39. [Quản lý lỗi](#39-quản-lý-lỗi)
40. [Logging và Event Trace](#40-logging-và-event-trace)
41. [Thiết kế module](#41-thiết-kế-module)
42. [Quy trình phát triển](#42-quy-trình-phát-triển)
43. [Best Practices](#43-best-practices)
44. [Anti-pattern và lỗi thường gặp](#44-anti-pattern-và-lỗi-thường-gặp)
45. [Kiểm thử](#45-kiểm-thử)
46. [Bài thực hành](#46-bài-thực-hành)
47. [Tiêu chí hoàn thành](#47-tiêu-chí-hoàn-thành)
48. [Cấu trúc repository đề xuất](#48-cấu-trúc-repository-đề-xuất)
49. [Tài liệu tham khảo](#49-tài-liệu-tham-khảo)
50. [Tổng kết](#50-tổng-kết)

---

# 1. Mục tiêu học tập

Sau khi hoàn thành chủ đề này, người học cần có khả năng:

- Giải thích được Active Object trong firmware hướng sự kiện.
- Hiểu quan hệ giữa:
  - Active Object.
  - Mailbox.
  - Event Handler.
  - State Machine.
  - Scheduler.
  - Event Pool.
- Thiết kế mailbox dạng FIFO hoặc priority queue.
- Viết Event Handler theo nguyên tắc run-to-completion.
- Phân biệt:
  - Flat State Machine.
  - Hierarchical State Machine.
  - Table-Driven State Machine.
  - Function-Based State Machine.
- Lựa chọn loại State Machine phù hợp với độ phức tạp của sản phẩm.
- Hiểu vai trò của Event Pool trong quản lý bộ nhớ.
- Quản lý ownership và reference count của event.
- Thiết kế Static Memory Pool.
- Tính toán kích thước Event Pool.
- Theo dõi high-water mark của pool.
- Gửi event:
  - Trực tiếp tới một Active Object.
  - Broadcast.
  - Publish-Subscribe.
- Thiết kế Data-Link Layer để truyền event qua:
  - UART.
  - SPI.
  - CAN.
  - TCP/UDP.
  - Shared Memory.
- Serialize và deserialize event.
- Thiết kế frame có:
  - Source.
  - Destination.
  - Signal.
  - Payload length.
  - Sequence number.
  - CRC.
- Xây dựng hệ thống Event-Driven phân tán trên nhiều MCU hoặc nhiều process.
- Test được mailbox, event pool, state machine và data-link layer.

---

# 2. Tổng quan kiến trúc Event-Driven

Một hệ thống Event-Driven hoàn chỉnh có thể được biểu diễn như sau:

```text
+--------------------+
| Hardware / Driver  |
| GPIO, UART, CAN... |
+---------+----------+
          |
          | create event
          v
+--------------------+
| Event Pool         |
| allocate event     |
+---------+----------+
          |
          | post
          v
+--------------------+
| Active Object      |
| + Mailbox          |
| + Event Handler    |
| + State Machine    |
+---------+----------+
          |
          | publish / route / serialize
          v
+--------------------+
| Data-Link Layer    |
| UART / CAN / IPC   |
+---------+----------+
          |
          v
+--------------------+
| Remote Active      |
| Object / Node      |
+--------------------+
```

Trong một MCU đơn lẻ:

```text
ISR / Timer / Driver
        |
        v
Event Pool
        |
        v
Mailbox
        |
        v
Event Handler
        |
        v
State Machine
```

Trong hệ thống nhiều node:

```text
Local Event
    |
    v
Router
    |
    +--> Local Mailbox
    |
    +--> Data-Link Layer
              |
              v
         Remote Node
              |
              v
         Remote Mailbox
```

---

# 3. Active Object là gì?

Active Object là một đối tượng hoạt động độc lập trong hệ thống Event-Driven.

Mỗi Active Object thường có:

- Một danh tính riêng.
- Một mailbox riêng.
- Một Event Handler riêng.
- Một State Machine riêng.
- Dữ liệu nội bộ riêng.
- Quyền sở hữu một hoặc nhiều resource.
- Priority hoặc scheduling policy riêng.

Ví dụ:

```text
Button Active Object
Display Active Object
Game Active Object
Communication Active Object
Storage Active Object
Diagnostic Active Object
```

Một Active Object không cần là một thread độc lập.

Trong bare-metal Event-Driven Framework, Active Object có thể chỉ là:

```c
typedef struct
{
    uint8_t id;
    uint8_t priority;
    event_queue_t mailbox;
    event_handler_t handler;
    void *state_object;
} active_object_t;
```

## Đặc trưng chính

### 1. Encapsulation

State nội bộ được giữ private.

```c
typedef struct
{
    uint8_t current_page;
    bool backlight_on;
    uint16_t brightness;
} display_context_t;
```

Task khác không được sửa trực tiếp:

```c
display_context.current_page = 2U;
```

Thay vào đó:

```c
post_event(DISPLAY_CHANGE_PAGE_SIG);
```

### 2. Asynchronous communication

Active Object không cần gọi trực tiếp nhau.

```text
Game AO
   |
   | DISPLAY_FRAME_READY
   v
Display AO
```

### 3. Single ownership

Mỗi resource nên có một owner.

```text
OLED     -> Display AO
UART TX  -> Communication AO
Flash    -> Storage AO
Game map -> Game AO
```

### 4. Run-to-completion

Mỗi event được xử lý hoàn tất trước khi Active Object nhận event tiếp theo.

---

# 4. Cấu trúc bên trong Active Object

Một cấu trúc tổng quát:

```c
typedef struct active_object active_object_t;

typedef void (*active_handler_t)(
    active_object_t *me,
    const event_t *event
);

struct active_object
{
    uint8_t id;
    uint8_t priority;

    event_queue_t mailbox;

    active_handler_t handler;

    void *context;
};
```

Ví dụ Display Active Object:

```c
typedef struct
{
    active_object_t super;

    uint8_t current_screen;
    uint8_t brightness;
    bool dirty;
} display_ao_t;
```

Khởi tạo:

```c
void display_ao_init(display_ao_t *me)
{
    active_object_init(
        &me->super,
        AO_DISPLAY_ID,
        AO_PRIORITY_LOW,
        display_event_handler
    );

    me->current_screen = SCREEN_HOME;
    me->brightness = 80U;
    me->dirty = true;
}
```

Event Handler:

```c
void display_event_handler(
    active_object_t *base,
    const event_t *event
)
{
    display_ao_t *me = (display_ao_t *)base;

    switch (event->signal)
    {
        case DISPLAY_SET_SCREEN_SIG:
        {
            me->current_screen =
                (uint8_t)event->parameter;

            me->dirty = true;
            break;
        }

        case DISPLAY_REFRESH_SIG:
        {
            if (me->dirty)
            {
                display_render(me);
                me->dirty = false;
            }
            break;
        }

        default:
        {
            break;
        }
    }
}
```

---

# 5. Mailbox

Mailbox là nơi lưu event đang chờ Active Object xử lý.

```text
Producer A ----+
               |
Producer B ----+--> Mailbox --> Active Object
               |
Timer ---------+
```

Một mailbox thường là queue FIFO:

```text
Head                                  Tail
  |                                     |
  v                                     v
+------+--------+--------+--------+--------+
| E5   | E1     | E2     | E3     | E4     |
+------+--------+--------+--------+--------+
```

## Thuộc tính quan trọng

- Capacity.
- FIFO hoặc priority.
- Producer context.
- Consumer context.
- Overflow policy.
- High-water mark.
- Critical-section policy.

## Cấu trúc mailbox

```c
#define MAILBOX_CAPACITY (16U)

typedef struct
{
    event_t *buffer[MAILBOX_CAPACITY];

    uint16_t head;
    uint16_t tail;
    uint16_t count;

    uint16_t max_count;
    uint32_t overflow_count;
} mailbox_t;
```

## Khởi tạo

```c
void mailbox_init(mailbox_t *mailbox)
{
    if (mailbox == NULL)
    {
        return;
    }

    mailbox->head = 0U;
    mailbox->tail = 0U;
    mailbox->count = 0U;
    mailbox->max_count = 0U;
    mailbox->overflow_count = 0U;
}
```

## Post

```c
bool mailbox_post(
    mailbox_t *mailbox,
    event_t *event
)
{
    if ((mailbox == NULL) || (event == NULL))
    {
        return false;
    }

    if (mailbox->count >= MAILBOX_CAPACITY)
    {
        mailbox->overflow_count++;
        return false;
    }

    mailbox->buffer[mailbox->head] = event;

    mailbox->head =
        (uint16_t)((mailbox->head + 1U) %
                   MAILBOX_CAPACITY);

    mailbox->count++;

    if (mailbox->count > mailbox->max_count)
    {
        mailbox->max_count = mailbox->count;
    }

    return true;
}
```

## Get

```c
event_t *mailbox_get(mailbox_t *mailbox)
{
    event_t *event;

    if ((mailbox == NULL) ||
        (mailbox->count == 0U))
    {
        return NULL;
    }

    event = mailbox->buffer[mailbox->tail];

    mailbox->tail =
        (uint16_t)((mailbox->tail + 1U) %
                   MAILBOX_CAPACITY);

    mailbox->count--;

    return event;
}
```

## Overflow policy

Có thể chọn:

### Drop newest

Event mới bị bỏ.

```text
Queue full + New event -> Drop new event
```

Phù hợp với event không quan trọng hoặc có thể được tạo lại.

### Drop oldest

Bỏ event cũ nhất để giữ event mới.

```text
Queue full + New event -> Remove oldest -> Add newest
```

Phù hợp dữ liệu realtime chỉ cần giá trị mới nhất.

### Fatal

Dùng nếu mất event có thể gây lỗi nghiêm trọng.

Ví dụ:

- Safety event.
- Stop request.
- Critical diagnostic event.

### Coalescing

Không thêm event nếu event tương tự đã pending.

Ví dụ:

```text
DISPLAY_REFRESH_SIG
SENSOR_UPDATE_SIG
UI_INVALIDATE_SIG
```

---

# 6. Event Handler

Event Handler là hàm xử lý một event.

```c
typedef void (*event_handler_t)(
    active_object_t *me,
    const event_t *event
);
```

## Yêu cầu

Event Handler phải:

- Không blocking.
- Không dùng delay dài.
- Không chờ busy-loop.
- Không chạy vòng lặp vô hạn.
- Không giữ interrupt disabled lâu.
- Không truy cập resource không thuộc ownership.
- Kết thúc trong thời gian hữu hạn.
- Có thể post event khác.
- Có thể thay đổi state nội bộ.
- Có thể arm hoặc cancel timer.

## Ví dụ tốt

```c
void motor_handler(
    active_object_t *base,
    const event_t *event
)
{
    motor_ao_t *me = (motor_ao_t *)base;

    switch (event->signal)
    {
        case MOTOR_START_SIG:
        {
            motor_hw_start();
            me->running = true;

            timer_start(
                AO_MOTOR_ID,
                MOTOR_TIMEOUT_SIG,
                5000U
            );
            break;
        }

        case MOTOR_STOP_SIG:
        {
            motor_hw_stop();
            me->running = false;
            break;
        }

        default:
        {
            break;
        }
    }
}
```

## Ví dụ không tốt

```c
void motor_handler(
    active_object_t *base,
    const event_t *event
)
{
    motor_hw_start();

    while (!motor_reached_target())
    {
    }

    motor_hw_stop();
}
```

Handler trên block toàn bộ scheduler.

## Cách sửa

```text
MOTOR_START_SIG
      |
      v
Start motor
      |
      v
Return

Hardware / Timer
      |
      v
MOTOR_TARGET_REACHED_SIG
      |
      v
Stop motor
```

---

# 7. Run-to-Completion

Run-to-completion có nghĩa:

```text
Dispatch event
    |
    v
Handler starts
    |
    v
Handler processes event
    |
    v
Handler returns
    |
    v
Next event may be dispatched
```

Không có context switch giữa handler nếu framework single-threaded cooperative.

## Ưu điểm

- Dễ reasoning.
- Không cần mutex giữa các Active Object cùng scheduler.
- State nội bộ không bị thay đổi giữa chừng.
- Tiết kiệm stack.
- Dễ trace.

## Nhược điểm

- Một handler dài làm trễ toàn hệ thống.
- Latency phụ thuộc worst-case execution time.
- Không phù hợp công việc blocking.
- Cần thiết kế theo nhiều bước event nhỏ.

## Nguyên tắc chia việc

Không nên:

```c
process_entire_file();
```

Nên:

```text
FILE_OPEN_SIG
FILE_READ_CHUNK_SIG
FILE_PROCESS_CHUNK_SIG
FILE_COMPLETE_SIG
```

Mỗi event xử lý một phần nhỏ.

---

# 8. Scheduler và Dispatcher

Scheduler quyết định Active Object nào được chạy.

Dispatcher đưa event tới đúng handler.

## Cooperative scheduler

```c
void scheduler_run_once(void)
{
    active_object_t *ao =
        scheduler_select_ready_ao();

    if (ao == NULL)
    {
        platform_idle();
        return;
    }

    event_t *event =
        mailbox_get(&ao->mailbox);

    if (event == NULL)
    {
        return;
    }

    ao->handler(ao, event);

    event_release(event);
}
```

## Priority-based scheduling

```text
Priority 3: Communication AO
Priority 2: Control AO
Priority 1: Display AO
Priority 0: Debug AO
```

Scheduler chọn mailbox không rỗng có priority cao nhất.

## Round-robin cùng priority

Nếu nhiều Active Object cùng priority:

```text
AO_A -> AO_B -> AO_C -> AO_A
```

## Starvation

Nếu AO priority cao luôn có event:

```text
High-priority queue never empty
        |
        v
Low-priority AO never runs
```

Giải pháp:

- Giảm tần suất event.
- Coalesce event.
- Giới hạn số event xử lý mỗi vòng.
- Priority aging.
- Chia priority hợp lý.
- Không để handler priority cao post liên tục vào chính nó.

---

# 9. State Machine trong hệ thống Event-Driven

State Machine là trung tâm của logic sản phẩm.

Công thức:

```text
Current State + Event
    =
Action + Next State
```

Ví dụ cửa tự động:

```text
CLOSED + OPEN_REQUEST -> OPENING
OPENING + LIMIT_OPEN -> OPEN
OPEN + TIMEOUT -> CLOSING
CLOSING + OBSTACLE -> OPENING
```

State Machine giúp:

- Loại bỏ nhiều biến cờ.
- Mô tả hành vi rõ ràng.
- Kiểm soát transition.
- Test logic độc lập hardware.
- Trace state dễ dàng.

---

# 10. Flat State Machine

Flat State Machine có tất cả state cùng cấp.

```text
IDLE
RUNNING
PAUSED
ERROR
```

Ví dụ:

```c
typedef enum
{
    APP_STATE_IDLE = 0,
    APP_STATE_RUNNING,
    APP_STATE_PAUSED,
    APP_STATE_ERROR
} app_state_t;
```

Handler:

```c
void app_dispatch(
    app_context_t *me,
    const event_t *event
)
{
    switch (me->state)
    {
        case APP_STATE_IDLE:
        {
            if (event->signal == START_SIG)
            {
                app_start();
                me->state = APP_STATE_RUNNING;
            }
            break;
        }

        case APP_STATE_RUNNING:
        {
            if (event->signal == PAUSE_SIG)
            {
                app_pause();
                me->state = APP_STATE_PAUSED;
            }
            else if (event->signal == ERROR_SIG)
            {
                app_stop();
                me->state = APP_STATE_ERROR;
            }
            break;
        }

        case APP_STATE_PAUSED:
        {
            if (event->signal == RESUME_SIG)
            {
                app_resume();
                me->state = APP_STATE_RUNNING;
            }
            break;
        }

        case APP_STATE_ERROR:
        {
            if (event->signal == RESET_SIG)
            {
                app_reset();
                me->state = APP_STATE_IDLE;
            }
            break;
        }

        default:
        {
            me->state = APP_STATE_ERROR;
            break;
        }
    }
}
```

## Ưu điểm

- Dễ học.
- Dễ cài đặt.
- Phù hợp logic nhỏ.
- Không cần framework phức tạp.

## Nhược điểm

- Lặp code giữa các state.
- `switch` lớn.
- Khó quản lý khi số state tăng.
- Không có state cha để chia sẻ hành vi.

---

# 11. Hierarchical State Machine

Hierarchical State Machine có state cha và state con.

```text
SYSTEM
├── OFF
└── ON
    ├── IDLE
    ├── RUNNING
    └── PAUSED
```

Event chung của `ON` có thể được xử lý tại state cha.

Ví dụ:

```text
OFF
ON
 ├── IDLE
 ├── ACTIVE
 └── ERROR
```

`POWER_OFF_SIG` được xử lý ở state `ON`, không cần lặp trong từng state con.

## Luồng xử lý event

```text
Current leaf state
      |
      v
Try child handler
      |
      +--> handled -> stop
      |
      +--> not handled
              |
              v
         Try parent
```

## Ưu điểm

- Giảm code lặp.
- Mô hình hóa sản phẩm phức tạp.
- Dễ thêm state con.
- Có entry/exit theo cây trạng thái.
- Phù hợp UI, protocol, game, communication stack.

## Nhược điểm

- Khó học hơn.
- Transition phức tạp.
- Cần framework hoặc quy tắc rõ.
- Debug khó hơn nếu không có trace state.

## Ví dụ khái niệm

```c
typedef enum
{
    HSM_HANDLED,
    HSM_SUPER,
    HSM_TRANSITION
} hsm_result_t;
```

State con:

```c
hsm_result_t state_running(
    app_hsm_t *me,
    const event_t *event
)
{
    switch (event->signal)
    {
        case PAUSE_SIG:
        {
            hsm_transition(me, state_paused);
            return HSM_TRANSITION;
        }

        default:
        {
            return hsm_super(me, state_on);
        }
    }
}
```

State cha:

```c
hsm_result_t state_on(
    app_hsm_t *me,
    const event_t *event
)
{
    switch (event->signal)
    {
        case POWER_OFF_SIG:
        {
            hsm_transition(me, state_off);
            return HSM_TRANSITION;
        }

        default:
        {
            return hsm_super(me, state_top);
        }
    }
}
```

---

# 12. Table-Driven State Machine

Table-Driven State Machine mô tả transition bằng bảng dữ liệu.

```c
typedef struct
{
    uint8_t current_state;
    uint16_t signal;
    uint8_t next_state;
    transition_action_t action;
} transition_t;
```

Bảng:

```c
static const transition_t transitions[] =
{
    {
        STATE_IDLE,
        START_SIG,
        STATE_RUNNING,
        action_start
    },
    {
        STATE_RUNNING,
        PAUSE_SIG,
        STATE_PAUSED,
        action_pause
    },
    {
        STATE_PAUSED,
        RESUME_SIG,
        STATE_RUNNING,
        action_resume
    },
    {
        STATE_RUNNING,
        ERROR_SIG,
        STATE_ERROR,
        action_error
    }
};
```

Dispatcher:

```c
bool table_sm_dispatch(
    table_sm_t *me,
    const event_t *event
)
{
    for (size_t i = 0U;
         i < ARRAY_SIZE(transitions);
         i++)
    {
        const transition_t *transition =
            &transitions[i];

        if ((transition->current_state == me->state) &&
            (transition->signal == event->signal))
        {
            if (transition->action != NULL)
            {
                transition->action(me, event);
            }

            me->state = transition->next_state;
            return true;
        }
    }

    return false;
}
```

## Ưu điểm

- Dễ review.
- Gần với transition table trong tài liệu.
- Có thể generate từ công cụ.
- Dễ kiểm tra coverage.
- Phù hợp protocol.

## Nhược điểm

- Action phức tạp vẫn cần code riêng.
- Tìm kiếm tuyến tính có thể chậm nếu bảng lớn.
- Khó biểu diễn hierarchy.
- Entry/exit cần cơ chế bổ sung.

---

# 13. State Pattern và Function-Based State Machine

Function-Based State Machine dùng con trỏ hàm để biểu diễn state.

```c
typedef void (*state_fn_t)(
    void *context,
    const event_t *event
);

typedef struct
{
    state_fn_t state;
} fsm_t;
```

Transition:

```c
void fsm_transition(
    fsm_t *me,
    state_fn_t next_state
)
{
    me->state = next_state;
}
```

Dispatch:

```c
void fsm_dispatch(
    fsm_t *me,
    void *context,
    const event_t *event
)
{
    me->state(context, event);
}
```

Ví dụ:

```c
static void led_state_off(
    void *context,
    const event_t *event
);

static void led_state_on(
    void *context,
    const event_t *event
);
```

## Ưu điểm

- Không cần `switch` state lớn.
- Mỗi state là một hàm riêng.
- Dễ tách file.
- Dễ mở rộng.
- Gần với State Pattern.

## Nhược điểm

- Khó xem toàn bộ transition cùng lúc.
- Con trỏ hàm cần hiểu rõ.
- Có thể khó debug hơn với người mới.
- Cần quy ước entry/exit.

---

# 14. So sánh các loại State Machine

| Loại | Ưu điểm | Nhược điểm | Phù hợp |
|---|---|---|---|
| Flat FSM | Đơn giản, dễ code | Khó mở rộng | Chức năng nhỏ |
| Function-Based FSM | Tách state rõ | Khó xem toàn cục | Module vừa |
| Table-Driven FSM | Dễ review, dễ generate | Khó hierarchy | Protocol |
| Hierarchical FSM | Tái sử dụng hành vi state cha | Phức tạp | UI, game, product logic |
| State Pattern OOP | Encapsulation tốt | Overhead và phức tạp hơn | C++ firmware |

## Quy tắc lựa chọn

Chọn Flat FSM khi:

- Dưới khoảng 5–8 state.
- Ít transition.
- Không có hành vi chung.

Chọn Function-Based FSM khi:

- Mỗi state có nhiều logic riêng.
- Muốn tách code theo state.

Chọn Table-Driven khi:

- Transition rõ ràng.
- Cần review hoặc generate.
- Protocol có nhiều event-state pair.

Chọn HSM khi:

- Có state cha/con.
- Nhiều state dùng chung event.
- UI hoặc product mode phức tạp.

---

# 15. Entry, Exit và Transition Action

Một transition chuẩn có thể gồm:

```text
Exit current state
        |
        v
Transition action
        |
        v
Enter next state
```

Ví dụ:

```text
RUNNING --STOP_SIG--> IDLE
```

Các action:

```text
Exit RUNNING:
    stop periodic timer

Transition:
    save statistics

Entry IDLE:
    turn status LED off
```

## Ví dụ code

```c
static void app_exit_running(app_context_t *me)
{
    timer_cancel(
        AO_APP_ID,
        APP_TICK_SIG
    );

    me->running = false;
}

static void app_enter_idle(app_context_t *me)
{
    me->current_job = JOB_NONE;
}

static void app_transition_to_idle(
    app_context_t *me
)
{
    app_exit_running(me);
    app_enter_idle(me);

    me->state = APP_STATE_IDLE;
}
```

## Lỗi thường gặp

- Quên cancel timer khi exit.
- Không reset dữ liệu state cũ.
- Entry action post event tạo vòng lặp.
- Transition chéo quá nhiều.
- Action có blocking operation.

---

# 16. Event Pool

Event Pool là vùng bộ nhớ dùng để cấp phát event.

Thay vì:

```c
event_t *event = malloc(sizeof(event_t));
```

hệ thống sử dụng block tĩnh:

```text
+---------+---------+---------+---------+
| Event 0 | Event 1 | Event 2 | Event 3 |
+---------+---------+---------+---------+
    used      free      used      free
```

## Cấu trúc event cơ bản

```c
typedef struct event
{
    uint16_t signal;
    uint8_t source;
    uint8_t destination;

    uint8_t ref_count;
    uint8_t pool_id;

    uint16_t length;
} event_t;
```

Payload có thể nằm ngay sau header:

```text
+--------------+------------------+
| Event Header | Payload          |
+--------------+------------------+
```

---

# 17. Vì sao Event Pool là xương sống của hệ thống

Mọi giao tiếp giữa Active Object đều thông qua event.

Nếu Event Pool không hoạt động đúng:

- Không thể post event.
- Mailbox không nhận được message.
- Timer không thể tạo timeout event.
- Data-Link không thể đưa frame lên application.
- Event trace bị gián đoạn.
- Hệ thống có thể deadlock logic.
- Event quan trọng có thể bị mất.

Event Pool quyết định:

- Khả năng chịu burst.
- RAM sử dụng.
- Thời gian allocation.
- Độ ổn định.
- Khả năng trace ownership.
- Mức deterministic của hệ thống.

## So sánh heap và pool

| Tiêu chí | Heap | Static Pool |
|---|---|---|
| Fragmentation | Có thể xảy ra | Không |
| Allocation time | Không cố định | Gần như cố định |
| RAM predictability | Thấp hơn | Cao |
| Failure mode | Runtime unpredictable | Pool exhausted rõ ràng |
| Embedded real-time | Cần thận trọng | Phù hợp hơn |

---

# 18. Static Event, Common Event và Dynamic Event

## Static Event

Event tồn tại suốt chương trình.

```c
static const event_t g_system_start_event =
{
    .signal = SYSTEM_START_SIG,
    .source = AO_SYSTEM_ID,
    .destination = AO_APP_ID
};
```

Ưu điểm:

- Không cần allocation.
- Không thể cạn pool.
- Phù hợp event không payload.
- Phù hợp event bất biến.

Nhược điểm:

- Không chứa dữ liệu thay đổi.
- Cần cẩn thận nếu broadcast và có ref count.

## Common Event

Event có payload cố định.

```c
typedef struct
{
    event_t super;
    uint32_t parameter;
    uint16_t value;
} common_event_t;
```

Phù hợp:

- Sensor sample nhỏ.
- Button ID.
- Timer ID.
- Command ID.

## Dynamic Event

Event có kích thước thay đổi.

```c
event_t *event =
    event_alloc(sizeof(event_t) + payload_size);
```

Phù hợp:

- Protocol frame.
- Chuỗi.
- Packet.
- Data block lớn.

Nhược điểm:

- Pool phức tạp hơn.
- Cần nhiều block size.
- Có thể lãng phí nội bộ.
- Ownership khó hơn.

---

# 19. Reference Count và Ownership

## Ownership

Một event phải có owner rõ ràng.

Luồng đơn giản:

```text
Producer allocates event
        |
        v
Producer posts event
        |
        v
Mailbox owns event
        |
        v
Consumer handles event
        |
        v
Framework releases event
```

Sau khi post thành công, producer không được dùng tiếp event nếu contract chuyển ownership.

## Reference Count

Broadcast event tới nhiều subscriber:

```text
Publisher
  |
  +--> Subscriber A
  +--> Subscriber B
  +--> Subscriber C
```

Event có:

```c
event->ref_count = 3U;
```

Mỗi consumer xử lý xong:

```c
event_release(event);
```

Khi:

```c
ref_count == 0U
```

event được trả về pool.

## API ví dụ

```c
void event_retain(event_t *event)
{
    if (event != NULL)
    {
        event->ref_count++;
    }
}

void event_release(event_t *event)
{
    if (event == NULL)
    {
        return;
    }

    if (event->ref_count > 0U)
    {
        event->ref_count--;
    }

    if (event->ref_count == 0U)
    {
        event_pool_free(event);
    }
}
```

## Lỗi ownership phổ biến

### Use-after-free

```c
post_event(event);
event_pool_free(event);
```

### Memory leak

Consumer không release event.

### Double free

Hai nơi cùng nghĩ mình là owner cuối.

### Dangling pointer

Task lưu pointer event sau khi handler kết thúc.

---

# 20. Memory Pool

Một Memory Pool có thể gồm các block cùng kích thước.

```c
#define EVENT_POOL_BLOCK_COUNT (32U)
#define EVENT_POOL_BLOCK_SIZE  (32U)

typedef union
{
    uint8_t raw[EVENT_POOL_BLOCK_SIZE];
    max_align_t align;
} event_block_t;
```

Pool:

```c
typedef struct
{
    event_block_t blocks[EVENT_POOL_BLOCK_COUNT];
    uint8_t used[EVENT_POOL_BLOCK_COUNT];

    uint16_t current_used;
    uint16_t max_used;
    uint32_t allocation_failures;
} event_pool_t;
```

## Allocation

```c
void *event_pool_alloc(event_pool_t *pool)
{
    if (pool == NULL)
    {
        return NULL;
    }

    for (uint16_t i = 0U;
         i < EVENT_POOL_BLOCK_COUNT;
         i++)
    {
        if (pool->used[i] == 0U)
        {
            pool->used[i] = 1U;
            pool->current_used++;

            if (pool->current_used > pool->max_used)
            {
                pool->max_used =
                    pool->current_used;
            }

            return pool->blocks[i].raw;
        }
    }

    pool->allocation_failures++;
    return NULL;
}
```

## Free

```c
bool event_pool_free(
    event_pool_t *pool,
    void *block
)
{
    if ((pool == NULL) || (block == NULL))
    {
        return false;
    }

    for (uint16_t i = 0U;
         i < EVENT_POOL_BLOCK_COUNT;
         i++)
    {
        if (block == pool->blocks[i].raw)
        {
            if (pool->used[i] == 0U)
            {
                return false;
            }

            pool->used[i] = 0U;
            pool->current_used--;
            return true;
        }
    }

    return false;
}
```

## Tối ưu

Thay vì quét toàn bộ mảng, dùng free list:

```text
Free block 1 -> Free block 5 -> Free block 7
```

Allocation và free trở thành O(1).

---

# 21. Kích thước pool và High-Water Mark

Không nên chọn pool bằng cảm tính.

## Ước lượng

```text
Pool size
=
Maximum events in all mailboxes
+
Events currently being handled
+
Events waiting in Data-Link TX
+
Timer events burst
+
Safety margin
```

Ví dụ:

```text
Button mailbox      : 4
App mailbox         : 8
Display mailbox     : 4
Communication queue : 8
Timers simultaneous : 4
In-flight handlers  : 2
Safety margin       : 6
--------------------------------
Total               : 36 events
```

Chọn pool:

```text
40 hoặc 48 blocks
```

## High-Water Mark

```c
if (pool->current_used > pool->max_used)
{
    pool->max_used = pool->current_used;
}
```

Sau stress test:

```text
Pool total : 48
Pool max   : 37
Margin     : 11
```

## Cảnh báo

Pool max gần total:

```text
max_used >= 90% total
```

Có thể cần:

- Tăng pool.
- Giảm event burst.
- Tối ưu handler.
- Coalesce event.
- Tách pool.
- Giảm event lifetime.

---

# 22. Event Routing

Router quyết định event đi đâu.

```c
bool event_route(event_t *event)
{
    if (event == NULL)
    {
        return false;
    }

    if (event->destination == NODE_LOCAL)
    {
        return local_post(event);
    }

    return data_link_send(event);
}
```

Có thể route theo:

- Destination Active Object.
- Destination node.
- Signal.
- Topic.
- Interface type.
- Priority.

## Routing table

```c
typedef struct
{
    uint8_t destination_node;
    uint8_t interface_id;
} route_entry_t;
```

Ví dụ:

```text
Node 0 -> Local
Node 1 -> CAN
Node 2 -> UART
Node 3 -> Ethernet
```

---

# 23. Direct Post, Publish-Subscribe và Broadcast

## Direct Post

Một producer gửi tới một destination.

```text
Button AO -> App AO
```

```c
event_post(AO_APP_ID, event);
```

Ưu điểm:

- Rõ destination.
- Dễ kiểm soát.
- Ít overhead.

Nhược điểm:

- Producer biết consumer.
- Coupling cao hơn pub-sub.

## Publish-Subscribe

Publisher phát signal lên topic.

```text
Sensor Publisher
       |
       +--> Display Subscriber
       +--> Logger Subscriber
       +--> Control Subscriber
```

```c
event_publish(event);
```

Subscription table:

```c
subscribe(
    SENSOR_DATA_READY_SIG,
    AO_DISPLAY_ID
);

subscribe(
    SENSOR_DATA_READY_SIG,
    AO_LOGGER_ID
);
```

Ưu điểm:

- Publisher không biết subscriber.
- Dễ mở rộng.
- Phù hợp shared event.

Nhược điểm:

- Quản lý ref count.
- Event fan-out.
- Khó dự đoán tổng tải hơn.
- Có thể gây event storm.

## Broadcast

Gửi tới tất cả Active Object hoặc tất cả node.

Chỉ nên dùng cho:

- System shutdown.
- Time synchronization.
- Diagnostic mode.
- Global configuration update.

Không nên broadcast quá thường xuyên.

---

# 24. Data-Link Layer là gì?

Data-Link Layer là lớp chuyển event hoặc frame giữa các execution domain khác nhau.

Execution domain có thể là:

- MCU khác.
- Process khác.
- Core khác.
- Board khác.
- Node khác trong network.

Data-Link Layer che giấu chi tiết transport khỏi application.

Application chỉ thấy:

```c
event_post(destination, event);
```

Router quyết định:

```text
Local destination -> Local mailbox
Remote destination -> Serialize -> Data-Link
```

---

# 25. Vai trò của Data-Link Layer

Data-Link Layer chịu trách nhiệm:

- Framing.
- Addressing.
- Serialization.
- Deserialization.
- Error detection.
- Sequence number.
- Acknowledgement nếu cần.
- Retry.
- Duplicate detection.
- Timeout.
- Fragmentation.
- Reassembly.
- Interface abstraction.
- Link status.
- Statistics.

Kiến trúc:

```text
Application Event
       |
       v
Event Router
       |
       v
Serializer
       |
       v
Frame
       |
       v
Transport Driver
       |
       v
Physical Link
```

Ở phía nhận:

```text
Physical Link
       |
       v
Driver RX
       |
       v
Frame Decoder
       |
       v
CRC Check
       |
       v
Deserializer
       |
       v
Remote Event
       |
       v
Local Mailbox
```

---

# 26. Kiến trúc nhiều MCU

Ví dụ sản phẩm gồm:

```text
MCU A: Main Controller
MCU B: Display Controller
MCU C: Sensor Controller
```

Kết nối:

```text
MCU A <---CAN---> MCU B
MCU A <---UART--> MCU C
```

Mỗi MCU có Event-Driven Framework riêng.

```text
MCU A
+-------------------+
| App AO            |
| Communication AO  |
| Router            |
+---------+---------+
          |
          | CAN
          v
MCU B
+-------------------+
| Display AO        |
| Communication AO  |
| Router            |
+-------------------+
```

Application không cần biết frame CAN cụ thể.

```c
event_post_remote(
    NODE_DISPLAY,
    AO_DISPLAY_ID,
    DISPLAY_SET_PAGE_SIG,
    &page,
    sizeof(page)
);
```

---

# 27. Kiến trúc nhiều process trên MPU

Trên Linux hoặc MPU, mỗi process có thể chạy một Event-Driven Runtime.

```text
Process A
  |
  | Unix Domain Socket
  v
Process B
```

Transport có thể là:

- Unix Domain Socket.
- Shared memory.
- Pipe.
- POSIX message queue.
- TCP/UDP.
- CAN socket.
- RPMsg.

Kiến trúc:

```text
Process A
+-------------------+
| Active Objects    |
| Event Router      |
| IPC Data-Link     |
+---------+---------+
          |
          | UDS
          v
Process B
+-------------------+
| IPC Data-Link     |
| Event Router      |
| Active Objects    |
+-------------------+
```

## Lưu ý

Không truyền pointer trực tiếp giữa process.

Sai:

```c
event->data = local_pointer;
```

Đúng:

- Serialize dữ liệu.
- Copy qua IPC.
- Deserialize ở process nhận.

---

# 28. Event qua network

Khi event đi qua network, cần phân biệt:

```text
Internal Event
Wire Frame
```

Internal Event:

```c
typedef struct
{
    uint16_t signal;
    uint8_t source_ao;
    uint8_t destination_ao;
    uint16_t payload_length;
    void *payload;
} event_t;
```

Wire Frame:

```c
typedef struct
{
    uint8_t version;
    uint8_t source_node;
    uint8_t destination_node;
    uint8_t source_ao;
    uint8_t destination_ao;
    uint16_t signal;
    uint16_t sequence;
    uint16_t payload_length;
    uint16_t crc;
} frame_header_t;
```

Không nên gửi raw struct trực tiếp nếu:

- Compiler khác nhau.
- Endianness khác nhau.
- Padding khác nhau.
- Version firmware khác nhau.

---

# 29. Event Serialization

Serialization chuyển event thành byte stream.

```text
Event object
    |
    v
Serialize
    |
    v
Byte array
```

Ví dụ helper:

```c
static void write_u16_be(
    uint8_t *buffer,
    uint16_t value
)
{
    buffer[0] = (uint8_t)(value >> 8);
    buffer[1] = (uint8_t)(value & 0xFFU);
}
```

Serialize:

```c
size_t event_serialize(
    const event_t *event,
    uint8_t *buffer,
    size_t capacity
)
{
    const size_t required =
        8U + event->payload_length;

    if ((event == NULL) ||
        (buffer == NULL) ||
        (capacity < required))
    {
        return 0U;
    }

    buffer[0] = event->source;
    buffer[1] = event->destination;

    write_u16_be(&buffer[2], event->signal);
    write_u16_be(&buffer[4], event->payload_length);

    memcpy(
        &buffer[6],
        event_get_payload(event),
        event->payload_length
    );

    const uint16_t crc =
        crc16(buffer, 6U + event->payload_length);

    write_u16_be(
        &buffer[6U + event->payload_length],
        crc
    );

    return required;
}
```

## Deserialization

Phải kiểm tra:

- Minimum length.
- Version.
- Payload length.
- Maximum size.
- CRC.
- Destination.
- Signal range.
- Authentication nếu cần.

---

# 30. Frame Format

Một frame tổng quát:

```text
+---------+---------+---------+---------+
| SOF     | Version | Flags   | Length  |
+---------+---------+---------+---------+
| SrcNode | DstNode | SrcAO   | DstAO   |
+---------+---------+---------+---------+
| Signal            | Sequence          |
+---------------------------------------+
| Payload                               |
+---------------------------------------+
| CRC                                   |
+---------------------------------------+
```

## Ví dụ

| Field | Size | Mô tả |
|---|---:|---|
| SOF | 1 byte | Start of frame |
| Version | 1 byte | Protocol version |
| Flags | 1 byte | ACK, fragment, priority |
| Length | 2 bytes | Tổng payload |
| Source Node | 1 byte | Node gửi |
| Destination Node | 1 byte | Node nhận |
| Source AO | 1 byte | Active Object gửi |
| Destination AO | 1 byte | Active Object nhận |
| Signal | 2 bytes | Event signal |
| Sequence | 2 bytes | Phát hiện mất/trùng |
| Payload | N bytes | Dữ liệu |
| CRC | 2 bytes | Error detection |

## Versioning

Khi thay đổi protocol:

- Không đổi nghĩa field cũ tùy tiện.
- Tăng version.
- Hỗ trợ backward compatibility nếu cần.
- Có length để bỏ qua field chưa biết.

---

# 31. Addressing và Routing giữa các node

Một địa chỉ có thể gồm:

```text
Node ID + Active Object ID
```

Ví dụ:

```text
0x01: Main Controller
0x02: Display Controller
0x03: Sensor Controller
```

Active Object:

```text
0x10: Application
0x11: Display
0x12: Sensor
0x13: Diagnostic
```

Địa chỉ logic:

```text
Node 0x02 / AO 0x11
```

## Routing table

```c
typedef struct
{
    uint8_t destination_node;
    uint8_t link_id;
} route_t;
```

Ví dụ:

```text
Node 0x01 -> LOCAL
Node 0x02 -> CAN_1
Node 0x03 -> UART_2
```

## Broadcast address

```text
Node 0xFF -> all nodes
AO   0xFF -> all Active Objects
```

Broadcast phải được giới hạn và kiểm soát.

---

# 32. Error Detection và Reliability

## CRC

Dùng để phát hiện frame lỗi.

```text
TX:
payload -> CRC -> send

RX:
receive -> compute CRC -> compare
```

## Sequence Number

Phát hiện:

- Frame bị mất.
- Frame bị lặp.
- Frame sai thứ tự.

```text
Expected sequence: 101
Received sequence: 103
-> frame 102 có thể đã mất
```

## ACK/NACK

Dùng khi cần reliability.

```text
Sender -> DATA(seq=10)
Receiver -> ACK(seq=10)
```

Nếu timeout:

```text
Retry
```

## Retry policy

Cần giới hạn:

```text
Maximum retry = 3
```

Không retry vô hạn.

## Duplicate detection

Nếu sender retry vì ACK mất, receiver có thể nhận frame trùng.

Cần:

- Sequence cache.
- Idempotent command.
- Duplicate suppression.

## Timeout

Mỗi transaction cần timeout.

Không nên chờ vô hạn.

---

# 33. Ví dụ Event qua UART

## TX flow

```text
Application AO
      |
      v
Remote event
      |
      v
Serialize
      |
      v
UART TX queue
      |
      v
DMA / Interrupt TX
```

## RX flow

```text
UART RX ISR
      |
      v
Ring Buffer
      |
      v
Communication AO
      |
      v
Frame Parser
      |
      v
CRC check
      |
      v
Deserialize Event
      |
      v
Local Mailbox
```

## Parser state machine

```text
WAIT_SOF
READ_HEADER
READ_PAYLOAD
READ_CRC
VALIDATE
```

Ví dụ state enum:

```c
typedef enum
{
    RX_WAIT_SOF = 0,
    RX_READ_HEADER,
    RX_READ_PAYLOAD,
    RX_READ_CRC
} rx_state_t;
```

## Không parse trong ISR

ISR chỉ:

```c
void UART_IRQHandler(void)
{
    const uint8_t byte = uart_read_byte();

    ring_buffer_write(
        &g_uart_rx_buffer,
        byte
    );

    event_post_static(
        AO_COMM_ID,
        &g_uart_rx_available_event
    );
}
```

---

# 34. Ví dụ Event qua CAN

CAN có payload nhỏ nên cần mapping.

## Cách 1: Một signal tương ứng một CAN ID

```text
CAN ID 0x101 -> BUTTON_EVENT
CAN ID 0x201 -> DISPLAY_COMMAND
CAN ID 0x301 -> SENSOR_DATA
```

Ưu điểm:

- Nhanh.
- Dễ filter phần cứng.
- Ít overhead.

Nhược điểm:

- Khó mở rộng.
- Số CAN ID tăng.
- Mapping cứng.

## Cách 2: CAN ID theo node, payload chứa signal

```text
CAN ID = destination node
Payload:
[signal][source][length][data...]
```

## Fragmentation

Nếu payload event lớn hơn CAN frame:

```text
Event payload
    |
    v
Fragment 0
Fragment 1
Fragment 2
```

Cần:

- Message ID.
- Fragment index.
- Total fragments.
- Timeout.
- Reassembly buffer.
- CRC toàn message.

## CAN FD

CAN FD cho payload lớn hơn classic CAN, nhưng vẫn cần protocol rõ ràng.

---

# 35. Ví dụ hệ thống nhiều MCU

## Yêu cầu

- MCU A đọc button.
- MCU A gửi `BUTTON_PRESSED_SIG`.
- MCU B điều khiển display.
- MCU C ghi log.

## Kiến trúc

```text
MCU A
+------------------+
| Button AO        |
| Router           |
| CAN Data-Link    |
+--------+---------+
         |
         | CAN
         v
MCU B
+------------------+
| CAN Data-Link    |
| Display AO       |
+------------------+

MCU C
+------------------+
| CAN Data-Link    |
| Logger AO        |
+------------------+
```

## Publish-Subscribe phân tán

`BUTTON_PRESSED_SIG` có subscriber:

```text
MCU B / Display AO
MCU C / Logger AO
```

Router MCU A:

```text
Local subscribers  -> local post
Remote subscribers -> serialize + CAN send
```

## Sequence

```text
Button IRQ on MCU A
        |
        v
Button AO
        |
        v
Publish BUTTON_PRESSED
        |
        +--> CAN frame to MCU B
        |
        +--> CAN frame to MCU C
```

MCU B:

```text
CAN RX
  |
  v
Deserialize
  |
  v
Display AO mailbox
  |
  v
Update screen
```

---

# 36. Sequence Diagram tổng thể

```text
Button ISR   Button AO   Event Pool   Router   Data-Link   Remote AO
    |            |           |          |          |           |
    | IRQ        |           |          |          |           |
    |----------->|           |          |          |           |
    |            | alloc     |          |          |           |
    |            |---------->|          |          |           |
    |            | event     |          |          |           |
    |            |<----------|          |          |           |
    |            | post                 |          |           |
    |            |--------------------->|          |           |
    |            |                      | remote   |           |
    |            |                      |--------->|           |
    |            |                      |          | serialize |
    |            |                      |          |---------->|
    |            |                      |          | transport |
    |            |                      |          |==========>|
    |            |                      |          |           |
    |            |                      |          |           | handle
```

---

# 37. Quản lý concurrency

Trong single-thread cooperative Event-Driven:

- Chỉ một handler chạy tại một thời điểm.
- State Active Object không cần mutex nếu chỉ được handler của nó truy cập.
- ISR vẫn có thể chạy bất kỳ lúc nào.
- Data-Link DMA có thể cập nhật buffer.

## Shared data giữa ISR và AO

Dùng:

- Ring buffer.
- Atomic index.
- Critical section ngắn.
- Single-producer single-consumer queue.

## Không chia sẻ state trực tiếp giữa AO

Sai:

```c
display_ao.current_page =
    game_ao.requested_page;
```

Đúng:

```c
event_post(
    AO_DISPLAY_ID,
    DISPLAY_SET_PAGE_SIG
);
```

## Multi-threaded runtime

Nếu nhiều scheduler thread chạy song song:

- Mỗi Active Object vẫn nên chỉ có một consumer.
- Mailbox cần thread-safe.
- Shared pool cần lock hoặc atomic.
- Ordering phải được định nghĩa.
- State Machine không được dispatch đồng thời.

---

# 38. Interrupt và Active Object

ISR là event source, không phải Active Object.

```text
Interrupt
   |
   v
Capture minimum data
   |
   v
Post event
   |
   v
Active Object handles later
```

## Ví dụ ADC DMA

```c
void DMA_IRQHandler(void)
{
    dma_clear_irq();

    adc_buffer_t *buffer =
        adc_dma_get_completed_buffer();

    event_t *event =
        adc_event_alloc();

    if (event != NULL)
    {
        event->signal = ADC_BUFFER_READY_SIG;
        event->destination = AO_SENSOR_ID;

        adc_event_set_buffer(event, buffer);

        event_post(event);
    }
}
```

## Lưu ý buffer ownership

Nếu DMA tiếp tục ghi vào buffer, consumer không được đọc buffer đang bị overwrite.

Giải pháp:

- Double buffer.
- Ping-pong buffer.
- Buffer pool.
- Ownership transfer.

---

# 39. Quản lý lỗi

Các lỗi chính:

- Mailbox overflow.
- Event Pool exhausted.
- Invalid destination.
- Invalid signal.
- CRC error.
- Frame timeout.
- Sequence mismatch.
- Deserialization failure.
- State transition invalid.
- Reference count underflow.
- Double free.
- Link down.

## Error event

```c
typedef struct
{
    event_t super;
    uint16_t error_code;
    uint16_t detail;
} error_event_t;
```

## Error Manager Active Object

```text
All modules
    |
    | ERROR_REPORTED_SIG
    v
Error Manager AO
    |
    +--> log
    +--> retry
    +--> safe state
    +--> reset
```

## Không dùng fatal cho mọi lỗi

Recoverable:

- CRC sai.
- Packet timeout.
- Sensor sample lỗi.
- User command sai.

Fatal:

- Pool metadata corrupt.
- Reference count sai nghiêm trọng.
- Impossible state.
- Safety invariant bị phá vỡ.

---

# 40. Logging và Event Trace

Mỗi event có thể được trace:

```text
timestamp
source
destination
signal
state_before
state_after
queue_depth
```

Record:

```c
typedef struct
{
    uint32_t timestamp;
    uint16_t signal;
    uint8_t source;
    uint8_t destination;
    uint8_t state_before;
    uint8_t state_after;
    uint8_t queue_depth;
} event_trace_record_t;
```

## Trace points

- Event allocated.
- Event posted.
- Event enqueued.
- Event dequeued.
- Handler started.
- State transition.
- Handler completed.
- Event released.
- Frame TX.
- Frame RX.
- CRC failure.
- Queue overflow.

## Lợi ích

- Phân tích latency.
- Phát hiện event storm.
- Tìm lost event.
- Xem transition sai.
- Điều tra lỗi ngoài hiện trường.
- Tính high-water mark.

---

# 41. Thiết kế module

Một module Active Object nên có:

```text
module.h
module.c
module_config.h
module_events.h
module_private.h
```

Ví dụ:

```text
display/
├── display_ao.h
├── display_ao.c
├── display_events.h
├── display_config.h
└── display_driver_port.h
```

## Public API

```c
void display_ao_init(void);
active_object_t *display_ao_get(void);
```

## Không expose state private

Không nên:

```c
extern display_context_t g_display_context;
```

Nên cung cấp event hoặc query API rõ ràng.

---

# 42. Quy trình phát triển

## Bước 1 — Xác định Active Object

Ví dụ:

```text
Input AO
Application AO
Display AO
Communication AO
Storage AO
Diagnostic AO
```

## Bước 2 — Xác định ownership

| Resource | Owner |
|---|---|
| Buttons | Input AO |
| Product state | Application AO |
| OLED | Display AO |
| UART/CAN TX | Communication AO |
| Flash | Storage AO |

## Bước 3 — Xác định event

Tạo `event-list.md`.

| Signal | Source | Destination | Payload |
|---|---|---|---|
| BUTTON_PRESSED | Input | App | Button ID |
| DISPLAY_SET_PAGE | App | Display | Page ID |
| STORE_CONFIG | App | Storage | Config |
| LINK_FRAME_RX | Driver | Comm | Buffer handle |

## Bước 4 — Thiết kế mailbox

Ghi rõ:

- Capacity.
- Priority.
- Overflow policy.
- Producer context.
- Consumer context.

## Bước 5 — Thiết kế State Machine

Vẽ state diagram trước khi code.

## Bước 6 — Thiết kế Event Pool

- Loại block.
- Kích thước.
- Số block.
- High-water mark.
- Failure policy.

## Bước 7 — Thiết kế router

- Local destination.
- Remote destination.
- Publish-subscribe.
- Broadcast.
- Route table.

## Bước 8 — Thiết kế Data-Link

- Frame format.
- CRC.
- Sequence.
- Timeout.
- Retry.
- Fragmentation.
- Versioning.

## Bước 9 — Viết test

- Queue.
- State Machine.
- Pool.
- Serialization.
- CRC.
- Retry.
- Fragmentation.

## Bước 10 — Stress test

- Event burst.
- Link loss.
- Packet duplication.
- Pool exhaustion.
- Queue overflow.
- Timer burst.

---

# 43. Best Practices

Các nguyên tắc nên áp dụng:

## Active Object sở hữu state của chính nó

Không sửa state từ module khác.

## Mailbox phải có metrics

Theo dõi:

```text
current depth
maximum depth
overflow count
```

## Event Handler phải ngắn

Không block.

## Event phải nhỏ

Truyền handle với payload lớn.

## Ownership phải rõ

Ghi trong tài liệu:

```text
Producer owns before post.
Framework owns after successful post.
Consumer may not retain after handler unless retained.
```

## State Machine phải được vẽ

Không code state bằng các biến cờ rời rạc.

## Timer phải gắn với state

Cancel timer khi exit.

## Pool phải được đo

Không chỉ tăng kích thước theo cảm tính.

## Data-Link không expose transport lên application

Application không nên biết CAN ID hoặc UART framing.

## Wire protocol phải versioned

Không gửi raw struct.

## Trace phải có timestamp

Để đo latency.

## Retry phải giới hạn

Không retry vô hạn.

## Broadcast phải được hạn chế

Tránh event storm.

---

# 44. Anti-pattern và lỗi thường gặp

Các anti-pattern và lỗi sau cần được tránh:

## God Active Object

Một AO xử lý mọi thứ:

```text
App AO:
button
display
sensor
storage
communication
audio
```

Hậu quả:

- Mailbox lớn.
- Handler lớn.
- State phức tạp.
- Không rõ ownership.

## Too many Active Objects

Mỗi hàm thành một AO.

Hậu quả:

- Quá nhiều queue.
- Tốn RAM.
- Routing phức tạp.
- Event overhead cao.

## Shared mutable global state

Nhiều AO sửa cùng một biến.

## Handler gọi trực tiếp handler khác

```c
display_handler(...);
```

Nên post event.

## Event chứa pointer không hợp lệ

Pointer tới stack hoặc buffer DMA đang bị reuse.

## Không xử lý pool exhaustion

Allocation trả `NULL` nhưng code vẫn dùng.

## Không kiểm tra CRC

Frame lỗi được chuyển thành event hợp lệ.

## Gửi raw struct qua network

Dính padding và endianness.

## Retry vô hạn

Link down làm hệ thống bận hoàn toàn.

## Timer cũ tác động state mới

Không cancel timer khi transition.

## Broadcast event quá nhiều

Mỗi subscriber tạo thêm event mới.

---

# 45. Kiểm thử

Các nội dung cần kiểm thử:

## Test Mailbox FIFO

- Post E1, E2, E3.
- Get đúng E1, E2, E3.

## Test Wraparound

- Fill một phần.
- Get một phần.
- Post thêm.
- Kiểm tra head/tail.

## Test Overflow

- Fill queue.
- Post thêm.
- Kiểm tra policy.

## Test Active Object

- Post event.
- Run scheduler.
- Kiểm tra handler và state.

## Test State Machine

- Transition hợp lệ.
- Event không hợp lệ.
- Entry/exit.
- Timer cancel.

## Test Event Pool

- Allocate toàn bộ.
- Allocation tiếp theo thất bại.
- Free và allocate lại.
- Double free.
- Invalid pointer.
- High-water mark.

## Test Reference Count

- Publish tới nhiều subscriber.
- Release từng subscriber.
- Event chỉ free ở ref count 0.

## Test Serialization

- Encode/decode.
- Endianness.
- Maximum payload.
- Invalid length.
- CRC sai.
- Version sai.

## Test Reliability

- Drop ACK.
- Duplicate frame.
- Sequence gap.
- Retry limit.
- Timeout.

## Integration Test

```text
Local AO
-> Router
-> Data-Link TX
-> Loopback
-> Data-Link RX
-> Remote AO
```

---

# 46. Bài thực hành

Các bài thực hành được sắp xếp từ cơ bản đến tích hợp:

## Bài 1 — Active Object cơ bản

Tạo:

```text
Button AO
LED AO
```

Button AO gửi event tới LED AO.

## Bài 2 — Mailbox riêng

Mỗi AO có mailbox capacity khác nhau.

Theo dõi max depth.

## Bài 3 — Priority Scheduler

Tạo ba AO:

```text
High
Medium
Low
```

Kiểm tra thứ tự dispatch.

## Bài 4 — Flat FSM

Tạo state:

```text
OFF
ON
BLINKING
```

## Bài 5 — Function-Based FSM

Viết lại bài 4 bằng con trỏ hàm.

## Bài 6 — Table-Driven FSM

Tạo transition table và test coverage.

## Bài 7 — Hierarchical State Machine

Tạo:

```text
SYSTEM
├── OFF
└── ON
    ├── IDLE
    └── RUNNING
```

`POWER_OFF_SIG` xử lý ở state cha `ON`.

## Bài 8 — Static Event Pool

Viết pool O(1) bằng free list.

## Bài 9 — Pool Metrics

Thêm:

```text
current_used
max_used
allocation_failures
```

## Bài 10 — Publish-Subscribe

Sensor publish data cho:

```text
Display AO
Logger AO
Control AO
```

## Bài 11 — UART Data-Link

Thiết kế frame có:

```text
SOF
Version
Source
Destination
Signal
Length
Payload
CRC
```

## Bài 12 — Parser State Machine

Parser UART không blocking.

## Bài 13 — CAN Event Transport

Map event sang CAN frame.

Xử lý payload dài bằng fragmentation.

## Bài 14 — Multi-MCU Demo

MCU A đọc button, MCU B điều khiển LED hoặc OLED.

## Bài 15 — Link Failure

Ngắt dây truyền thông.

Kiểm tra:

- Timeout.
- Retry.
- Link state.
- Error event.

## Bài 16 — Event Trace

Log toàn bộ:

```text
post
enqueue
dispatch
transition
release
```

## Bài 17 — Stress Test

Tạo event burst từ:

- Timer.
- UART.
- Button.
- CAN.

Theo dõi pool và mailbox.

## Bài 18 — Mini Distributed Game

Node 1:

```text
Input AO
Game AO
```

Node 2:

```text
Display AO
Audio AO
```

Game event truyền qua Data-Link.

---

# 47. Tiêu chí hoàn thành

Người học được xem là hoàn thành chủ đề khi có thể:

- Giải thích được Active Object.
- Thiết kế mailbox.
- Viết Event Handler run-to-completion.
- Viết scheduler cơ bản.
- Hiểu priority và starvation.
- Viết Flat State Machine.
- Viết Function-Based State Machine.
- Viết Table-Driven State Machine.
- Giải thích Hierarchical State Machine.
- Dùng entry/exit action đúng.
- Tạo Event Pool tĩnh.
- Quản lý ownership.
- Hiểu reference count.
- Theo dõi high-water mark.
- Xử lý pool exhaustion.
- Dùng direct post.
- Dùng publish-subscribe.
- Thiết kế router.
- Thiết kế frame Data-Link.
- Serialize và deserialize event.
- Kiểm tra CRC.
- Dùng sequence number.
- Xử lý timeout và retry.
- Truyền event qua UART hoặc CAN.
- Hoàn thành demo nhiều MCU hoặc nhiều process.
- Có unit test và integration test.
- Có event trace.
- Có tài liệu kiến trúc trên GitHub.

---

# 48. Cấu trúc repository đề xuất

Cấu trúc repository đề xuất:

```text
04-event-driven-system-components/
├── README.md
├── Makefile
├── framework/
│   ├── include/
│   │   ├── active_object.h
│   │   ├── mailbox.h
│   │   ├── event.h
│   │   ├── event_pool.h
│   │   ├── scheduler.h
│   │   ├── router.h
│   │   ├── fsm.h
│   │   ├── hsm.h
│   │   └── pubsub.h
│   └── src/
│       ├── active_object.c
│       ├── mailbox.c
│       ├── event_pool.c
│       ├── scheduler.c
│       ├── router.c
│       ├── fsm.c
│       ├── hsm.c
│       └── pubsub.c
├── application/
│   ├── app_ao.c
│   ├── button_ao.c
│   ├── display_ao.c
│   ├── communication_ao.c
│   └── diagnostic_ao.c
├── datalink/
│   ├── include/
│   │   ├── datalink.h
│   │   ├── frame.h
│   │   ├── serializer.h
│   │   ├── uart_link.h
│   │   └── can_link.h
│   └── src/
│       ├── datalink.c
│       ├── frame.c
│       ├── serializer.c
│       ├── uart_link.c
│       └── can_link.c
├── platform/
│   ├── gpio.c
│   ├── uart.c
│   ├── can.c
│   ├── timer.c
│   └── critical_section.c
├── tests/
│   ├── test_mailbox.c
│   ├── test_event_pool.c
│   ├── test_scheduler.c
│   ├── test_fsm.c
│   ├── test_hsm.c
│   ├── test_pubsub.c
│   ├── test_serializer.c
│   └── test_datalink.c
└── docs/
    ├── architecture.md
    ├── active-objects.md
    ├── event-list.md
    ├── state-machines.md
    ├── pool-sizing.md
    ├── routing-table.md
    ├── wire-protocol.md
    └── sequence-diagrams.md
```

## Nội dung README của project
- Mục tiêu hệ thống.
- Board và MCU.
- Danh sách Active Object.
- Ownership của từng resource.
- Mailbox capacity.
- Priority.
- Event list.
- State diagram.
- Event Pool configuration.
- Pool high-water mark.
- Routing table.
- Frame format.
- CRC.
- Retry policy.
- Cách build.
- Cách flash.
- Cách test.
- Event trace mẫu.
- Stress-test result.
- Known limitations.

---

# 49. Tài liệu tham khảo

Các tài liệu nên tham khảo:

## Active Kernel Base Kit

- https://github.com/ak-embedded-software/ak-base-kit-stm32l151

## Các chủ đề cần đọc thêm

- Active Object Pattern.
- Finite State Machine.
- Hierarchical State Machine.
- Memory Pool.
- Publish-Subscribe.
- Producer-Consumer.
- Circular Buffer.
- Message Passing.
- Event Serialization.
- UART Framing.
- CAN Transport Protocol.
- Inter-Process Communication.
- Distributed Event-Driven Systems.

## Tài liệu MCU

- Datasheet.
- Reference Manual.
- Interrupt/NVIC documentation.
- UART/CAN peripheral documentation.
- DMA documentation.
- Memory map.
- Errata sheet.

---

# 50. Tổng kết

Các thành phần cốt lõi của một hệ thống Event-Driven hoàn chỉnh gồm:

```text
Active Object
    +
Mailbox
    +
Event Handler
    +
State Machine
    +
Event Pool
    +
Scheduler
    +
Event Router
    +
Data-Link Layer
```

Luồng xử lý cơ bản:

```text
Event Source
    |
    v
Allocate Event
    |
    v
Post to Mailbox
    |
    v
Scheduler selects Active Object
    |
    v
Event Handler runs to completion
    |
    v
State Machine handles transition
    |
    v
Event is released to Pool
```

Luồng phân tán:

```text
Local Event
    |
    v
Router
    |
    v
Serialize
    |
    v
Data-Link
    |
    v
Remote Node
    |
    v
Deserialize
    |
    v
Remote Mailbox
    |
    v
Remote Active Object
```

Các nguyên tắc quan trọng nhất:

1. Mỗi Active Object phải có responsibility rõ ràng.
2. Mỗi resource chỉ nên có một owner.
3. Mailbox phải có capacity và overflow policy.
4. Event Handler phải run-to-completion.
5. State Machine phải được thiết kế trước khi code.
6. Event Pool là tài nguyên hữu hạn.
7. Ownership và reference count phải rõ ràng.
8. Pool phải có high-water mark.
9. Không gửi raw struct qua network.
10. Data-Link phải có framing, addressing và error detection.
11. Retry và timeout phải có giới hạn.
12. Publish-Subscribe cần kiểm soát event fan-out.
13. ISR chỉ tạo dữ liệu tối thiểu và post event.
14. Trace event là công cụ debug quan trọng.
15. Kiến trúc local và distributed nên dùng cùng một mô hình event thống nhất.

Sau chủ đề này, người học cần đủ khả năng tự thiết kế một Event-Driven Framework có thể chạy trên một MCU, đồng thời mở rộng sang hệ thống nhiều MCU hoặc nhiều process mà không làm thay đổi logic nghiệp vụ ở tầng application.
