# Chủ đề 3 - Active Kernel (AK)
## Khái niệm, tính năng và cách sử dụng

> Chủ đề này tập trung vào cách sử dụng Active Kernel để tổ chức một firmware hướng sự kiện bằng **Task**, **Signal**, **Message**, **Timer** và **State Machine**.  
> Sau khi hoàn thành, người học cần hiểu luồng xử lý bên trong AK, biết tạo task, gửi message, cấu hình timer, xây dựng state machine và sử dụng các công cụ debug cơ bản.

---

## Mục lục

1. [Mục tiêu học tập](#1-mục-tiêu-học-tập)
2. [Active Kernel là gì?](#2-active-kernel-là-gì)
3. [AK không phải RTOS truyền thống](#3-ak-không-phải-rtos-truyền-thống)
4. [Kiến trúc tổng thể](#4-kiến-trúc-tổng-thể)
5. [Luồng khởi động AK](#5-luồng-khởi-động-ak)
6. [Task trong AK](#6-task-trong-ak)
7. [Task Priority](#7-task-priority)
8. [Signal](#8-signal)
9. [Message trong AK](#9-message-trong-ak)
10. [Pure Message](#10-pure-message)
11. [Common Message](#11-common-message)
12. [Dynamic Message](#12-dynamic-message)
13. [Message Pool và Reference Count](#13-message-pool-và-reference-count)
14. [Gửi message giữa các task](#14-gửi-message-giữa-các-task)
15. [Task Scheduler và Run-to-Completion](#15-task-scheduler-và-run-to-completion)
16. [Polling Task](#16-polling-task)
17. [Timer trong AK](#17-timer-trong-ak)
18. [One-shot Timer](#18-one-shot-timer)
19. [Periodic Timer](#19-periodic-timer)
20. [State Machine trong AK](#20-state-machine-trong-ak)
21. [FSM dạng function-based](#21-fsm-dạng-function-based)
22. [Table State Machine](#22-table-state-machine)
23. [Interrupt và AK](#23-interrupt-và-ak)
24. [Log và Debug](#24-log-và-debug)
25. [Fatal Error](#25-fatal-error)
26. [Command Line qua UART](#26-command-line-qua-uart)
27. [Realtime Event và Record Event](#27-realtime-event-và-record-event)
28. [Ví dụ hoàn chỉnh: Button điều khiển LED](#28-ví-dụ-hoàn-chỉnh-button-điều-khiển-led)
29. [Ví dụ gửi dữ liệu cảm biến](#29-ví-dụ-gửi-dữ-liệu-cảm-biến)
30. [Ví dụ Timer và State Machine](#30-ví-dụ-timer-và-state-machine)
31. [Cách phân chia task](#31-cách-phân-chia-task)
32. [Quản lý tài nguyên](#32-quản-lý-tài-nguyên)
33. [Quy trình phát triển với AK](#33-quy-trình-phát-triển-với-ak)
34. [Best Practices](#34-best-practices)
35. [Lỗi thường gặp](#35-lỗi-thường-gặp)
36. [Bài thực hành](#36-bài-thực-hành)
37. [Tiêu chí hoàn thành](#37-tiêu-chí-hoàn-thành)
38. [Cấu trúc repository đề xuất](#38-cấu-trúc-repository-đề-xuất)
39. [Tài liệu tham khảo](#39-tài-liệu-tham-khảo)

---

# 1. Mục tiêu học tập

Sau khi hoàn thành chủ đề, người học cần có khả năng:

- Giải thích được vai trò của Active Kernel trong firmware hướng sự kiện.
- Phân biệt AK với một RTOS preemptive truyền thống.
- Hiểu được các thành phần:
  - Task.
  - Task ID.
  - Task Priority.
  - Signal.
  - Message.
  - Message Pool.
  - Timer.
  - Finite State Machine.
  - Table State Machine.
- Tạo được bảng task cho ứng dụng.
- Gửi được:
  - Pure Message.
  - Common Message.
  - Dynamic Message.
- Hiểu ownership và vòng đời của message.
- Sử dụng one-shot và periodic timer.
- Tổ chức logic task theo mô hình run-to-completion.
- Chuyển xử lý từ ISR sang task context.
- Xây dựng State Machine bằng API của AK.
- Theo dõi lượng sử dụng message pool và timer pool.
- Tạo command line đơn giản qua UART.
- Ghi log, trace event và xử lý fatal error.
- Hoàn thành một project nhỏ chạy trên AK Embedded Base Kit.

---

# 2. Active Kernel là gì?

Active Kernel là một framework tổ chức firmware theo mô hình hướng sự kiện.

Thay vì viết toàn bộ logic trong một vòng lặp lớn:

```c
while (1)
{
    process_button();
    process_uart();
    process_display();
    process_sensor();
}
```

AK chia hệ thống thành các task độc lập:

```text
Button Task
Application Task
Display Task
UART Task
Sensor Task
Debug Task
```

Các task không cần gọi trực tiếp vào nhau. Chúng trao đổi thông tin qua message:

```text
Task A
  |
  | post message
  v
AK Scheduler / Queue
  |
  | dispatch
  v
Task B
```

Các thành phần trung tâm:

```text
+----------------------+
|      Event Source    |
| Button/UART/Timer/...|
+----------+-----------+
           |
           v
+----------------------+
|       Message        |
| src, des, sig, data  |
+----------+-----------+
           |
           v
+----------------------+
| Task Queue / Priority|
+----------+-----------+
           |
           v
+----------------------+
|     Task Handler     |
+----------+-----------+
           |
           v
+----------------------+
|    State Machine     |
+----------------------+
```

---

# 3. AK không phải RTOS truyền thống

## 3.1 RTOS preemptive

Trong một RTOS truyền thống:

- Mỗi task thường có stack riêng.
- Scheduler có thể chuyển context giữa các task.
- Task có thể block.
- Có semaphore, mutex, queue, event group.
- Priority có thể gây preemption.

```text
Task A running
    |
    | higher-priority task becomes ready
    v
Context switch
    |
    v
Task B running
```

## 3.2 Active Kernel

Trong AK:

- Task là một event handler.
- Task thường không có stack riêng.
- Handler chạy đến khi kết thúc.
- Task không được block lâu.
- Message kích hoạt task.
- Scheduler lấy message và gọi task phù hợp.
- Priority được sử dụng để lựa chọn nhóm task/message cần xử lý.

```text
Get message
    |
    v
Call task handler
    |
    v
Handler finishes
    |
    v
Get next message
```

Đây là mô hình **run-to-completion**.

## 3.3 Hệ quả thiết kế

Vì task không có stack riêng, hệ thống tiết kiệm RAM hơn so với nhiều RTOS task.

Đổi lại, người lập trình phải bảo đảm:

- Không dùng delay blocking.
- Không chờ vòng lặp dài trong task.
- Không đọc UART bằng busy-wait.
- Không giữ CPU quá lâu.
- Tách công việc dài thành nhiều event.
- Dùng timer để chờ thời gian.
- Dùng state machine để lưu tiến trình.

---

# 4. Kiến trúc tổng thể

```text
                      +------------------+
                      | Hardware IRQ     |
                      | GPIO/UART/Timer  |
                      +---------+--------+
                                |
                                | create/post message
                                v
+---------------+      +------------------+
| Application   |----->| Message Pool     |
| Task          |      | Pure/Common/Dyn  |
+---------------+      +---------+--------+
                                |
                                v
                      +------------------+
                      | Task Queue       |
                      | Priority Queues  |
                      +---------+--------+
                                |
                                v
                      +------------------+
                      | task_run()       |
                      | Scheduler        |
                      +---------+--------+
                                |
                                v
                      +------------------+
                      | Destination Task |
                      | task_xxx(msg)    |
                      +---------+--------+
                                |
                                v
                      +------------------+
                      | FSM / TSM        |
                      +------------------+
```

Có thể chia source code thành ba lớp:

```text
Application
    |
    v
Active Kernel
    |
    v
Platform / Driver
```

### Application

- Task nghiệp vụ.
- Signal nghiệp vụ.
- State Machine.
- Logic sản phẩm.

### Active Kernel

- Message Pool.
- Task Scheduler.
- Timer.
- FSM.
- Routing message.

### Platform

- GPIO.
- UART.
- Timer hardware.
- Interrupt.
- Critical section.

---

# 5. Luồng khởi động AK

Một luồng khởi động điển hình:

```text
Reset
  |
  v
Startup Code
  |
  v
main()
  |
  v
Hardware initialization
  |
  v
task_init()
  |
  v
task_create(app_task_table)
  |
  v
task_polling_create(...)
  |
  v
Application initialization
  |
  v
Start software timers
  |
  v
task_run()
```

Ví dụ khung:

```c
int main(void)
{
    board_init();

    task_init();
    task_create((task_t *)app_task_table);

    app_init();

    task_post_pure_msg(AC_TASK_APP_ID, APP_SYSTEM_START_SIG);

    for (;;)
    {
        task_run();
        task_polling_run();
    }
}
```

Tùy phiên bản project, `task_run()` có thể chứa vòng scheduler hoặc được gọi từ vòng lặp chính.

Điểm quan trọng:

1. Kernel phải được khởi tạo trước khi post message.
2. Bảng task phải kết thúc bằng phần tử EOT.
3. Hardware phải sẵn sàng trước khi task sử dụng driver.
4. Timer tick phải được cung cấp đều đặn cho AK.

---

# 6. Task trong AK

Trong AK, task được mô tả bởi kiểu:

```c
typedef struct
{
    task_id_t id;
    task_pri_t pri;
    pf_task task;
} task_t;
```

Trong đó:

| Thành phần | Ý nghĩa |
|---|---|
| `id` | Mã định danh duy nhất của task |
| `pri` | Mức ưu tiên |
| `task` | Con trỏ tới hàm xử lý message |

Handler có dạng:

```c
void task_led(ak_msg_t *msg);
```

Ví dụ:

```c
void task_led(ak_msg_t *msg)
{
    switch (msg->sig)
    {
        case LED_ON_SIG:
        {
            led_on();
            break;
        }

        case LED_OFF_SIG:
        {
            led_off();
            break;
        }

        default:
        {
            break;
        }
    }
}
```

Task không phải một thread độc lập. Đây là một hàm được AK gọi khi có message gửi tới.

---

# 7. Task Priority

AK định nghĩa các mức priority:

```c
TASK_PRI_LEVEL_0
TASK_PRI_LEVEL_1
TASK_PRI_LEVEL_2
TASK_PRI_LEVEL_3
TASK_PRI_LEVEL_4
TASK_PRI_LEVEL_5
TASK_PRI_LEVEL_6
TASK_PRI_LEVEL_7
```

Ví dụ bảng task:

```c
const task_t app_task_table[] =
{
    {TASK_TIMER_TICK_ID, TASK_PRI_LEVEL_7, task_timer_tick},
    {AC_TASK_APP_ID,     TASK_PRI_LEVEL_2, task_app},
    {AC_TASK_BUTTON_ID,  TASK_PRI_LEVEL_3, task_button},
    {AC_TASK_LED_ID,     TASK_PRI_LEVEL_4, task_led},
    {AC_TASK_SHELL_ID,   TASK_PRI_LEVEL_2, task_shell},

    {AK_TASK_EOT_ID, TASK_PRI_LEVEL_0, (pf_task)0}
};
```

## Quy tắc lựa chọn priority

Không nên đặt mọi task ở priority cao nhất.

Gợi ý:

| Loại task | Priority gợi ý |
|---|---|
| Timer tick nội bộ | Cao |
| Communication RX | Trung bình-cao |
| Control logic | Trung bình |
| Display refresh | Trung bình-thấp |
| Debug/Shell | Thấp |
| Background statistic | Thấp |

Priority phải được quyết định dựa trên:

- Deadline.
- Tần suất event.
- Thời gian handler.
- Mức quan trọng.
- Khả năng gây đầy queue.

## Lưu ý

Priority cao không sửa được một handler viết sai.

Nếu task priority cao chạy quá lâu:

- Task khác vẫn bị trễ.
- Queue vẫn có thể đầy.
- System latency vẫn tăng.

---

# 8. Signal

Signal biểu diễn loại event.

AK dành một vùng signal cho hệ thống và một vùng cho application.

Ví dụ:

```c
enum
{
    APP_SYSTEM_START_SIG = AK_USER_DEFINE_SIG,
    APP_BUTTON_PRESSED_SIG,
    APP_BUTTON_RELEASED_SIG,
    APP_LED_ON_SIG,
    APP_LED_OFF_SIG,
    APP_LED_BLINK_SIG,
    APP_SENSOR_DATA_SIG,
    APP_TIMEOUT_SIG
};
```

## Quy tắc đặt tên

Nên đặt rõ ý nghĩa:

```text
BUTTON_PRESSED_SIG
SENSOR_DATA_READY_SIG
DISPLAY_REFRESH_REQ_SIG
UART_TX_DONE_SIG
```

Không nên:

```text
SIG_1
DO_IT_SIG
DATA_SIG
UPDATE_SIG
```

## Signal là sự kiện, không phải câu lệnh máy móc

Tốt:

```text
BUTTON_PRESSED_SIG
```

Task Application quyết định hành động dựa trên state hiện tại.

Ít tốt hơn:

```text
TURN_LED_ON_AND_PLAY_BUZZER_SIG
```

Signal quá cụ thể làm tăng coupling giữa các module.

---

# 9. Message trong AK

Header message chính là `ak_msg_t`.

Các trường quan trọng:

```c
typedef struct ak_msg_t
{
    struct ak_msg_t *next;

    uint8_t src_task_id;
    uint8_t des_task_id;
    uint8_t ref_count;
    uint8_t sig;

    uint8_t if_src_task_id;
    uint8_t if_des_task_id;
    uint8_t if_src_type;
    uint8_t if_des_type;
    uint8_t if_sig;
} ak_msg_t;
```

Trong xử lý nội bộ, thường quan tâm:

| Field | Ý nghĩa |
|---|---|
| `src_task_id` | Task gửi |
| `des_task_id` | Task nhận |
| `sig` | Signal |
| `ref_count` | Theo dõi số nơi đang tham chiếu message |
| `next` | Liên kết message trong queue/pool |

Các macro thường dùng:

```c
set_msg_sig(msg, signal);
set_msg_src_task_id(msg, task_id);
set_msg_des_task_id(msg, task_id);
```

AK cung cấp ba loại message chính:

```text
Pure Message
Common Message
Dynamic Message
```

---

# 10. Pure Message

Pure Message chỉ mang signal, không có payload.

Cấu trúc:

```c
typedef struct
{
    ak_msg_t msg_header;
} ak_msg_pure_t;
```

Dùng khi chỉ cần thông báo:

- Button pressed.
- Timeout.
- LED on.
- System start.
- Refresh request.

Gửi nhanh:

```c
task_post_pure_msg(AC_TASK_LED_ID, LED_ON_SIG);
```

Luồng:

```text
Application Task
      |
      | task_post_pure_msg()
      v
Pure Message Pool
      |
      v
LED Task Queue
      |
      v
task_led(msg)
```

## Khi nên dùng

- Event không cần dữ liệu.
- Dữ liệu đã thuộc sở hữu task nhận.
- Signal tự nó đã đủ ý nghĩa.

## Không nên dùng khi

Task nhận cần biết giá trị cụ thể như:

- Nhiệt độ.
- UART byte.
- CAN frame.
- Tọa độ.
- Tham số cấu hình.

---

# 11. Common Message

Common Message chứa một buffer có kích thước cố định.

Cấu trúc:

```c
typedef struct
{
    ak_msg_t msg_header;
    uint8_t len;
    uint8_t data[AK_COMMON_MSG_DATA_SIZE];
} ak_msg_common_t;
```

Trong cấu hình mặc định của header tham khảo:

```c
AK_COMMON_MSG_POOL_SIZE
AK_COMMON_MSG_DATA_SIZE
```

Các macro này có thể được override trong file cấu hình build.

## Gửi common message

```c
typedef struct
{
    int16_t temperature_x10;
    uint16_t humidity_x10;
} sensor_data_t;

sensor_data_t data =
{
    .temperature_x10 = 275,
    .humidity_x10 = 650
};

task_post_common_msg(
    AC_TASK_APP_ID,
    SENSOR_DATA_READY_SIG,
    (uint8_t *)&data,
    sizeof(data)
);
```

## Đọc dữ liệu

```c
void task_app(ak_msg_t *msg)
{
    switch (msg->sig)
    {
        case SENSOR_DATA_READY_SIG:
        {
            uint8_t *raw = get_data_common_msg(msg);
            uint8_t len = get_data_len_common_msg(msg);

            if (len == sizeof(sensor_data_t))
            {
                sensor_data_t data;
                memcpy(&data, raw, sizeof(data));

                app_process_sensor_data(&data);
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

## Ưu điểm

- Không dùng heap chuẩn.
- Payload nằm trong block cố định.
- Dễ dự đoán RAM.
- Lifetime do kernel quản lý.
- Phù hợp dữ liệu nhỏ.

## Hạn chế

- Payload bị giới hạn bởi `AK_COMMON_MSG_DATA_SIZE`.
- Copy dữ liệu.
- Pool có số block hữu hạn.

---

# 12. Dynamic Message

Dynamic Message chứa payload có kích thước linh hoạt.

Cấu trúc:

```c
typedef struct
{
    ak_msg_t msg_header;
    uint32_t len;
    uint8_t *data;
} ak_msg_dynamic_t;
```

API liên quan:

```c
get_dynamic_msg();
set_data_dynamic_msg();
get_data_dynamic_msg();
get_data_len_dynamic_msg();
task_post_dynamic_msg();
```

Ví dụ:

```c
uint8_t frame[128];

size_t frame_len = build_protocol_frame(frame, sizeof(frame));

task_post_dynamic_msg(
    AC_TASK_COMM_ID,
    COMM_TX_FRAME_SIG,
    frame,
    frame_len
);
```

## Khi nên dùng

- Payload lớn hơn common message.
- Độ dài thay đổi.
- Protocol frame.
- Chuỗi lệnh dài.
- Buffer truyền thông.

## Rủi ro

- Dynamic pool có giới hạn.
- Có thể thiếu block khi traffic tăng.
- Tốn thời gian copy.
- Cần kiểm tra kích thước.
- Không nên dùng tùy tiện cho mọi message.

## Nguyên tắc

Ưu tiên:

```text
Pure Message
    |
    v
Common Message
    |
    v
Dynamic Message
```

Chỉ dùng loại phức tạp hơn khi thực sự cần.

---

# 13. Message Pool và Reference Count

## 13.1 Tại sao dùng pool?

Nếu mỗi event gọi `malloc()`:

```c
message = malloc(sizeof(message_t));
```

firmware có thể gặp:

- Fragmentation.
- Allocation failure.
- Thời gian cấp phát không xác định.
- Memory leak.
- Khó phân tích RAM.

AK sử dụng các pool riêng:

```text
Pure Message Pool
Common Message Pool
Dynamic Message Pool
Dynamic Data Pool
Timer Pool
```

## 13.2 Pool hữu hạn

Ví dụ cấu hình mặc định trong header có các macro:

```c
AK_PURE_MSG_POOL_SIZE
AK_COMMON_MSG_POOL_SIZE
AK_DYNAMIC_MSG_POOL_SIZE
AK_DYNAMIC_DATA_POOL_SIZE
AK_TIMER_POOL_SIZE
```

Các giá trị phải được điều chỉnh theo sản phẩm.

Không nên tăng pool một cách cảm tính.

Cần đo:

- Current usage.
- Maximum usage.
- Event burst lớn nhất.
- Worst-case latency.
- Số producer đồng thời.

## 13.3 Theo dõi pool

AK cung cấp các hàm dạng:

```c
get_pure_msg_pool_used();
get_pure_msg_pool_used_max();

get_common_msg_pool_used();
get_common_msg_pool_used_max();

get_dynamic_msg_pool_used();
get_dynamic_msg_pool_used_max();

get_timer_msg_pool_used();
get_timer_msg_pool_used_max();
```

Có thể tạo command shell:

```text
pool
```

Kết quả:

```text
Pure    : current=3 max=11 total=32
Common  : current=1 max=6  total=8
Dynamic : current=0 max=4  total=8
Timer   : current=5 max=9  total=16
```

## 13.4 Reference Count

Reference count giúp một message có thể được chia sẻ hoặc theo dõi vòng đời.

API:

```c
msg_inc_ref_count(msg);
msg_dec_ref_count(msg);
msg_free(msg);
msg_force_free(msg);
```

Không nên tự ý gọi `msg_force_free()` trong application nếu chưa hiểu ownership.

Sai:

```c
task_post(TASK_A, msg);
msg_force_free(msg);
```

Task A có thể nhận pointer tới message đã bị giải phóng.

Nguyên tắc:

- Người tạo message phải hiểu ai sở hữu message sau khi post.
- Không sử dụng message sau khi kernel đã nhận ownership.
- Không giữ pointer message ra ngoài handler nếu không tăng reference count đúng cách.
- Không force-free message đang nằm trong queue.

---

# 14. Gửi message giữa các task

AK cung cấp các API mức cao:

```c
task_post_pure_msg();
task_post_common_msg();
task_post_dynamic_msg();
```

Ngoài ra có API tổng quát:

```c
task_post(task_id_t des_task_id, ak_msg_t *msg);
```

## 14.1 Pure message

```c
task_post_pure_msg(
    AC_TASK_DISPLAY_ID,
    DISPLAY_REFRESH_SIG
);
```

## 14.2 Common message

```c
display_model_t model;

task_post_common_msg(
    AC_TASK_DISPLAY_ID,
    DISPLAY_MODEL_UPDATE_SIG,
    (uint8_t *)&model,
    sizeof(model)
);
```

## 14.3 Dynamic message

```c
task_post_dynamic_msg(
    AC_TASK_COMM_ID,
    COMM_SEND_PACKET_SIG,
    packet,
    packet_len
);
```

## 14.4 Gửi message tự tạo

```c
ak_msg_t *msg = get_common_msg();

if (msg != AK_MSG_NULL)
{
    set_msg_sig(msg, SENSOR_DATA_READY_SIG);

    if (set_data_common_msg(
            msg,
            (uint8_t *)&data,
            sizeof(data)) == AK_MSG_OK)
    {
        task_post(AC_TASK_APP_ID, msg);
    }
    else
    {
        msg_free(msg);
    }
}
```

Luôn kiểm tra việc lấy message từ pool có thành công hay không.

---

# 15. Task Scheduler và Run-to-Completion

Scheduler thực hiện logic khái niệm:

```text
while system is running:
    find highest-priority ready queue
    get one message
    identify destination task
    call task handler
    free/recycle message
```

Pseudo-code:

```c
int task_run(void)
{
    ak_msg_t *msg = scheduler_get_next_message();

    if (msg != AK_MSG_NULL)
    {
        task_t *task = find_task(msg->des_task_id);

        if (task != NULL)
        {
            task->task(msg);
        }

        msg_free(msg);
    }

    return AK_RET_OK;
}
```

## Run-to-completion

Handler phải kết thúc trước khi message tiếp theo được xử lý.

Tốt:

```c
void task_led(ak_msg_t *msg)
{
    if (msg->sig == LED_BLINK_SIG)
    {
        led_toggle();
        timer_set(
            AC_TASK_LED_ID,
            LED_BLINK_TIMEOUT_SIG,
            500U,
            TIMER_ONE_SHOT
        );
    }
}
```

Sai:

```c
void task_led(ak_msg_t *msg)
{
    if (msg->sig == LED_BLINK_SIG)
    {
        while (1)
        {
            led_toggle();
            delay_ms(500U);
        }
    }
}
```

Task trên không trả quyền điều khiển cho kernel.

---

# 16. Polling Task

AK cũng có khái niệm polling task:

```c
typedef struct
{
    task_id_t id;
    uint8_t ability;
    pf_task_polling task_polling;
} task_polling_t;
```

Bảng polling:

```c
const task_polling_t app_task_polling_table[] =
{
    {
        AC_TASK_POLLING_CONSOLE_ID,
        AK_ENABLE,
        task_polling_console
    },

    {
        AK_TASK_POLLING_EOT_ID,
        AK_DISABLE,
        (pf_task_polling)0
    }
};
```

API:

```c
task_polling_create();
task_polling_run();
task_polling_set_ability();
```

## Khi dùng polling task

- Driver không hỗ trợ interrupt.
- Cần đọc console ring buffer.
- Kiểm tra trạng thái rất nhanh.
- Tác vụ background nhỏ.

## Không lạm dụng

Polling task không nên trở thành nơi chứa toàn bộ application logic.

Không tốt:

```c
void polling_task(void)
{
    process_everything();
}
```

Tốt hơn:

```c
void task_polling_console(void)
{
    if (uart_rx_available())
    {
        uint8_t byte = uart_read();

        task_post_common_msg(
            AC_TASK_SHELL_ID,
            SHELL_RX_BYTE_SIG,
            &byte,
            1U
        );
    }
}
```

Polling chỉ phát hiện điều kiện, sau đó post event.

---

# 17. Timer trong AK

AK timer chuyển một khoảng thời gian thành một signal gửi tới task.

Cấu trúc timer:

```c
typedef struct ak_timer_t
{
    struct ak_timer_t *next;
    task_id_t des_task_id;
    timer_sig_t sig;
    uint32_t counter;
    uint32_t period;
} ak_timer_t;
```

Hai loại timer:

```c
TIMER_ONE_SHOT
TIMER_PERIODIC
```

API chính:

```c
timer_init();
timer_tick();
timer_set();
timer_remove_attr();
```

Timer được xác định bởi:

```text
Destination Task ID + Signal
```

## Luồng timer

```text
timer_set(task, signal, duty, type)
              |
              v
       Timer list/pool
              |
              v
     Hardware tick occurs
              |
              v
         timer_tick()
              |
              v
       Counter reaches 0
              |
              v
 Post signal to destination task
```

---

# 18. One-shot Timer

One-shot timer chỉ tạo event một lần.

```c
timer_set(
    AC_TASK_BUTTON_ID,
    BUTTON_DEBOUNCE_TIMEOUT_SIG,
    20U,
    TIMER_ONE_SHOT
);
```

Dùng cho:

- Debounce.
- Communication timeout.
- Delayed action.
- Sensor warm-up.
- State timeout.

Ví dụ:

```c
void task_button(ak_msg_t *msg)
{
    switch (msg->sig)
    {
        case BUTTON_EDGE_SIG:
        {
            timer_set(
                AC_TASK_BUTTON_ID,
                BUTTON_DEBOUNCE_TIMEOUT_SIG,
                20U,
                TIMER_ONE_SHOT
            );
            break;
        }

        case BUTTON_DEBOUNCE_TIMEOUT_SIG:
        {
            if (button_read())
            {
                task_post_pure_msg(
                    AC_TASK_APP_ID,
                    BUTTON_PRESSED_SIG
                );
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

# 19. Periodic Timer

Periodic timer tự reload sau mỗi lần hết hạn.

```c
timer_set(
    AC_TASK_LIFE_ID,
    LIFE_TICK_SIG,
    1000U,
    TIMER_PERIODIC
);
```

Task:

```c
void task_life(ak_msg_t *msg)
{
    if (msg->sig == LIFE_TICK_SIG)
    {
        led_life_toggle();
    }
}
```

Dùng cho:

- Heartbeat.
- Sensor sampling.
- Display refresh.
- Statistics.
- Watchdog service.

## Xóa timer

```c
timer_remove_attr(
    AC_TASK_LIFE_ID,
    LIFE_TICK_SIG
);
```

Cần xóa timer khi:

- Rời state.
- Tắt module.
- Hủy operation.
- Không muốn nhận timeout cũ.

---

# 20. State Machine trong AK

State Machine giúp task phản ứng khác nhau với cùng một signal tùy state.

Ví dụ:

```text
State IDLE + START_SIG   -> RUNNING
State RUNNING + STOP_SIG -> IDLE
State RUNNING + ERROR_SIG -> ERROR
```

AK có hai cách tổ chức nổi bật:

```text
FSM  — function-based state machine
TSM  — table state machine
```

---

# 21. FSM dạng function-based

Định nghĩa cơ bản:

```c
typedef void (*state_handler)(ak_msg_t *msg);

typedef struct
{
    state_handler state;
} fsm_t;
```

Macro:

```c
FSM(me, init_func);
FSM_TRAN(me, target);
```

Dispatch:

```c
fsm_dispatch(fsm_t *me, ak_msg_t *msg);
```

## Ví dụ

```c
typedef struct
{
    fsm_t super;
    uint32_t blink_period;
} led_fsm_t;

static led_fsm_t g_led_fsm;
```

State prototypes:

```c
static void led_state_off(ak_msg_t *msg);
static void led_state_on(ak_msg_t *msg);
static void led_state_blink(ak_msg_t *msg);
```

Khởi tạo:

```c
void led_fsm_init(void)
{
    FSM(&g_led_fsm, led_state_off);
    g_led_fsm.blink_period = 500U;
}
```

Task:

```c
void task_led(ak_msg_t *msg)
{
    fsm_dispatch(&g_led_fsm.super, msg);
}
```

State OFF:

```c
static void led_state_off(ak_msg_t *msg)
{
    switch (msg->sig)
    {
        case LED_ON_SIG:
        {
            led_on();
            FSM_TRAN(&g_led_fsm, led_state_on);
            break;
        }

        case LED_BLINK_START_SIG:
        {
            led_on();

            timer_set(
                AC_TASK_LED_ID,
                LED_BLINK_TIMEOUT_SIG,
                g_led_fsm.blink_period,
                TIMER_PERIODIC
            );

            FSM_TRAN(&g_led_fsm, led_state_blink);
            break;
        }

        default:
        {
            break;
        }
    }
}
```

State ON:

```c
static void led_state_on(ak_msg_t *msg)
{
    switch (msg->sig)
    {
        case LED_OFF_SIG:
        {
            led_off();
            FSM_TRAN(&g_led_fsm, led_state_off);
            break;
        }

        case LED_BLINK_START_SIG:
        {
            timer_set(
                AC_TASK_LED_ID,
                LED_BLINK_TIMEOUT_SIG,
                g_led_fsm.blink_period,
                TIMER_PERIODIC
            );

            FSM_TRAN(&g_led_fsm, led_state_blink);
            break;
        }

        default:
        {
            break;
        }
    }
}
```

State BLINK:

```c
static void led_state_blink(ak_msg_t *msg)
{
    switch (msg->sig)
    {
        case LED_BLINK_TIMEOUT_SIG:
        {
            led_toggle();
            break;
        }

        case LED_BLINK_STOP_SIG:
        {
            timer_remove_attr(
                AC_TASK_LED_ID,
                LED_BLINK_TIMEOUT_SIG
            );

            led_off();
            FSM_TRAN(&g_led_fsm, led_state_off);
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

# 22. Table State Machine

TSM mô tả transition bằng bảng.

Một entry:

```c
typedef struct
{
    uint8_t sig;
    tsm_state_t next_state;
    tsm_func_f tsm_func;
} tsm_t;
```

Bảng tổng:

```c
typedef struct tsm_tbl_t
{
    tsm_state_t state;
    tsm_on_state_f on_state;
    tsm_t **table;
} tsm_tbl_t;
```

API:

```c
TSM(table_object, table, initial_state);
TSM_TRAN(table_object, new_state);
tsm_dispatch(table_object, msg);
```

## Transition table khái niệm

| State | Signal | Action | Next State |
|---|---|---|---|
| OFF | LED_ON | Turn on | ON |
| OFF | BLINK_START | Start timer | BLINK |
| ON | LED_OFF | Turn off | OFF |
| BLINK | TIMEOUT | Toggle | BLINK |
| BLINK | BLINK_STOP | Stop timer | OFF |

## Ưu điểm

- Dễ nhìn toàn bộ transition.
- Dễ review.
- Dễ sinh tài liệu.
- Phù hợp state machine có cấu trúc đều.

## Nhược điểm

- Bảng lớn có thể khó đọc.
- Action phức tạp vẫn cần hàm riêng.
- Cần quy ước rõ state và signal.

## Khi chọn FSM hay TSM?

Dùng FSM function-based khi:

- Mỗi state có logic khác biệt rõ.
- State handler có nhiều nhánh.
- Cần code trực quan.

Dùng TSM khi:

- Transition table rõ ràng.
- Nhiều state có cấu trúc giống nhau.
- Muốn review state transition dễ hơn.

---

# 23. Interrupt và AK

AK cung cấp các hook:

```c
task_entry_interrupt();
task_exit_interrupt();
```

Một ISR khái niệm:

```c
void EXTI_IRQHandler(void)
{
    task_entry_interrupt();

    if (button_irq_pending())
    {
        button_irq_clear();

        task_post_pure_msg(
            AC_TASK_BUTTON_ID,
            BUTTON_EDGE_SIG
        );
    }

    task_exit_interrupt();
}
```

## ISR nên làm

- Xác định nguồn interrupt.
- Clear interrupt flag.
- Đọc dữ liệu tối thiểu.
- Post message.
- Thoát nhanh.

## ISR không nên làm

- Parse command.
- Render display.
- Dùng delay.
- Chạy state machine dài.
- Ghi Flash dài.
- In log quá nhiều.
- Dùng dynamic message lớn nếu có thể tránh.

## UART RX

```c
void USART_IRQHandler(void)
{
    task_entry_interrupt();

    if (uart_rx_ready())
    {
        uint8_t byte = uart_read_byte();

        task_post_common_msg(
            AC_TASK_UART_ID,
            UART_RX_BYTE_SIG,
            &byte,
            1U
        );
    }

    task_exit_interrupt();
}
```

Nếu tốc độ UART cao, post một message cho từng byte có thể gây tải lớn.

Giải pháp tốt hơn:

- ISR ghi vào ring buffer.
- Post một event `UART_RX_AVAILABLE_SIG`.
- UART task đọc nhiều byte từ ring buffer.

---

# 24. Log và Debug

Một hệ thống Event-Driven có lợi thế lớn: mọi hành động quan trọng đều đi qua event.

Có thể log:

```text
Timestamp
Source Task
Destination Task
Signal
Post time
Start execution time
Stop execution time
```

Trong cấu hình debug, message có thể chứa thông tin thời gian:

```c
typedef struct
{
    uint32_t start_post;
    uint32_t start_exe;
    uint32_t stop_exe;
} dbg_handler_t;
```

Từ đó tính:

```text
Queue latency  = start_exe - start_post
Handler time   = stop_exe - start_exe
Total latency  = stop_exe - start_post
```

## Log mức ứng dụng

```c
APP_PRINT(
    "[LED] sig=%u state=%u\n",
    msg->sig,
    g_led_state
);
```

## Log event

```text
[00001234] SRC=BUTTON DST=APP SIG=BUTTON_PRESSED
[00001235] SRC=APP    DST=LED SIG=LED_ON
```

## Không log quá nhiều trong ISR

`printf()` trong ISR có thể:

- Làm ISR kéo dài.
- Mất UART data.
- Tăng interrupt latency.
- Gây deadlock nếu driver không reentrant.

Nên:

- Ghi record nhỏ vào buffer.
- Gửi ra UART ở debug task.
- Cho phép bật/tắt từng nhóm log.

---

# 25. Fatal Error

Fatal Error là lỗi khiến hệ thống không thể tiếp tục hoạt động an toàn.

Ví dụ:

- Message pool cạn ở luồng quan trọng.
- Corrupt queue.
- Invalid task ID.
- Stack overflow.
- HardFault.
- Watchdog timeout.
- State không hợp lệ.
- Assertion thất bại.

## Fatal handler nên làm

1. Disable hoặc giới hạn chức năng nguy hiểm.
2. Lưu mã lỗi.
3. Lưu reset reason.
4. Lưu task hiện tại.
5. Lưu signal hiện tại.
6. Lưu CPU register nếu có thể.
7. Ghi record vào vùng không mất dữ liệu.
8. Reset hoặc đi vào safe state.

Ví dụ:

```c
typedef struct
{
    uint32_t magic;
    uint32_t error_code;
    uint32_t timestamp;
    uint8_t current_task;
    uint8_t current_signal;
    uint32_t restart_count;
} fatal_record_t;
```

Fatal API khái niệm:

```c
void app_fatal(uint32_t error_code)
{
    fatal_record_t record;

    record.magic = FATAL_MAGIC;
    record.error_code = error_code;
    record.timestamp = system_tick_get();
    record.current_task = get_current_task_id();

    fatal_record_save(&record);

    system_reset();
}
```

## Fatal không phải log thông thường

Không nên gọi fatal cho lỗi có thể phục hồi:

- User nhập sai command.
- Sensor trả về một sample lỗi.
- Packet checksum sai.
- Button bounce.

Các lỗi này nên được báo và xử lý bằng state machine.

---

# 26. Command Line qua UART

Command Line giúp kiểm thử firmware mà không cần sửa code hoặc dùng debugger.

Luồng:

```text
PC Terminal
    |
    | UART
    v
UART RX Driver
    |
    v
Ring Buffer
    |
    v
Shell Task
    |
    v
Command Parser
    |
    v
Post command event / print result
```

## Các command nên có

```text
help
version
task
pool
timer
state
event
led on
led off
led blink 500
reset
fatal
ram
stack
cpu
```

## Ví dụ parser đơn giản

```c
static int shell_led(int argc, char **argv)
{
    if (argc < 2)
    {
        shell_print("usage: led on|off|blink\n");
        return -1;
    }

    if (strcmp(argv[1], "on") == 0)
    {
        task_post_pure_msg(
            AC_TASK_LED_ID,
            LED_ON_SIG
        );
        return 0;
    }

    if (strcmp(argv[1], "off") == 0)
    {
        task_post_pure_msg(
            AC_TASK_LED_ID,
            LED_OFF_SIG
        );
        return 0;
    }

    shell_print("unknown led option\n");
    return -1;
}
```

## Shell không nên gọi driver ứng dụng trực tiếp

Không tốt:

```c
led_gpio_write(1);
```

Tốt:

```c
task_post_pure_msg(AC_TASK_LED_ID, LED_ON_SIG);
```

Như vậy command line đi qua đúng luồng sản phẩm và có thể trace.

---

# 27. Realtime Event và Record Event

## 27.1 Realtime Event

Realtime Event là dữ liệu event được quan sát ngay khi firmware đang chạy.

Ví dụ stream qua UART:

```text
1234,1,3,11
1235,3,4,20
1236,4,5,21
```

Có thể biểu diễn:

```text
timestamp,source,destination,signal
```

Ứng dụng trên PC có thể:

- Hiển thị timeline.
- Vẽ sequence diagram.
- Phát hiện event burst.
- Đo latency.
- Quan sát state transition.

## 27.2 Record Event

Record Event lưu một số event vào ring buffer trong RAM hoặc Flash.

Ví dụ:

```c
typedef struct
{
    uint32_t timestamp;
    uint8_t source;
    uint8_t destination;
    uint8_t signal;
    uint8_t state;
} event_record_t;
```

Ring buffer:

```c
#define EVENT_RECORD_CAPACITY (64U)

static event_record_t g_records[EVENT_RECORD_CAPACITY];
static uint16_t g_record_head;
```

Ghi record:

```c
void event_record_write(const ak_msg_t *msg, uint8_t state)
{
    event_record_t *record = &g_records[g_record_head];

    record->timestamp = system_tick_get();
    record->source = msg->src_task_id;
    record->destination = msg->des_task_id;
    record->signal = msg->sig;
    record->state = state;

    g_record_head =
        (g_record_head + 1U) % EVENT_RECORD_CAPACITY;
}
```

## Mục tiêu

- Điều tra lỗi hiếm.
- Xem event trước khi watchdog reset.
- Phân tích dead state.
- Kiểm tra sequence thực tế.
- So sánh với use case thiết kế.

## Không ghi Flash cho mọi event

Flash có giới hạn erase/write cycle.

Nên:

- Ghi ring buffer trong RAM.
- Chỉ flush khi fatal.
- Dùng external Flash nếu cần.
- Giảm kích thước record.
- Dùng mã số thay vì chuỗi.

---

# 28. Ví dụ hoàn chỉnh: Button điều khiển LED

## 28.1 Yêu cầu

- Nhấn button: LED đổi trạng thái.
- Debounce bằng one-shot timer.
- ISR chỉ post message.
- Application giữ mode.
- LED task sở hữu GPIO LED.

## 28.2 Danh sách task

```c
enum
{
    AC_TASK_APP_ID = 1,
    AC_TASK_BUTTON_ID,
    AC_TASK_LED_ID
};
```

## 28.3 Danh sách signal

```c
enum
{
    BUTTON_EDGE_SIG = AK_USER_DEFINE_SIG,
    BUTTON_DEBOUNCE_TIMEOUT_SIG,
    BUTTON_PRESSED_SIG,
    LED_ON_SIG,
    LED_OFF_SIG
};
```

## 28.4 Bảng task

```c
const task_t app_task_table[] =
{
    {
        TASK_TIMER_TICK_ID,
        TASK_PRI_LEVEL_7,
        task_timer_tick
    },

    {
        AC_TASK_BUTTON_ID,
        TASK_PRI_LEVEL_4,
        task_button
    },

    {
        AC_TASK_APP_ID,
        TASK_PRI_LEVEL_3,
        task_app
    },

    {
        AC_TASK_LED_ID,
        TASK_PRI_LEVEL_2,
        task_led
    },

    {
        AK_TASK_EOT_ID,
        TASK_PRI_LEVEL_0,
        (pf_task)0
    }
};
```

## 28.5 ISR

```c
void EXTI_BUTTON_IRQHandler(void)
{
    task_entry_interrupt();

    if (button_irq_pending())
    {
        button_irq_clear();

        task_post_pure_msg(
            AC_TASK_BUTTON_ID,
            BUTTON_EDGE_SIG
        );
    }

    task_exit_interrupt();
}
```

## 28.6 Button task

```c
void task_button(ak_msg_t *msg)
{
    switch (msg->sig)
    {
        case BUTTON_EDGE_SIG:
        {
            timer_remove_attr(
                AC_TASK_BUTTON_ID,
                BUTTON_DEBOUNCE_TIMEOUT_SIG
            );

            timer_set(
                AC_TASK_BUTTON_ID,
                BUTTON_DEBOUNCE_TIMEOUT_SIG,
                20U,
                TIMER_ONE_SHOT
            );
            break;
        }

        case BUTTON_DEBOUNCE_TIMEOUT_SIG:
        {
            if (button_read_active())
            {
                task_post_pure_msg(
                    AC_TASK_APP_ID,
                    BUTTON_PRESSED_SIG
                );
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

## 28.7 Application task

```c
typedef enum
{
    APP_LED_OFF = 0,
    APP_LED_ON
} app_led_state_t;

static app_led_state_t g_app_led_state;
```

```c
void task_app(ak_msg_t *msg)
{
    switch (msg->sig)
    {
        case BUTTON_PRESSED_SIG:
        {
            if (g_app_led_state == APP_LED_OFF)
            {
                task_post_pure_msg(
                    AC_TASK_LED_ID,
                    LED_ON_SIG
                );

                g_app_led_state = APP_LED_ON;
            }
            else
            {
                task_post_pure_msg(
                    AC_TASK_LED_ID,
                    LED_OFF_SIG
                );

                g_app_led_state = APP_LED_OFF;
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

## 28.8 LED task

```c
void task_led(ak_msg_t *msg)
{
    switch (msg->sig)
    {
        case LED_ON_SIG:
        {
            led_on();
            break;
        }

        case LED_OFF_SIG:
        {
            led_off();
            break;
        }

        default:
        {
            break;
        }
    }
}
```

## 28.9 Sequence

```text
Button IRQ
    |
    v
BUTTON_EDGE_SIG
    |
    v
Button Task
    |
    v
Start debounce timer
    |
    v
BUTTON_DEBOUNCE_TIMEOUT_SIG
    |
    v
BUTTON_PRESSED_SIG
    |
    v
Application Task
    |
    +--> LED_ON_SIG
    |
    +--> LED_OFF_SIG
            |
            v
         LED Task
```

---

# 29. Ví dụ gửi dữ liệu cảm biến

## 29.1 Payload

```c
typedef struct
{
    int16_t temperature_x10;
    uint16_t humidity_x10;
    uint32_t timestamp;
} sensor_sample_t;
```

## 29.2 Sensor task gửi dữ liệu

```c
void sensor_publish_sample(void)
{
    sensor_sample_t sample;

    sample.temperature_x10 = sensor_read_temperature_x10();
    sample.humidity_x10 = sensor_read_humidity_x10();
    sample.timestamp = system_tick_get();

    task_post_common_msg(
        AC_TASK_APP_ID,
        SENSOR_SAMPLE_SIG,
        (uint8_t *)&sample,
        sizeof(sample)
    );
}
```

## 29.3 Application nhận

```c
void task_app(ak_msg_t *msg)
{
    switch (msg->sig)
    {
        case SENSOR_SAMPLE_SIG:
        {
            uint8_t len = get_data_len_common_msg(msg);

            if (len != sizeof(sensor_sample_t))
            {
                app_report_error(
                    APP_ERROR_INVALID_SENSOR_PAYLOAD
                );
                break;
            }

            sensor_sample_t sample;

            memcpy(
                &sample,
                get_data_common_msg(msg),
                sizeof(sample)
            );

            app_update_sensor_model(&sample);

            task_post_common_msg(
                AC_TASK_DISPLAY_ID,
                DISPLAY_SENSOR_DATA_SIG,
                (uint8_t *)&sample,
                sizeof(sample)
            );
            break;
        }

        default:
        {
            break;
        }
    }
}
```

## 29.4 Lưu ý

Nếu gửi cùng một message đến nhiều task:

- Phải hiểu reference count.
- Không tự free sớm.
- Hoặc tạo message riêng cho từng destination.
- Hoặc để Application chuyển đổi thành event mới.

Cách đơn giản nhất cho người mới:

```text
Một post -> một destination
```

---

# 30. Ví dụ Timer và State Machine

## 30.1 Yêu cầu

LED có ba state:

```text
OFF
ON
BLINKING
```

Command:

```text
LED_ON_SIG
LED_OFF_SIG
LED_BLINK_START_SIG
LED_BLINK_STOP_SIG
```

Timer:

```text
LED_BLINK_TIMEOUT_SIG
```

## 30.2 State diagram

```text
                 LED_ON
          +-------------------+
          |                   v
      +-------+           +-------+
      |  OFF  |           |  ON   |
      +-------+           +-------+
          ^                   |
          |      LED_OFF      |
          +-------------------+

OFF/ON -- LED_BLINK_START --> BLINKING

+----------+
| BLINKING |
+----------+
     |
     | LED_BLINK_TIMEOUT
     v
 toggle LED and remain BLINKING

BLINKING -- LED_BLINK_STOP --> OFF
```

## 30.3 Nguyên tắc timer theo state

Khi vào BLINKING:

```c
timer_set(... TIMER_PERIODIC);
```

Khi rời BLINKING:

```c
timer_remove_attr(...);
```

Nếu không remove timer, task có thể nhận timeout khi đã ở state khác.

---

# 31. Cách phân chia task

Không nên tạo một task cho từng hàm nhỏ.

Task nên đại diện cho:

- Một Active Object.
- Một domain logic.
- Một resource owner.
- Một protocol layer.
- Một state machine độc lập.

Ví dụ hợp lý:

```text
Application Task
Button Task
Display Task
Audio Task
Communication Task
Storage Task
Debug Task
```

Ví dụ quá vụn:

```text
LED On Task
LED Off Task
LED Toggle Task
Button Read Task
Button Check Task
```

## Tiêu chí tách task

Tách thành task riêng khi module:

- Có queue/event riêng.
- Có state riêng.
- Sở hữu hardware resource.
- Có timer riêng.
- Có lifecycle độc lập.
- Cần tách priority.
- Cần test độc lập.

## Không tách task khi

- Chỉ là utility function.
- Không có state.
- Không nhận event.
- Không sở hữu resource.
- Luôn chạy đồng bộ trong task khác.

---

# 32. Quản lý tài nguyên

## 32.1 Một resource — một owner

Ví dụ:

| Resource | Owner |
|---|---|
| OLED | Display Task |
| UART TX | UART Task |
| Buzzer | Audio Task |
| Flash | Storage Task |
| Product mode | Application Task |

Task khác gửi request, không truy cập trực tiếp.

```text
App Task
   |
   | DISPLAY_DRAW_SIG
   v
Display Task
   |
   v
OLED driver
```

## 32.2 Tránh race condition

Nếu nhiều task cùng gọi UART driver:

```text
App Task ----+
             +--> UART Driver
Debug Task --+
```

Có thể xảy ra dữ liệu xen kẽ.

Tốt hơn:

```text
App Task ----+
             |
Debug Task --+--> UART TX Task --> UART Driver
```

## 32.3 Critical section

Chỉ dùng cho thao tác rất ngắn:

- Update queue metadata.
- Update shared counter.
- Access ISR-shared ring buffer.

Không dùng critical section cho:

- Gửi chuỗi dài.
- Ghi Flash.
- Render display.
- Parse command.

---

# 33. Quy trình phát triển với AK

## Bước 1 — Viết yêu cầu

Ví dụ:

- Nhấn button để đổi màn hình.
- Game update mỗi 20 ms.
- UART có command debug.
- Buzzer phát âm thanh khi va chạm.

## Bước 2 — Xác định task

```text
Game Task
Input Task
Display Task
Audio Task
Shell Task
```

## Bước 3 — Xác định ownership

```text
Game Task owns game state.
Input Task owns button debounce state.
Display Task owns OLED.
Audio Task owns buzzer.
Shell Task owns command buffer.
```

## Bước 4 — Lập danh sách signal

Tạo `docs/signals.md`:

| Signal | Source | Destination | Payload |
|---|---|---|---|
| BUTTON_PRESSED | Input | Game | Button ID |
| GAME_TICK | Timer | Game | None |
| FRAME_READY | Game | Display | Frame/model |
| PLAY_SOUND | Game | Audio | Sound ID |
| SHELL_COMMAND | Shell | App | Command ID |

## Bước 5 — Chọn message type

| Dữ liệu | Message |
|---|---|
| Chỉ signal | Pure |
| Struct nhỏ | Common |
| Buffer dài | Dynamic |

## Bước 6 — Thiết kế State Machine

Vẽ trước khi code:

```text
BOOT -> MENU -> PLAYING -> GAME_OVER
```

## Bước 7 — Chọn timer

- Game tick.
- Debounce.
- Animation.
- Timeout.
- Heartbeat.

## Bước 8 — Xác định priority

Không đặt tùy ý. Ghi lý do trong tài liệu.

## Bước 9 — Tính pool

Ước lượng:

```text
Maximum messages queued
+ simultaneous ISR posts
+ periodic timer events
+ communication burst
```

## Bước 10 — Viết test

- Task nhận đúng signal.
- Payload đúng kích thước.
- State transition đúng.
- Timer được remove đúng.
- Pool không cạn.
- Invalid signal không crash.

## Bước 11 — Thêm trace

Ghi:

```text
src, des, sig, timestamp
```

## Bước 12 — Stress test

- Nhấn button liên tục.
- UART gửi burst.
- Timer chạy đồng thời.
- Display refresh cao.
- Communication timeout.

---

# 34. Best Practices

## Task handler phải ngắn

Mục tiêu:

```text
Nhận event
-> xử lý nhỏ
-> cập nhật state
-> post event tiếp theo nếu cần
-> return
```

## Không dùng delay

Sai:

```c
delay_ms(1000U);
```

Đúng:

```c
timer_set(...);
```

## Kiểm tra payload length

```c
if (get_data_len_common_msg(msg) != sizeof(expected_t))
{
    return;
}
```

## Kiểm tra pool allocation

```c
ak_msg_t *msg = get_common_msg();

if (msg == AK_MSG_NULL)
{
    app_report_pool_exhausted();
    return;
}
```

## Mỗi task sở hữu state riêng

Không expose biến nội bộ:

```c
static app_state_t g_state;
```

## Signal phải có namespace

```text
APP_...
BUTTON_...
DISPLAY_...
COMM_...
```

## Remove timer khi rời state

Không để timeout cũ tác động state mới.

## Shell phải gửi event

Không bypass architecture.

## Đo high-water mark

Theo dõi mức sử dụng pool tối đa trong stress test.

## Log bằng ID thay vì chuỗi dài

Trong record runtime:

```text
task_id + signal + timestamp
```

PC tool có thể chuyển ID thành tên.

---

# 35. Lỗi thường gặp

## Task không nhận message

Kiểm tra:

- Task có trong `app_task_table`.
- Task ID có đúng không.
- Bảng có EOT không.
- Kernel đã `task_init()` chưa.
- Đã `task_create()` chưa.
- Signal có đúng không.
- Pool có cạn không.

## Timer không chạy

Kiểm tra:

- Timer tick hardware.
- `timer_tick()` có được gọi không.
- `task_timer_tick` có trong task table không.
- Duty có cùng đơn vị với tick không.
- Timer pool có đầy không.
- Destination task ID đúng không.

## Common message bị sai dữ liệu

Nguyên nhân:

- Payload lớn hơn giới hạn.
- Ép kiểu sai.
- Sai `len`.
- Struct padding khác nhau.
- Đọc message sai type.

## Dynamic pool hết

Nguyên nhân:

- Message chưa được free.
- Traffic burst lớn.
- Handler quá chậm.
- Dynamic message bị lạm dụng.
- Pool cấu hình quá nhỏ.

## System bị chậm

Kiểm tra:

- Task handler nào chạy lâu.
- Có `printf()` nhiều không.
- Có polling nặng không.
- Event storm.
- Periodic timer quá nhanh.
- Display render quá thường xuyên.

## Event loop bị treo

Tìm:

- `while` không thoát.
- Blocking UART.
- Delay.
- Chờ cờ phần cứng không timeout.
- Dead loop trong state handler.

## Nhận timeout ở state sai

Nguyên nhân:

- Không remove timer khi chuyển state.
- Signal timer dùng chung cho nhiều operation.
- Timeout cũ còn trong queue.

Giải pháp:

- Remove timer.
- Dùng operation ID trong payload.
- Kiểm tra state trước khi xử lý.
- Dùng signal riêng.

## Shell gây lỗi kiến trúc

Nguyên nhân:

- Shell sửa trực tiếp state task khác.
- Shell gọi hardware driver trực tiếp.
- Shell chạy command quá dài.

Giải pháp:

- Shell chỉ parse.
- Post command message.
- Task owner thực thi.

---

# 36. Bài thực hành

## Bài 1 — Đọc cấu trúc AK

Đọc và giải thích:

```text
ak.h
task.h
message.h
timer.h
fsm.h
tsm.h
```

Trả lời:

- Task được lưu bằng cấu trúc nào?
- Có bao nhiêu mức priority?
- Có mấy loại message?
- Timer có mấy loại?
- FSM và TSM khác gì nhau?

## Bài 2 — Tạo hai task

Tạo:

```text
Task A
Task B
```

Task A nhận `START_SIG` và gửi `HELLO_SIG` tới Task B.

Task B in log khi nhận message.

## Bài 3 — Pure Message

Gửi các signal:

```text
LED_ON_SIG
LED_OFF_SIG
LED_TOGGLE_SIG
```

Không có payload.

## Bài 4 — Common Message

Gửi struct:

```c
typedef struct
{
    uint16_t x;
    uint16_t y;
    uint8_t health;
} player_status_t;
```

Task nhận phải kiểm tra `len`.

## Bài 5 — Dynamic Message

Gửi chuỗi dài hơn common message.

Theo dõi:

```c
get_dynamic_msg_pool_used_max();
```

## Bài 6 — One-shot Timer

Button debounce 20 ms không dùng delay.

## Bài 7 — Periodic Timer

LED heartbeat 1 giây.

Cho phép shell thay đổi chu kỳ:

```text
life 100
life 500
life 1000
```

## Bài 8 — FSM

Tạo state:

```text
IDLE
RUNNING
PAUSED
ERROR
```

Signal:

```text
START
PAUSE
RESUME
STOP
ERROR
RESET
```

## Bài 9 — TSM

Viết lại State Machine bài 8 bằng transition table.

So sánh độ dễ đọc.

## Bài 10 — Command Line

Command:

```text
help
task
pool
timer
state
led
```

Mọi thao tác application phải đi qua message.

## Bài 11 — Pool Stress Test

Tạo burst message.

Đo:

- Message post thành công.
- Message post thất bại.
- Pool used max.
- Handler latency.

## Bài 12 — Event Record

Lưu 64 event cuối cùng.

Command:

```text
event dump
event clear
```

## Bài 13 — Fatal Record

Mô phỏng lỗi:

```text
fatal test
```

Sau reset, shell phải đọc được:

- Error code.
- Task ID.
- Timestamp.
- Restart count.

## Bài 14 — Mini Game

Tạo game đơn giản gồm:

```text
Input Task
Game Task
Display Task
Audio Task
Shell Task
```

Yêu cầu:

- Không blocking delay.
- Game tick bằng timer.
- Input bằng event.
- State Machine cho MENU/PLAY/GAME_OVER.
- Record event.
- Có command debug.

---

# 37. Tiêu chí hoàn thành

- Giải thích được Active Kernel.
- Phân biệt AK với RTOS preemptive.
- Hiểu run-to-completion.
- Tạo được `app_task_table`.
- Chọn được task priority hợp lý.
- Định nghĩa signal rõ ràng.
- Gửi được Pure Message.
- Gửi được Common Message.
- Gửi được Dynamic Message.
- Kiểm tra payload length.
- Hiểu message pool.
- Theo dõi pool high-water mark.
- Hiểu reference count.
- Không giữ pointer message sai lifetime.
- Tạo được one-shot timer.
- Tạo được periodic timer.
- Remove timer khi không dùng.
- Viết được FSM.
- Viết được TSM.
- ISR chỉ làm xử lý tối thiểu.
- Biết sử dụng `task_entry_interrupt()` và `task_exit_interrupt()`.
- Tạo được shell command.
- Tạo được event trace.
- Ghi được fatal record.
- Hoàn thành project demo chạy ổn định.
- Có tài liệu kiến trúc và signal list trên GitHub.

---

# 38. Cấu trúc repository đề xuất

```text
03-active-kernel/
├── README.md
├── Makefile
├── application/
│   ├── app.cpp
│   ├── app.h
│   ├── task_list.cpp
│   ├── task_list.h
│   ├── task_app.cpp
│   ├── task_app.h
│   ├── task_button.cpp
│   ├── task_button.h
│   ├── task_led.cpp
│   ├── task_led.h
│   ├── task_shell.cpp
│   └── task_shell.h
├── ak/
│   ├── inc/
│   │   ├── ak.h
│   │   ├── task.h
│   │   ├── message.h
│   │   ├── timer.h
│   │   ├── fsm.h
│   │   └── tsm.h
│   └── src/
│       ├── task.c
│       ├── message.c
│       ├── timer.c
│       ├── fsm.c
│       └── tsm.c
├── driver/
│   ├── button.c
│   ├── led.c
│   └── uart.c
├── platform/
│   ├── interrupt.c
│   ├── systick.c
│   └── critical_section.c
├── tests/
│   ├── test_task_app.cpp
│   ├── test_fsm.cpp
│   ├── test_timer.cpp
│   └── test_message.cpp
└── docs/
    ├── architecture.md
    ├── task-list.md
    ├── signal-list.md
    ├── state-machines.md
    ├── event-sequences.md
    └── memory-pools.md
```

## README của project nên có

- Board và MCU.
- Phiên bản AK hoặc commit đang sử dụng.
- Sơ đồ kiến trúc.
- Danh sách task.
- Priority của từng task.
- Danh sách signal.
- Loại message cho từng signal.
- Pool configuration.
- Timer configuration.
- State diagrams.
- Cách build.
- Cách flash.
- Cách mở shell.
- Danh sách command.
- Event trace mẫu.
- Stress-test result.
- Pool high-water mark.
- Lỗi đã gặp và cách xử lý.

---

# 39. Tài liệu tham khảo

## AK Embedded Base Kit — STM32L151

- https://github.com/the-ak-foundation/ak-base-kit-stm32l151

## Các file lõi cần đọc

```text
application/sources/ak/inc/ak.h
application/sources/ak/inc/task.h
application/sources/ak/inc/message.h
application/sources/ak/inc/timer.h
application/sources/ak/inc/fsm.h
application/sources/ak/inc/tsm.h

application/sources/ak/src/task.c
application/sources/ak/src/message.c
application/sources/ak/src/timer.c
application/sources/ak/src/fsm.c
application/sources/ak/src/tsm.c
```

## Các file application mẫu cần đọc

```text
application/sources/app/app.cpp
application/sources/app/task_list.cpp
application/sources/app/shell.cpp
application/sources/app/task_shell.cpp
```

## Tutorial

- https://epcb.vn/blogs/ak-embedded-software

---

# Tổng kết

Active Kernel cung cấp các thành phần giúp hiện thực hóa firmware hướng sự kiện:

```text
Task
  +
Signal
  +
Message Pool
  +
Priority Queue
  +
Timer
  +
State Machine
  +
Debug Tools
```

Luồng cốt lõi:

```text
Event Source
    |
    v
Create Message
    |
    v
Post to Destination Task
    |
    v
Scheduler selects message
    |
    v
Task handler runs to completion
    |
    v
State transition / new message / timer
```

Các nguyên tắc quan trọng nhất:

1. Task là event handler, không phải thread chạy vô hạn.
2. Task handler phải chạy nhanh và trả quyền cho kernel.
3. Không dùng delay blocking.
4. Dùng timer để biểu diễn thời gian.
5. Pure Message dùng cho event không payload.
6. Common Message dùng cho payload nhỏ, cố định.
7. Dynamic Message chỉ dùng khi thực sự cần.
8. Pool là tài nguyên hữu hạn và phải được đo.
9. Mỗi hardware resource cần một task owner.
10. ISR chỉ lấy dữ liệu tối thiểu và post event.
11. State Machine quản lý hành vi theo trạng thái.
12. Shell phải giao tiếp với application qua message.
13. Event trace và fatal record phải được thiết kế từ đầu.
14. Không tăng pool để che giấu handler chậm hoặc event storm.
15. Hiểu AK để có thể tự xây dựng hoặc thay đổi framework Event-Driven sau này.

> Lưu ý: tên API và cấu hình có thể thay đổi giữa các nhánh hoặc commit. Khi áp dụng vào project, hãy đối chiếu trực tiếp các header `task.h`, `message.h`, `timer.h`, `fsm.h`, `tsm.h` trong phiên bản repository đang sử dụng.
