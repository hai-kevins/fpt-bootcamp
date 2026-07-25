# Chủ đề 2 - Lập trình bất đồng bộ và hướng sự kiện
## Asynchronous & Event-Driven Programming

> Mục tiêu của chủ đề này là hiểu vì sao firmware cần chuyển từ cách viết tuần tự, polling và blocking sang mô hình bất đồng bộ hướng sự kiện; đồng thời tự xây dựng được một Event-Driven Framework tối thiểu gồm Event, Event Queue, Dispatcher, Event Handler, Timer Event và State Machine.

---

## Mục lục

1. [Mục tiêu học tập](#1-mục-tiêu-học-tập)
2. [Kiến thức cần chuẩn bị](#2-kiến-thức-cần-chuẩn-bị)
3. [Vấn đề của chương trình tuần tự](#3-vấn-đề-của-chương-trình-tuần-tự)
4. [Đồng bộ và bất đồng bộ](#4-đồng-bộ-và-bất-đồng-bộ)
5. [Event-Driven Programming là gì?](#5-event-driven-programming-là-gì)
6. [Các thành phần chính](#6-các-thành-phần-chính)
7. [Event và Signal](#7-event-và-signal)
8. [Event Queue và Mailbox](#8-event-queue-và-mailbox)
9. [Dispatcher](#9-dispatcher)
10. [Event Handler](#10-event-handler)
11. [State Machine](#11-state-machine)
12. [Timer Event](#12-timer-event)
13. [Interrupt trong hệ thống Event-Driven](#13-interrupt-trong-hệ-thống-event-driven)
14. [Quản lý dữ liệu và bộ nhớ](#14-quản-lý-dữ-liệu-và-bộ-nhớ)
15. [Event-Driven Framework tối thiểu](#15-event-driven-framework-tối-thiểu)
16. [Ví dụ: Button điều khiển LED](#16-ví-dụ-button-điều-khiển-led)
17. [Ví dụ: UART nhận lệnh](#17-ví-dụ-uart-nhận-lệnh)
18. [Quy trình phát triển sản phẩm](#18-quy-trình-phát-triển-sản-phẩm)
19. [Best Practices](#19-best-practices)
20. [Anti-pattern và lỗi thường gặp](#20-anti-pattern-và-lỗi-thường-gặp)
21. [Kiểm thử](#21-kiểm-thử)
22. [Bài thực hành](#22-bài-thực-hành)
23. [Tiêu chí hoàn thành](#23-tiêu-chí-hoàn-thành)
24. [Cấu trúc repository đề xuất](#24-cấu-trúc-repository-đề-xuất)
25. [Tài liệu tham khảo](#25-tài-liệu-tham-khảo)

---

# 1. Mục tiêu học tập

Sau khi hoàn thành chủ đề này, người học cần có khả năng:

- Phân biệt synchronous, asynchronous, polling, interrupt và Event-Driven.
- Phân tích nhược điểm của chương trình dùng nhiều `delay()`, cờ trạng thái và biến toàn cục.
- Thiết kế được Event, Signal, Event Queue, Mailbox, Dispatcher và Event Handler.
- Xây dựng vòng lặp sự kiện không blocking.
- Chuyển phần xử lý nặng ra khỏi ISR bằng deferred processing.
- Xây dựng circular queue bằng bộ nhớ tĩnh.
- Phát hiện và xử lý queue overflow.
- Mô hình hóa một chức năng bằng State Machine.
- Thiết kế Timer Event thay cho blocking delay.
- Tổ chức project Event-Driven có khả năng mở rộng, trace và unit test.

---

# 2. Kiến thức cần chuẩn bị

Người học nên nắm:

- C cơ bản: `struct`, `enum`, con trỏ, mảng, con trỏ hàm.
- `static`, `const`, `volatile`.
- Toán tử bit.
- Circular buffer.
- Interrupt và timer cơ bản.
- Stack, heap, `.data`, `.bss`.
- Critical section.
- Producer–consumer ở mức cơ bản.

Ví dụ signal:

```c
typedef enum
{
    SIGNAL_NONE = 0,
    SIGNAL_BUTTON_PRESSED,
    SIGNAL_BUTTON_RELEASED,
    SIGNAL_TIMEOUT,
    SIGNAL_UART_RX,
} signal_t;
```

---

# 3. Vấn đề của chương trình tuần tự

## 3.1 Firmware blocking

```c
int main(void)
{
    board_init();

    while (1)
    {
        led_on();
        delay_ms(1000U);

        led_off();
        delay_ms(1000U);

        if (button_is_pressed())
        {
            process_button();
        }

        if (uart_has_data())
        {
            process_uart();
        }
    }
}
```

Các vấn đề:

- Trong lúc delay, CPU không xử lý chức năng khác.
- Button phản hồi chậm.
- UART có thể mất byte.
- Thêm tính năng làm `main()` ngày càng dài.
- Khó xác định thời gian phản hồi.
- Khó kiểm thử từng module.

## 3.2 Polling

```c
while (1)
{
    if (button_is_pressed())
    {
        process_button();
    }

    if (uart_has_data())
    {
        process_uart();
    }

    if (adc_conversion_done())
    {
        process_adc();
    }
}
```

Polling phù hợp với project rất nhỏ, nhưng khi số lượng chức năng tăng:

- CPU phải kiểm tra liên tục.
- Độ trễ phụ thuộc vị trí kiểm tra.
- Logic bị trộn trong một super-loop.
- Một hàm chạy lâu sẽ làm chậm toàn bộ hệ thống.

## 3.3 Quá nhiều biến toàn cục

```c
bool g_button_pressed;
bool g_led_blinking;
bool g_uart_ready;
uint32_t g_timeout_counter;
uint8_t g_current_mode;
```

Hậu quả:

- Không rõ module nào sở hữu dữ liệu.
- Nhiều module có thể sửa cùng một biến.
- Khó tìm nguyên nhân khi trạng thái sai.
- Dễ xuất hiện race condition.
- Unit test khó cô lập.

Không tốt:

```c
void uart_process(void)
{
    if (g_button_pressed && g_current_mode == 2U)
    {
        g_led_blinking = true;
    }
}
```

Module UART đang phụ thuộc trực tiếp vào Button, Mode và LED.

---

# 4. Đồng bộ và bất đồng bộ

## 4.1 Synchronous Programming

Trong lập trình đồng bộ, hàm gọi phải chờ công việc hoàn tất.

```c
sensor_read();
display_update();
uart_send();
```

Nếu `sensor_read()` mất 100 ms, hai hàm sau phải chờ.

```text
Call sensor_read()
        |
        v
Wait until complete
        |
        v
Return
        |
        v
Continue
```

## 4.2 Asynchronous Programming

Trong lập trình bất đồng bộ, firmware khởi động công việc rồi tiếp tục xử lý việc khác.

```c
sensor_start_conversion();
```

Khi ADC hoàn thành:

```c
post_event(SIGNAL_ADC_DONE);
```

```text
Start ADC
   |
   +-------------------------+
   |                         |
   v                         v
Continue other work      ADC complete
                             |
                             v
                         Post event
                             |
                             v
                        Handle later
```

## 4.3 Bất đồng bộ không nhất thiết là đa luồng

Một hệ thống có thể chỉ dùng một thread:

```text
Main Event Loop
+ Event Queue
+ Dispatcher
+ Non-blocking Handlers
```

Mô hình này phù hợp với MCU nhỏ vì đơn giản và tiết kiệm RAM.

---

# 5. Event-Driven Programming là gì?

Event-Driven Programming là mô hình trong đó luồng thực thi được quyết định bởi sự kiện.

Nguồn sự kiện có thể là:

- GPIO/Button.
- Timer.
- UART.
- CAN.
- ADC/DMA.
- Sensor.
- Module ứng dụng.
- Lỗi hệ thống.

Kiến trúc tổng quát:

```text
+--------------+
| Event Source |
| GPIO/UART/   |
| Timer/Sensor |
+------+-------+
       |
       | Post Event
       v
+--------------+
| Event Queue  |
| Mailbox      |
+------+-------+
       |
       | Get Event
       v
+--------------+
| Dispatcher   |
+------+-------+
       |
       | Route Event
       v
+--------------+
| Event Handler|
+------+-------+
       |
       | Action / Transition
       v
+--------------+
| State Machine|
+--------------+
```

Các nguyên tắc chính:

1. Event Source không gọi sâu vào logic ứng dụng.
2. Event được đưa vào queue.
3. Dispatcher gửi event tới đúng handler.
4. Handler xử lý nhanh rồi trả quyền điều khiển.
5. Trạng thái được quản lý trong State Machine hoặc object sở hữu riêng.

---

# 6. Các thành phần chính

```text
Event Source
    |
    v
Event
    |
    v
Event Queue / Mailbox
    |
    v
Dispatcher
    |
    v
Event Handler
    |
    v
State Machine / Action
```

## Event Source

Nguồn tạo event:

```text
GPIO ISR
UART ISR
Timer Tick
DMA Complete
CAN Receive
Application Module
```

## Event

Biểu diễn một việc đã xảy ra:

```text
BUTTON_PRESSED
UART_RX_BYTE
TIMEOUT
SENSOR_DATA_READY
ERROR_DETECTED
```

## Event Queue

Lưu event đang chờ xử lý.

## Dispatcher

Lấy event ra và định tuyến tới module phù hợp.

## Event Handler

Thực thi phản ứng tương ứng với event.

## State Machine

Quyết định phản ứng dựa trên trạng thái hiện tại:

```text
Current State + Event -> Action + Next State
```

---

# 7. Event và Signal

## 7.1 Signal

Signal là mã định danh loại sự kiện.

```c
typedef enum
{
    SIGNAL_NONE = 0,
    SIGNAL_ENTRY,
    SIGNAL_EXIT,
    SIGNAL_SYSTEM_START,
    SIGNAL_BUTTON_PRESSED,
    SIGNAL_BUTTON_RELEASED,
    SIGNAL_TIMEOUT,
    SIGNAL_UART_RX,
    SIGNAL_ERROR,
    SIGNAL_MAX
} signal_t;
```

## 7.2 Event tối thiểu

```c
typedef struct
{
    signal_t signal;
} event_t;
```

## 7.3 Event mở rộng

```c
typedef enum
{
    MODULE_NONE = 0,
    MODULE_BUTTON,
    MODULE_LED,
    MODULE_UART,
    MODULE_APP
} module_id_t;

typedef struct
{
    signal_t signal;
    module_id_t source;
    module_id_t destination;
    uint32_t timestamp_ms;
    uint32_t parameter;
} event_t;
```

## 7.4 Event có payload

```c
typedef struct
{
    event_t super;
    uint8_t byte;
} uart_rx_event_t;
```

## 7.5 Không nên tạo event quá lớn

Không tốt:

```c
typedef struct
{
    signal_t signal;
    uint8_t image[4096];
} image_event_t;
```

Hậu quả:

- Queue tốn nhiều RAM.
- Copy chậm.
- ISR post event lâu.
- Khó dự đoán thời gian thực thi.

Các lựa chọn tốt hơn:

- Gửi index của buffer.
- Gửi handle.
- Gửi pointer có ownership rõ ràng.
- Dùng memory pool.

---

# 8. Event Queue và Mailbox

## 8.1 FIFO Queue

```text
Post:     E1 -> E2 -> E3
Dispatch: E1 -> E2 -> E3
```

## 8.2 Mailbox

Mailbox thường là queue riêng của một đối tượng hoặc task:

```text
Button Mailbox
LED Mailbox
UART Mailbox
Application Mailbox
```

## 8.3 Circular Buffer

```text
+----+----+----+----+
| E0 | E1 | E2 | E3 |
+----+----+----+----+
  ^              ^
 tail           head
```

Khi `head` tới cuối mảng, nó quay về đầu.

## 8.4 Khai báo queue

```c
#include <stdbool.h>
#include <stddef.h>

#define EVENT_QUEUE_CAPACITY (16U)

typedef struct
{
    event_t buffer[EVENT_QUEUE_CAPACITY];
    size_t head;
    size_t tail;
    size_t count;
} event_queue_t;
```

## 8.5 Khởi tạo

```c
void event_queue_init(event_queue_t *queue)
{
    if (queue == NULL)
    {
        return;
    }

    queue->head = 0U;
    queue->tail = 0U;
    queue->count = 0U;
}
```

## 8.6 Kiểm tra trạng thái

```c
bool event_queue_is_empty(const event_queue_t *queue)
{
    return (queue == NULL) || (queue->count == 0U);
}

bool event_queue_is_full(const event_queue_t *queue)
{
    return (queue != NULL) &&
           (queue->count >= EVENT_QUEUE_CAPACITY);
}
```

## 8.7 Post event

```c
bool event_queue_post(event_queue_t *queue,
                      const event_t *event)
{
    if ((queue == NULL) || (event == NULL))
    {
        return false;
    }

    if (event_queue_is_full(queue))
    {
        return false;
    }

    queue->buffer[queue->head] = *event;
    queue->head = (queue->head + 1U) % EVENT_QUEUE_CAPACITY;
    queue->count++;

    return true;
}
```

## 8.8 Get event

```c
bool event_queue_get(event_queue_t *queue,
                     event_t *event)
{
    if ((queue == NULL) || (event == NULL))
    {
        return false;
    }

    if (event_queue_is_empty(queue))
    {
        return false;
    }

    *event = queue->buffer[queue->tail];
    queue->tail = (queue->tail + 1U) % EVENT_QUEUE_CAPACITY;
    queue->count--;

    return true;
}
```

## 8.9 Queue đầy

Các chiến lược có thể dùng:

- Drop event mới nhất.
- Drop event cũ nhất.
- Ghi log overflow.
- Tăng error counter.
- Fatal error với event quan trọng.
- Dùng queue riêng cho event quan trọng.
- Dùng priority queue.

Không được bỏ qua queue overflow một cách âm thầm.

```c
static uint32_t g_event_drop_count;

bool application_post_event(const event_t *event)
{
    const bool posted = event_queue_post(&g_event_queue, event);

    if (!posted)
    {
        g_event_drop_count++;
    }

    return posted;
}
```

---

# 9. Dispatcher

Dispatcher có nhiệm vụ:

- Lấy event khỏi queue.
- Xác định module đích.
- Gọi handler.
- Ghi trace.
- Xử lý event không hợp lệ.

## 9.1 Dispatcher dùng `switch`

```c
void event_dispatch(const event_t *event)
{
    if (event == NULL)
    {
        return;
    }

    switch (event->destination)
    {
        case MODULE_BUTTON:
            button_handle_event(event);
            break;

        case MODULE_LED:
            led_handle_event(event);
            break;

        case MODULE_UART:
            uart_handle_event(event);
            break;

        case MODULE_APP:
            app_handle_event(event);
            break;

        default:
            application_report_invalid_event(event);
            break;
    }
}
```

## 9.2 Routing table

```c
typedef void (*event_handler_fn_t)(const event_t *event);

typedef struct
{
    module_id_t destination;
    event_handler_fn_t handler;
} event_route_t;

static const event_route_t g_routes[] =
{
    { MODULE_BUTTON, button_handle_event },
    { MODULE_LED,    led_handle_event    },
    { MODULE_UART,   uart_handle_event   },
    { MODULE_APP,    app_handle_event    },
};
```

Routing table giúp giảm `switch` lớn và dễ mở rộng hơn.

---

# 10. Event Handler

Handler cần tuân theo nguyên tắc run-to-completion:

- Nhận event.
- Xử lý nhanh.
- Có thể post event mới.
- Trả quyền cho dispatcher.

Ví dụ tốt:

```c
void led_handle_event(const event_t *event)
{
    switch (event->signal)
    {
        case SIGNAL_LED_ON:
            led_on();
            break;

        case SIGNAL_LED_OFF:
            led_off();
            break;

        case SIGNAL_LED_TOGGLE:
            led_toggle();
            break;

        default:
            break;
    }
}
```

Ví dụ không tốt:

```c
void led_handle_event(const event_t *event)
{
    led_on();
    delay_ms(5000U);
    led_off();
}
```

Cách đúng:

```text
LED_ON
  |
  v
Turn LED on
  |
  v
Arm timer 5000 ms
  |
  v
Return immediately
  |
  v
TIMEOUT after 5000 ms
  |
  v
Turn LED off
```

---

# 11. State Machine

## 11.1 Khái niệm

```text
Current State + Event -> Action + Next State
```

Ví dụ:

```text
OFF + BUTTON_PRESSED -> LED ON  + ON
ON  + BUTTON_PRESSED -> LED OFF + OFF
```

## 11.2 Transition table

| Current State | Event | Action | Next State |
|---|---|---|---|
| OFF | BUTTON_PRESSED | LED ON | ON |
| ON | BUTTON_PRESSED | LED OFF | OFF |
| ON | TIMEOUT | LED OFF | OFF |
| OFF | TIMEOUT | None | OFF |

## 11.3 Cài đặt bằng `enum` và `switch`

```c
typedef enum
{
    LED_STATE_OFF = 0,
    LED_STATE_ON,
    LED_STATE_BLINKING
} led_state_t;

typedef struct
{
    led_state_t state;
} led_sm_t;
```

```c
void led_sm_dispatch(led_sm_t *sm, const event_t *event)
{
    if ((sm == NULL) || (event == NULL))
    {
        return;
    }

    switch (sm->state)
    {
        case LED_STATE_OFF:
        {
            if (event->signal == SIGNAL_BUTTON_PRESSED)
            {
                led_on();
                sm->state = LED_STATE_ON;
            }
            break;
        }

        case LED_STATE_ON:
        {
            if (event->signal == SIGNAL_BUTTON_PRESSED)
            {
                led_off();
                sm->state = LED_STATE_OFF;
            }
            break;
        }

        case LED_STATE_BLINKING:
        {
            if (event->signal == SIGNAL_TIMEOUT)
            {
                led_toggle();
            }
            break;
        }

        default:
        {
            led_off();
            sm->state = LED_STATE_OFF;
            break;
        }
    }
}
```

## 11.4 Flat và Hierarchical State Machine

Flat State Machine:

```text
IDLE
RUNNING
ERROR
```

Hierarchical State Machine:

```text
SYSTEM
 ├── ACTIVE
 │    ├── IDLE
 │    └── RUNNING
 └── ERROR
```

Trong chủ đề này, nên làm chủ Flat State Machine trước.

---

# 12. Timer Event

## 12.1 Thay blocking delay bằng timer

Không nên:

```c
led_on();
delay_ms(1000U);
led_off();
```

Nên:

```c
led_on();
software_timer_start(TIMER_LED, 1000U);
```

Sau 1000 ms, timer post `SIGNAL_LED_TIMEOUT`.

## 12.2 Software Timer

```c
typedef struct
{
    bool active;
    bool periodic;
    uint32_t remaining_ticks;
    uint32_t period_ticks;
    signal_t timeout_signal;
    module_id_t destination;
} software_timer_t;
```

```c
void software_timer_tick(software_timer_t *timer)
{
    if ((timer == NULL) || !timer->active)
    {
        return;
    }

    if (timer->remaining_ticks > 0U)
    {
        timer->remaining_ticks--;
    }

    if (timer->remaining_ticks == 0U)
    {
        const event_t event =
        {
            .signal = timer->timeout_signal,
            .source = MODULE_NONE,
            .destination = timer->destination,
            .parameter = 0U
        };

        (void)application_post_event(&event);

        if (timer->periodic)
        {
            timer->remaining_ticks = timer->period_ticks;
        }
        else
        {
            timer->active = false;
        }
    }
}
```

## 12.3 Xử lý tick overflow

Nên tính elapsed bằng phép trừ unsigned:

```c
if ((uint32_t)(now - start_time) >= duration)
{
    /* Timeout */
}
```

Cách này vẫn hoạt động khi bộ đếm tick quay vòng.

---

# 13. Interrupt trong hệ thống Event-Driven

## 13.1 ISR nên làm gì?

- Đọc trạng thái phần cứng.
- Clear interrupt flag.
- Lưu dữ liệu tối thiểu.
- Post event.
- Thoát nhanh.

Ví dụ UART RX:

```c
void USART1_IRQHandler(void)
{
    if (uart_rx_interrupt_pending())
    {
        const uint8_t byte = uart_read_data_register();

        const event_t event =
        {
            .signal = SIGNAL_UART_RX,
            .source = MODULE_UART,
            .destination = MODULE_UART,
            .parameter = byte
        };

        (void)application_post_event_from_isr(&event);
    }
}
```

## 13.2 ISR không nên làm gì?

- `printf()`.
- Delay.
- Parse command dài.
- Cập nhật UI.
- Ghi Flash.
- Chạy State Machine phức tạp.
- Gọi hàm blocking.

## 13.3 Deferred Processing

```text
Hardware interrupt
       |
       v
Read/Clear minimum data
       |
       v
Post software event
       |
       v
Return from ISR
       |
       v
Main event loop handles event
```

## 13.4 Đồng thời giữa ISR và main

Nếu ISR post event còn main get event, queue là dữ liệu dùng chung.

Có thể bảo vệ bằng:

- Critical section rất ngắn.
- Atomic operation.
- Single-producer single-consumer queue.
- Lock-free queue được thiết kế đúng.
- Primitive của RTOS khi dùng RTOS.

Không nên vô hiệu hóa interrupt trong thời gian dài.

---

# 14. Quản lý dữ liệu và bộ nhớ

## 14.1 Ownership

Mỗi dữ liệu cần có owner rõ ràng:

| Dữ liệu | Owner |
|---|---|
| Button state | Button module |
| LED state | LED module |
| Current product mode | Application |
| UART command buffer | UART module |
| Timer list | Timer module |

Module khác không sửa trực tiếp state private.

## 14.2 Đóng gói state

Không nên:

```c
bool g_led_on;
uint32_t g_blink_period;
```

Nên:

```c
typedef struct
{
    led_state_t state;
    uint32_t blink_period_ms;
    bool output_level;
} led_object_t;
```

## 14.3 Truyền dữ liệu bằng event

Thay vì:

```c
g_current_mode = MODE_RUN;
```

Nên:

```c
const event_t event =
{
    .signal = SIGNAL_MODE_CHANGE_REQUEST,
    .source = MODULE_UART,
    .destination = MODULE_APP,
    .parameter = MODE_RUN
};

application_post_event(&event);
```

## 14.4 Copy hay pointer?

Copy event:

- Ownership đơn giản.
- Không phụ thuộc lifetime bên ngoài.
- Phù hợp event nhỏ.

Pointer event:

- Không cần copy payload lớn.
- Phải quản lý lifetime.
- Dễ xảy ra dangling pointer.
- Thường cần memory pool.

Không được gửi pointer tới biến local:

```c
void bad_post(void)
{
    uint8_t local_buffer[8];
    event.data = local_buffer;
    post_event(&event);
}
```

Sau khi hàm kết thúc, `local_buffer` không còn hợp lệ.

## 14.5 Memory Pool

Memory Pool dùng các block tĩnh:

```text
+-------+-------+-------+-------+
|Block 0|Block 1|Block 2|Block 3|
+-------+-------+-------+-------+
  used    free    used    free
```

Ưu điểm:

- Không fragmentation.
- Thời gian cấp phát xác định.
- Phù hợp real-time embedded.

Ở chủ đề này nên bắt đầu bằng event có kích thước cố định, sau đó mới mở rộng sang pool event.

---

# 15. Event-Driven Framework tối thiểu

## 15.1 Cấu trúc thư mục

```text
event-driven-foundation/
├── README.md
├── Makefile
├── include/
│   ├── event.h
│   ├── event_queue.h
│   ├── dispatcher.h
│   ├── software_timer.h
│   ├── button.h
│   ├── led.h
│   └── app.h
├── src/
│   ├── event_queue.c
│   ├── dispatcher.c
│   ├── software_timer.c
│   ├── button.c
│   ├── led.c
│   ├── app.c
│   └── main.c
├── platform/
│   ├── gpio.c
│   ├── uart.c
│   └── systick.c
├── tests/
│   ├── test_event_queue.c
│   ├── test_timer.c
│   └── test_app_sm.c
└── docs/
    ├── architecture.md
    ├── event-list.md
    └── state-machines.md
```

## 15.2 `event.h`

```c
#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>

typedef enum
{
    SIGNAL_NONE = 0,
    SIGNAL_SYSTEM_START,
    SIGNAL_BUTTON_EDGE,
    SIGNAL_BUTTON_DEBOUNCE_TIMEOUT,
    SIGNAL_BUTTON_PRESSED,
    SIGNAL_BUTTON_RELEASED,
    SIGNAL_LED_ON,
    SIGNAL_LED_OFF,
    SIGNAL_LED_TIMEOUT,
    SIGNAL_UART_RX,
    SIGNAL_COMMAND_RECEIVED,
    SIGNAL_ERROR,
    SIGNAL_MAX
} signal_t;

typedef enum
{
    MODULE_NONE = 0,
    MODULE_BUTTON,
    MODULE_LED,
    MODULE_UART,
    MODULE_APP,
    MODULE_MAX
} module_id_t;

typedef struct
{
    signal_t signal;
    module_id_t source;
    module_id_t destination;
    uint32_t parameter;
} event_t;

#endif
```

## 15.3 Main Event Loop

```c
int main(void)
{
    board_init();
    application_event_system_init();
    button_init();
    led_init();
    uart_init();
    app_init();

    const event_t start_event =
    {
        .signal = SIGNAL_SYSTEM_START,
        .source = MODULE_NONE,
        .destination = MODULE_APP,
        .parameter = 0U
    };

    (void)application_post_event(&start_event);

    while (1)
    {
        event_t event;

        if (event_queue_get(&g_application_queue, &event))
        {
            event_trace(&event);
            event_dispatch(&event);
        }
        else
        {
            platform_idle();
        }
    }
}
```

## 15.4 Idle

```c
void platform_idle(void)
{
    __asm volatile ("wfi");
}
```

`WFI` giúp CPU chờ interrupt thay vì polling liên tục.

Cần kiểm tra race condition giữa lúc queue rỗng và lúc CPU chuẩn bị sleep.

---

# 16. Ví dụ: Button điều khiển LED

## 16.1 Yêu cầu

- Nhấn lần đầu: LED bật.
- Nhấn lần hai: LED tắt.
- Có debounce.
- Không dùng blocking delay.
- ISR chỉ post event.

## 16.2 Luồng xử lý

```text
GPIO Interrupt
      |
      v
Post BUTTON_EDGE
      |
      v
Button Handler
      |
      v
Start debounce timer
      |
      v
BUTTON_DEBOUNCE_TIMEOUT
      |
      v
Read stable GPIO level
      |
      +--> BUTTON_PRESSED
      |
      +--> BUTTON_RELEASED
              |
              v
       Application State Machine
              |
              v
        LED_ON / LED_OFF
```

## 16.3 Button State Machine

```c
typedef enum
{
    BUTTON_STATE_RELEASED = 0,
    BUTTON_STATE_DEBOUNCING_PRESS,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_DEBOUNCING_RELEASE
} button_state_t;
```

Transition cơ bản:

| State | Event | Điều kiện | Action | Next State |
|---|---|---|---|---|
| RELEASED | BUTTON_EDGE | — | Start timer | DEBOUNCING_PRESS |
| DEBOUNCING_PRESS | TIMEOUT | Pin active | Post PRESSED | PRESSED |
| DEBOUNCING_PRESS | TIMEOUT | Pin inactive | None | RELEASED |
| PRESSED | BUTTON_EDGE | — | Start timer | DEBOUNCING_RELEASE |
| DEBOUNCING_RELEASE | TIMEOUT | Pin inactive | Post RELEASED | RELEASED |

## 16.4 Application State Machine

```c
typedef enum
{
    APP_STATE_LED_OFF = 0,
    APP_STATE_LED_ON
} app_state_t;
```

```c
void app_handle_event(const event_t *event)
{
    if (event == NULL)
    {
        return;
    }

    switch (g_app.state)
    {
        case APP_STATE_LED_OFF:
        {
            if (event->signal == SIGNAL_BUTTON_PRESSED)
            {
                const event_t led_event =
                {
                    .signal = SIGNAL_LED_ON,
                    .source = MODULE_APP,
                    .destination = MODULE_LED,
                    .parameter = 0U
                };

                (void)application_post_event(&led_event);
                g_app.state = APP_STATE_LED_ON;
            }
            break;
        }

        case APP_STATE_LED_ON:
        {
            if (event->signal == SIGNAL_BUTTON_PRESSED)
            {
                const event_t led_event =
                {
                    .signal = SIGNAL_LED_OFF,
                    .source = MODULE_APP,
                    .destination = MODULE_LED,
                    .parameter = 0U
                };

                (void)application_post_event(&led_event);
                g_app.state = APP_STATE_LED_OFF;
            }
            break;
        }

        default:
            g_app.state = APP_STATE_LED_OFF;
            break;
    }
}
```

## 16.5 LED Handler

```c
void led_handle_event(const event_t *event)
{
    if (event == NULL)
    {
        return;
    }

    switch (event->signal)
    {
        case SIGNAL_LED_ON:
            led_set(true);
            break;

        case SIGNAL_LED_OFF:
            led_set(false);
            break;

        default:
            break;
    }
}
```

---

# 17. Ví dụ: UART nhận lệnh

## 17.1 Yêu cầu

UART nhận:

```text
led on
led off
status
```

Không parse command trong ISR.

## 17.2 Luồng

```text
UART RX Interrupt
       |
       v
Read one byte
       |
       v
Post UART_RX
       |
       v
UART Handler
       |
       v
Append to command buffer
       |
       +--> Newline?
                |
                v
         Parse command
                |
                v
   Post business-level event
```

Ví dụ event nghiệp vụ:

```text
LED_ON_REQUEST
LED_OFF_REQUEST
STATUS_REQUEST
```

Cách này tốt hơn việc để Application truy cập trực tiếp command buffer của UART.

## 17.3 Parser object

```c
#define COMMAND_BUFFER_SIZE (32U)

typedef struct
{
    char buffer[COMMAND_BUFFER_SIZE];
    size_t length;
} command_parser_t;
```

Cần xử lý:

- Buffer đầy.
- Ký tự không hợp lệ.
- CR/LF.
- Lệnh quá dài.
- Reset parser sau khi hoàn tất.

---

# 18. Quy trình phát triển sản phẩm

## Bước 1 — Phân tích yêu cầu

Ví dụ:

- Button đổi chế độ.
- LED hiển thị trạng thái.
- UART cấu hình.
- Sensor cung cấp dữ liệu.
- Lỗi đưa hệ thống vào Error State.

## Bước 2 — Chia module

```text
Button
LED
UART
Sensor
Application
Error Manager
Timer Service
```

## Bước 3 — Xác định Event Source

| Event Source | Event |
|---|---|
| GPIO interrupt | BUTTON_EDGE |
| Timer | TIMEOUT |
| UART interrupt | UART_RX |
| ADC/DMA | SENSOR_DATA_READY |
| Error detector | ERROR_DETECTED |

## Bước 4 — Lập Event List

| Signal | Source | Destination | Payload | Mô tả |
|---|---|---|---|---|
| SYSTEM_START | System | App | None | Khởi động ứng dụng |
| BUTTON_PRESSED | Button | App | None | Button đã debounce |
| LED_ON | App | LED | None | Bật LED |
| UART_RX | ISR | UART | Byte | Byte vừa nhận |
| TIMEOUT | Timer | Module | Timer ID | Hết thời gian |

## Bước 5 — Xác định ownership

Mỗi state và buffer chỉ có một owner.

## Bước 6 — Vẽ State Machine

```text
              BUTTON_PRESSED
+---------+ --------------------> +---------+
|  IDLE   |                       | ACTIVE  |
+---------+ <-------------------- +---------+
              BUTTON_PRESSED

ACTIVE --ERROR_DETECTED--> ERROR
ERROR  --RESET_REQUEST----> IDLE
```

## Bước 7 — Thiết kế queue

Cần trả lời:

- Queue chung hay mailbox riêng?
- Capacity bao nhiêu?
- Event lớn nhất bao nhiêu byte?
- Producer nào chạy trong ISR?
- Khi queue đầy làm gì?
- Có cần priority không?

## Bước 8 — Viết interface module

```c
void button_init(void);
void button_handle_event(const event_t *event);

void led_init(void);
void led_handle_event(const event_t *event);

void app_init(void);
void app_handle_event(const event_t *event);
```

## Bước 9 — Unit Test

Test queue, timer và state transition trước hoặc song song với tích hợp phần cứng.

## Bước 10 — Tích hợp hardware

Kết nối GPIO, UART, timer, sensor và interrupt.

## Bước 11 — Trace Event

```text
[001025] SRC=BUTTON DST=APP SIG=BUTTON_PRESSED PARAM=0
[001026] SRC=APP    DST=LED SIG=LED_ON PARAM=0
```

## Bước 12 — Đánh giá

- Có blocking delay không?
- ISR có quá dài không?
- Queue có overflow không?
- Handler có chạy lâu không?
- State Machine có rõ không?
- Module có phụ thuộc vòng không?

---

# 19. Best Practices

1. Handler phải run-to-completion.
2. ISR chỉ làm phần tối thiểu.
3. Mỗi module sở hữu state của chính nó.
4. Các module giao tiếp bằng event.
5. Event nên mô tả ý nghĩa nghiệp vụ rõ ràng.
6. Không dùng event tên mơ hồ như `DO_THING` hoặc `UPDATE`.
7. Event payload phải có ownership rõ ràng.
8. Không gửi pointer tới biến local.
9. Queue phải có overflow policy.
10. Không phụ thuộc vào thứ tự event ngầm định.
11. Tách framework, application và platform.
12. Ghi trace event để debug.
13. Đo thời gian chạy handler nếu hệ thống có yêu cầu timing.
14. Không dùng `malloc()` tùy tiện cho từng event.
15. Thiết kế State Machine và transition table trước khi code.

Tổ chức layer:

```text
framework/
  event_queue.c
  dispatcher.c
  software_timer.c

application/
  app.c
  button.c
  led.c

platform/
  gpio.c
  uart.c
  systick.c
```

---

# 20. Anti-pattern và lỗi thường gặp

## 20.1 Blocking trong handler

```c
void handle_event(const event_t *event)
{
    delay_ms(1000U);
}
```

Hậu quả:

- Dispatcher bị block.
- Event latency tăng.
- Queue có thể đầy.

## 20.2 Gọi trực tiếp module khác quá sâu

Không tốt:

```c
void button_handler(void)
{
    app_change_mode();
    display_update();
    uart_send_status();
}
```

Nên post event nghiệp vụ cho Application.

## 20.3 Event storm

Một event tạo quá nhiều event mới:

```text
E1 -> E2, E3, E4
E2 -> E5, E6
E3 -> E7, E8
```

Giải pháp:

- Rate limit.
- Debounce.
- Coalescing.
- Chỉ post khi state thực sự thay đổi.

## 20.4 Duplicate Event

Display refresh được post nhiều lần dù chỉ cần một event pending.

Có thể dùng cờ nội bộ:

```text
If REFRESH already pending:
    Do not enqueue another REFRESH
```

## 20.5 Lost Event

Nguyên nhân:

- Queue đầy.
- Race condition.
- Pointer hết lifetime.
- ISR clear flag sai.
- Event bị overwrite.

## 20.6 State không đồng bộ

Ví dụ Application nghĩ LED đang bật nhưng LED module đang tắt.

Giải pháp:

- Một owner duy nhất.
- Command/confirmation event khi cần.
- Không sao chép state không cần thiết.

## 20.7 Heap không kiểm soát

Cấp phát động cho từng event có thể gây:

- Fragmentation.
- Memory leak.
- Allocation failure.
- Latency không xác định.

Nên dùng static queue hoặc memory pool.

---

# 21. Kiểm thử

## 21.1 Test FIFO

```c
void test_queue_fifo(void)
{
    event_queue_t queue;
    event_queue_init(&queue);

    const event_t e1 = { .signal = SIGNAL_BUTTON_PRESSED };
    const event_t e2 = { .signal = SIGNAL_LED_TIMEOUT };

    assert(event_queue_post(&queue, &e1));
    assert(event_queue_post(&queue, &e2));

    event_t output;

    assert(event_queue_get(&queue, &output));
    assert(output.signal == SIGNAL_BUTTON_PRESSED);

    assert(event_queue_get(&queue, &output));
    assert(output.signal == SIGNAL_LED_TIMEOUT);
}
```

## 21.2 Test queue full

```c
void test_queue_full(void)
{
    event_queue_t queue;
    event_queue_init(&queue);

    const event_t event = { .signal = SIGNAL_SYSTEM_START };

    for (size_t i = 0U; i < EVENT_QUEUE_CAPACITY; i++)
    {
        assert(event_queue_post(&queue, &event));
    }

    assert(event_queue_is_full(&queue));
    assert(!event_queue_post(&queue, &event));
}
```

## 21.3 Test wraparound

1. Post một phần queue.
2. Get một số event.
3. Post thêm để `head` quay về đầu.
4. Kiểm tra thứ tự FIFO vẫn đúng.

## 21.4 Test State Machine

```c
void test_led_toggle(void)
{
    app_init();

    const event_t event =
    {
        .signal = SIGNAL_BUTTON_PRESSED
    };

    app_handle_event(&event);
    assert(app_get_state() == APP_STATE_LED_ON);

    app_handle_event(&event);
    assert(app_get_state() == APP_STATE_LED_OFF);
}
```

## 21.5 Test invalid input

Handler không được crash khi nhận:

- `NULL`.
- Signal không hợp lệ.
- Destination sai.
- Payload ngoài phạm vi.

## 21.6 Test timer

Dùng fake tick thay vì chờ thời gian thật.

## 21.7 Test event sequence

Chuỗi mong đợi:

```text
BUTTON_EDGE
BUTTON_DEBOUNCE_TIMEOUT
BUTTON_PRESSED
LED_ON
```

---

# 22. Bài thực hành

## Bài 1 — Chuyển blocking sang non-blocking

Chuyển:

```c
led_on();
delay_ms(1000U);
led_off();
delay_ms(1000U);
```

thành thiết kế dùng timer event.

## Bài 2 — Circular Event Queue

Yêu cầu:

- FIFO.
- Capacity cố định.
- `post()`.
- `get()`.
- `is_empty()`.
- `is_full()`.
- Test wraparound.
- Counter overflow.

## Bài 3 — Dispatcher

- Tối thiểu ba module.
- Route theo destination.
- Có invalid-event handler.
- Có trace.

## Bài 4 — Button debounce

- GPIO interrupt.
- ISR chỉ post event.
- Debounce bằng software timer.
- Phân biệt pressed và released.

## Bài 5 — LED State Machine

State:

```text
OFF
ON
BLINKING
```

Event:

```text
LED_ON
LED_OFF
LED_BLINK_START
LED_BLINK_STOP
LED_TIMEOUT
```

## Bài 6 — UART command parser

Lệnh:

```text
led on
led off
blink 500
status
```

## Bài 7 — Event Trace

Log dạng CSV:

```text
timestamp,source,destination,signal,parameter
```

## Bài 8 — Queue stress test

Thống kê:

- Tổng event post.
- Tổng event dispatch.
- Event bị drop.
- Queue high-water mark.

## Bài 9 — Product State Machine

State:

```text
BOOT
IDLE
ACTIVE
ERROR
```

Event:

```text
SYSTEM_START
BUTTON_PRESSED
SENSOR_READY
ERROR_DETECTED
RESET_REQUEST
```

## Bài 10 — So sánh kiến trúc

Cài cùng một chức năng theo:

1. Super-loop polling.
2. Event-Driven.

So sánh:

- Biến toàn cục.
- Độ dài `main()`.
- Latency.
- Khả năng test.
- Khả năng mở rộng.
- RAM và CPU usage.

---

# 23. Tiêu chí hoàn thành

- Phân biệt synchronous và asynchronous.
- Giải thích được Event-Driven Programming.
- Phân tích được vấn đề của blocking delay.
- Thiết kế được danh sách signal.
- Viết được event structure.
- Viết được circular event queue.
- Xử lý queue full và empty.
- Có overflow counter hoặc error policy.
- Viết được dispatcher.
- Viết được handler run-to-completion.
- Viết được software timer.
- Chuyển xử lý từ ISR sang event context.
- Viết được State Machine bằng `enum` và `switch`.
- Vẽ được state diagram.
- Viết được transition table.
- Xác định được ownership dữ liệu.
- Không gửi pointer tới biến local qua event.
- Unit test queue và State Machine.
- Trace được luồng event.
- Demo chạy được trên board.
- Có tài liệu trên GitHub.

---

# 24. Cấu trúc repository đề xuất

```text
02-asynchronous-event-driven/
├── README.md
├── Makefile
├── include/
│   ├── event.h
│   ├── event_queue.h
│   ├── dispatcher.h
│   ├── software_timer.h
│   ├── app.h
│   ├── button.h
│   ├── led.h
│   └── uart_service.h
├── src/
│   ├── event_queue.c
│   ├── dispatcher.c
│   ├── software_timer.c
│   ├── app.c
│   ├── button.c
│   ├── led.c
│   ├── uart_service.c
│   └── main.c
├── platform/
│   ├── gpio.c
│   ├── uart.c
│   ├── systick.c
│   └── critical_section.c
├── tests/
│   ├── test_event_queue.c
│   ├── test_timer.c
│   ├── test_button_sm.c
│   ├── test_led_sm.c
│   └── test_app_sm.c
├── docs/
│   ├── architecture.md
│   ├── event-list.md
│   ├── sequence-diagrams.md
│   └── state-machines.md
└── build/
```

README project thực hành nên có:

- Mục tiêu.
- Board và MCU.
- Sơ đồ kiến trúc.
- Danh sách module.
- Danh sách event.
- State diagram.
- Event sequence.
- Queue capacity.
- Overflow policy.
- Data ownership.
- Cách build và flash.
- Cách test.
- Event trace mẫu.
- Kết quả thực nghiệm.
- Hạn chế và hướng phát triển.

---

# 25. Tài liệu tham khảo

## Event Driven — Task & Signal

- https://epcb.vn/blogs/ak-embedded-software/ak-embedded-base-kit-stm32l151-event-driven-task-signal

## Event Driven — Timer

- https://epcb.vn/blogs/ak-embedded-software/ak-embedded-base-kit-stm32l151-event-driven-timer

## Tài liệu nên đọc thêm

- Datasheet và Reference Manual của MCU.
- ARM Cortex-M Generic User Guide.
- Tài liệu NVIC và interrupt.
- Tài liệu timer của MCU.
- Finite State Machine.
- Circular Buffer.
- Run-to-completion.
- Producer–consumer.
- Memory Pool trong Embedded System.

---

# Tổng kết

Event-Driven Programming giúp chuyển firmware từ mô hình:

```text
Super-loop lớn
+ nhiều polling
+ nhiều delay
+ nhiều cờ trạng thái
+ nhiều biến toàn cục
```

sang mô hình:

```text
Event Source
    |
    v
Event Queue
    |
    v
Dispatcher
    |
    v
Event Handler
    |
    v
State Machine
```

Các nguyên tắc cốt lõi:

1. ISR phải ngắn.
2. Handler không được blocking.
3. Mỗi module sở hữu state của chính nó.
4. Module giao tiếp bằng event.
5. Queue phải có overflow policy.
6. Event payload phải có ownership rõ ràng.
7. State Machine nên được thiết kế trước khi code.
8. Event cần có khả năng trace.
9. Framework, application và platform nên tách riêng.
10. Queue, timer và state transition phải được kiểm thử.

Sau chủ đề này, người học cần tự xây dựng được một Event-Driven Framework tối thiểu mà không phụ thuộc vào Active Kernel hoặc framework có sẵn. Đây là nền tảng để tiếp tục học Active Object, Mailbox, Pool Event, Timer Service và Active Kernel.