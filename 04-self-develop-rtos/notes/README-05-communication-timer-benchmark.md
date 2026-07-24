# Chủ đề 5 — Communication, Software Timer và Benchmark
## Message Passing, Timer Service và Kernel Performance

> README này được xây dựng theo phạm vi **Chủ đề 5 — “Communication + Timer + Benchmark”** trong chương trình tự phát triển AKOS.

---

## Mục lục

1. [Mục tiêu học tập](#1-mục-tiêu-học-tập)
2. [Phạm vi của chủ đề](#2-phạm-vi-của-chủ-đề)
3. [Kiến thức cần chuẩn bị](#3-kiến-thức-cần-chuẩn-bị)
4. [Communication trong RTOS là gì?](#4-communication-trong-rtos-là-gì)
5. [Shared memory và message passing](#5-shared-memory-và-message-passing)
6. [Tại sao message passing hữu ích?](#6-tại-sao-message-passing-hữu-ích)
7. [Các mô hình truyền thông cơ bản](#7-các-mô-hình-truyền-thông-cơ-bản)
8. [Queue](#8-queue)
9. [Mailbox](#9-mailbox)
10. [Direct task notification](#10-direct-task-notification)
11. [Event flag và message khác nhau thế nào?](#11-event-flag-và-message-khác-nhau-thế-nào)
12. [Message là dữ liệu hay là sự kiện?](#12-message-là-dữ-liệu-hay-là-sự-kiện)
13. [Message ownership](#13-message-ownership)
14. [Copy-by-value và pointer message](#14-copy-by-value-và-pointer-message)
15. [Zero-copy communication](#15-zero-copy-communication)
16. [Message lifetime](#16-message-lifetime)
17. [Queue storage model](#17-queue-storage-model)
18. [Static queue allocation](#18-static-queue-allocation)
19. [Ring buffer](#19-ring-buffer)
20. [Queue indices và count](#20-queue-indices-và-count)
21. [Queue invariants](#21-queue-invariants)
22. [Queue send và receive](#22-queue-send-và-receive)
23. [Blocking send](#23-blocking-send)
24. [Blocking receive](#24-blocking-receive)
25. [No-wait và finite timeout](#25-no-wait-và-finite-timeout)
26. [Wait forever](#26-wait-forever)
27. [Sender wait list và receiver wait list](#27-sender-wait-list-và-receiver-wait-list)
28. [Direct handoff](#28-direct-handoff)
29. [Queue full và queue empty](#29-queue-full-và-queue-empty)
30. [Wake-up policy](#30-wake-up-policy)
31. [Priority ordering và FIFO](#31-priority-ordering-và-fifo)
32. [Queue from ISR](#32-queue-from-isr)
33. [Không được block trong ISR](#33-không-được-block-trong-isr)
34. [Message pool](#34-message-pool)
35. [Fixed-block memory pool](#35-fixed-block-memory-pool)
36. [Pool ownership và double free](#36-pool-ownership-và-double-free)
37. [Software timer là gì?](#37-software-timer-là-gì)
38. [Software timer và hardware timer](#38-software-timer-và-hardware-timer)
39. [Software timer và task delay](#39-software-timer-và-task-delay)
40. [One-shot timer](#40-one-shot-timer)
41. [Periodic timer](#41-periodic-timer)
42. [Timer state machine](#42-timer-state-machine)
43. [Timer expiry tick](#43-timer-expiry-tick)
44. [Timer list](#44-timer-list)
45. [Sorted timer list](#45-sorted-timer-list)
46. [Delta list](#46-delta-list)
47. [Timing wheel](#47-timing-wheel)
48. [Timer service task](#48-timer-service-task)
49. [Timer callback contract](#49-timer-callback-contract)
50. [Không chạy callback dài trong SysTick](#50-không-chạy-callback-dài-trong-systick)
51. [Timer command queue](#51-timer-command-queue)
52. [Start, stop và restart timer](#52-start-stop-và-restart-timer)
53. [Timer cancel race](#53-timer-cancel-race)
54. [Periodic timer drift](#54-periodic-timer-drift)
55. [Timer overrun](#55-timer-overrun)
56. [SysTick trong software timer](#56-systick-trong-software-timer)
57. [Benchmark là gì?](#57-benchmark-là-gì)
58. [Benchmark khác profiling thế nào?](#58-benchmark-khác-profiling-thế-nào)
59. [Các đại lượng cần đo](#59-các-đại-lượng-cần-đo)
60. [Context-switch latency](#60-context-switch-latency)
61. [Event latency](#61-event-latency)
62. [Message latency](#62-message-latency)
63. [Interrupt-to-task latency](#63-interrupt-to-task-latency)
64. [Timer expiry latency](#64-timer-expiry-latency)
65. [Queue throughput](#65-queue-throughput)
66. [Jitter](#66-jitter)
67. [Worst-case và average](#67-worst-case-và-average)
68. [Percentile](#68-percentile)
69. [Timestamp backend](#69-timestamp-backend)
70. [DWT CYCCNT](#70-dwt-cyccnt)
71. [General-purpose timer](#71-general-purpose-timer)
72. [GPIO pulse và logic analyzer](#72-gpio-pulse-và-logic-analyzer)
73. [Benchmark overhead](#73-benchmark-overhead)
74. [Warm-up và cache](#74-warm-up-và-cache)
75. [Cortex-M3 và cache note](#75-cortex-m3-và-cache-note)
76. [Interrupt masking và benchmark](#76-interrupt-masking-và-benchmark)
77. [Benchmark reproducibility](#77-benchmark-reproducibility)
78. [Histogram](#78-histogram)
79. [Trace buffer](#79-trace-buffer)
80. [Mã khung queue](#80-mã-khung-queue)
81. [Mã khung message pool](#81-mã-khung-message-pool)
82. [Mã khung software timer](#82-mã-khung-software-timer)
83. [Mã khung timer service](#83-mã-khung-timer-service)
84. [Mã khung timestamp](#84-mã-khung-timestamp)
85. [Mã khung benchmark statistics](#85-mã-khung-benchmark-statistics)
86. [Chiến lược kiểm thử](#86-chiến-lược-kiểm-thử)
87. [Lỗi thường gặp](#87-lỗi-thường-gặp)
88. [Bài thực hành](#88-bài-thực-hành)
89. [Project tổng kết chủ đề](#89-project-tổng-kết-chủ-đề)
90. [Cấu trúc repository đề xuất](#90-cấu-trúc-repository-đề-xuất)
91. [Liên hệ với roadmap HairRTOS](#91-liên-hệ-với-roadmap-hairtos)
92. [Tiêu chí hoàn thành](#92-tiêu-chí-hoàn-thành)
93. [Tổng kết](#93-tổng-kết)

---

# 1. Mục tiêu học tập

Sau khi hoàn thành chủ đề này, người học cần có khả năng:

- Giải thích được message passing giữa các task.
- Phân biệt:
  - shared memory;
  - queue;
  - mailbox;
  - event flag;
  - direct notification.
- Thiết kế static message queue bằng ring buffer.
- Giải thích được queue ownership và message lifetime.
- Phân biệt copy-by-value và pointer message.
- Cài đặt:
  - send;
  - receive;
  - no-wait;
  - finite timeout;
  - wait forever.
- Quản lý sender wait list và receiver wait list.
- Wake task theo effective priority và FIFO giữa các task cùng priority.
- Thiết kế ISR-safe queue API.
- Giải thích được fixed-block message pool.
- Phát hiện double allocation, double free và use-after-free.
- Giải thích software timer.
- Phân biệt:
  - one-shot timer;
  - periodic timer;
  - task delay;
  - hardware timer.
- Tổ chức software timer bằng sorted list.
- Thiết kế timer service task.
- Giải thích callback contract.
- Xử lý start, stop, restart và cancel race.
- Giải thích benchmark kernel performance.
- Đo:
  - context-switch latency;
  - event latency;
  - message latency;
  - interrupt-to-task latency;
  - timer expiry latency;
  - queue throughput;
  - jitter.
- Sử dụng DWT cycle counter, general-purpose timer hoặc GPIO pulse để timestamp.
- Giảm ảnh hưởng của instrumentation lên kết quả.
- Tính:
  - minimum;
  - maximum;
  - average;
  - variance cơ bản;
  - histogram;
  - percentile.
- Viết host tests cho queue, pool, timer và statistics.
- Chạy benchmark project trên STM32F103.
- Ghi lại điều kiện đo để kết quả có thể tái lập.

---

# 2. Phạm vi của chủ đề

Theo chương trình AKOS, Chủ đề 5 gồm:

```text
Communication + Timer + Benchmark
    |
    +-- Hệ thống truyền tin nhắn trong AK-mOS
    +-- Cơ chế hoạt động của message system
    +-- Software timer và SysTick
    +-- Quản lý timer trong AK-mOS
    +-- Benchmark AK-mOS trên Cortex-M3
        |
        +-- Context-switch latency
        +-- Event/message latency
```

README này mở rộng thành lộ trình:

```text
Message-passing models
        |
        v
Static queue
        |
        v
Blocking send/receive
        |
        v
ISR communication
        |
        v
Message pool
        |
        v
Software timer
        |
        v
Timer service task
        |
        v
Timestamp backend
        |
        v
Latency + throughput benchmark
        |
        v
Kernel Benchmark Console
```

## Ranh giới của chủ đề

Chủ đề này tập trung vào:

- task-to-task communication;
- queue;
- mailbox;
- fixed-block message pool;
- software timer;
- timer service;
- benchmark;
- latency;
- throughput;
- jitter;
- statistics;
- target measurement.

Chủ đề này không tập trung vào:

- dynamic general-purpose heap;
- networking stack;
- CAN protocol stack;
- file system;
- multicore scheduling;
- full tracing framework;
- certification-grade WCET analysis.

---

# 3. Kiến thức cần chuẩn bị

## 3.1 Từ Chủ đề 1

- Static allocation.
- First-fit allocator.
- Fragmentation.
- Memory ownership.
- Stack.
- Linker map.
- Host sanitizer.

## 3.2 Từ Chủ đề 2

- Scheduler.
- SVC.
- PendSV.
- SysTick.
- Context switch.
- Preemption.
- ISR-to-task wake-up.

## 3.3 Từ Chủ đề 3

- Intrusive list.
- TCB.
- Ready queue.
- Ready bitmap.
- List ownership.
- Registry.

## 3.4 Từ Chủ đề 4

- BLOCKED state.
- Delayed list.
- Timeout.
- Semaphore.
- Mutex.
- ISR-safe API.
- Single-winner wake-up.

---

# 4. Communication trong RTOS là gì?

Communication là cách các execution context trao đổi dữ liệu hoặc tín hiệu.

Các context có thể là:

```text
Task -> Task
ISR  -> Task
Task -> ISR-facing driver state
Timer service -> Task
```

Ví dụ:

```text
ADC ISR
    |
    v
Message queue
    |
    v
Signal-processing task
```

Hoặc:

```text
Sensor task
    |
    v
Mailbox
    |
    v
Control task
```

Communication cần trả lời:

- dữ liệu nằm ở đâu;
- ai sở hữu dữ liệu;
- ai được sửa;
- khi queue đầy thì làm gì;
- khi queue rỗng thì làm gì;
- có timeout không;
- ISR có được dùng không;
- task nào được wake trước;
- dữ liệu còn hợp lệ tới khi nào.

---

# 5. Shared memory và message passing

## Shared memory

Nhiều task cùng truy cập object.

```c
shared_state.temperature = value;
```

Cần:

- mutex;
- critical section;
- atomic;
- ownership rule.

## Message passing

Producer gửi message cho consumer.

```text
Producer
    |
    v
Queue
    |
    v
Consumer
```

Message passing giảm coupling vì consumer không cần truy cập trực tiếp internal state của producer.

---

# 6. Tại sao message passing hữu ích?

- Tách producer và consumer.
- Chuyển ownership rõ hơn.
- Hỗ trợ buffering.
- Hỗ trợ blocking.
- Tự nhiên với event-driven system.
- Dễ trace.
- Dễ kiểm thử.
- Hạn chế shared mutable state.

Message passing không tự động loại bỏ mọi race. Queue implementation vẫn cần atomicity.

---

# 7. Các mô hình truyền thông cơ bản

```text
Queue
Mailbox
Direct notification
Event flags
Shared buffer + semaphore
Message pool + pointer queue
```

Lựa chọn dựa trên:

- payload size;
- số producer;
- số consumer;
- có cần lưu nhiều message không;
- copy cost;
- lifetime;
- ISR usage;
- latency;
- memory budget.

---

# 8. Queue

Queue lưu nhiều item theo FIFO.

```text
Producer -> [M1][M2][M3] -> Consumer
```

Đặc trưng:

- fixed capacity;
- fixed item size trong implementation cơ bản;
- send tail;
- receive head;
- có thể block khi full/empty.

---

# 9. Mailbox

Mailbox thường lưu một message hoặc một slot.

```text
Producer -> [latest value] -> Consumer
```

Policy có thể:

- reject nếu đầy;
- overwrite old value;
- block producer;
- replace latest.

Mailbox phù hợp:

- latest sensor state;
- configuration update;
- one outstanding command.

---

# 10. Direct task notification

Direct notification gắn signal trực tiếp vào TCB.

Ưu điểm:

- nhanh;
- ít memory;
- không cần queue object riêng.

Nhược điểm:

- coupling với một task;
- payload hạn chế;
- khó multiple consumers;
- API và semantics cần rõ.

Không phải phạm vi bắt buộc của Chủ đề 5, nhưng nên hiểu để so sánh.

---

# 11. Event flag và message khác nhau thế nào?

Event flag:

```text
bit 0 -> RX ready
bit 1 -> timeout
bit 2 -> error
```

Không chứa payload phức tạp.

Message:

```text
type + length + data
```

Event flag phù hợp báo trạng thái. Message phù hợp chuyển dữ liệu.

---

# 12. Message là dữ liệu hay là sự kiện?

Message có thể biểu diễn cả hai.

Ví dụ event-only:

```c
typedef enum
{
    MSG_BUTTON_PRESSED,
    MSG_TIMEOUT
} message_type_t;
```

Ví dụ payload:

```c
typedef struct
{
    message_type_t type;
    uint32_t timestamp;

    union
    {
        uint16_t adc;
        int32_t temperature;
    } payload;
} message_t;
```

---

# 13. Message ownership

Ownership cần trả lời:

```text
Ai tạo?
Ai được sửa?
Ai giải phóng?
Khi nào ownership chuyển?
```

Ví dụ copy-by-value:

```text
Producer owns local object before send.
Queue owns copied bytes after send.
Consumer owns received copy.
```

Pointer message:

```text
Producer allocates block.
Ownership transfers to queue on successful send.
Ownership transfers to consumer on receive.
Consumer returns block to pool.
```

---

# 14. Copy-by-value và pointer message

## Copy-by-value

Queue copy `item_size` bytes.

Ưu điểm:

- lifetime đơn giản;
- producer có thể reuse local buffer sau send;
- không double free.

Nhược điểm:

- copy cost;
- queue storage lớn;
- item size cố định.

## Pointer message

Queue lưu pointer.

Ưu điểm:

- ít copy;
- payload lớn;
- zero-copy gần đúng.

Nhược điểm:

- ownership phức tạp;
- pool cần thiết;
- pointer lifetime;
- double free/use-after-free.

---

# 15. Zero-copy communication

Zero-copy thường có nghĩa:

- producer lấy block từ pool;
- điền data;
- gửi pointer;
- consumer xử lý;
- consumer trả block.

Không có nghĩa hoàn toàn không có copy ở mọi layer.

Metadata và pointer vẫn được copy.

---

# 16. Message lifetime

Sai:

```c
void producer(void)
{
    message_t message;
    queue_send(&queue, &message_pointer);
}
```

nếu queue chỉ lưu pointer tới stack local và producer tiếp tục.

Đúng:

- copy-by-value;
- static object với ownership rõ;
- memory pool block;
- immutable shared object có reference count.

---

# 17. Queue storage model

Static queue:

```c
typedef struct
{
    uint8_t *storage;
    size_t item_size;
    size_t capacity;

    size_t head;
    size_t tail;
    size_t count;
} hr_queue_t;
```

Storage do application cấp:

```c
static uint8_t queue_storage[
    QUEUE_CAPACITY * sizeof(message_t)];
```

---

# 18. Static queue allocation

Creation API:

```c
hr_status_t hr_queue_init_static(
    hr_queue_t *queue,
    void *storage,
    size_t capacity,
    size_t item_size);
```

Validate:

- queue không NULL;
- storage không NULL;
- capacity > 0;
- item_size > 0;
- multiplication không overflow;
- alignment nếu copy typed object cần policy.

---

# 19. Ring buffer

Queue dùng circular indices:

```text
capacity = 4

[0][1][2][3]

tail wraps:
3 -> 0
```

Index update:

```c
index = (index + 1U) % capacity;
```

Có thể tránh modulo bằng:

```c
index++;
if (index == capacity)
{
    index = 0U;
}
```

---

# 20. Queue indices và count

Model:

```text
head  -> item receive tiếp theo
tail  -> slot send tiếp theo
count -> số item đang lưu
```

Empty:

```text
count == 0
```

Full:

```text
count == capacity
```

Dùng `count` giúp phân biệt full và empty khi `head == tail`.

---

# 21. Queue invariants

- `capacity > 0`.
- `item_size > 0`.
- `head < capacity`.
- `tail < capacity`.
- `count <= capacity`.
- empty khi count 0.
- full khi count capacity.
- receiver waiters chỉ tồn tại theo policy khi queue empty.
- sender waiters chỉ tồn tại theo policy khi queue full.
- waiting task state BLOCKED.
- waiting object trỏ queue.
- queue memory không overlap invalid.

---

# 22. Queue send và receive

Send immediate:

```text
queue not full
    |
    +-- copy item to tail
    +-- advance tail
    +-- count++
    +-- wake receiver if policy requires
```

Receive immediate:

```text
queue not empty
    |
    +-- copy item from head
    +-- advance head
    +-- count--
    +-- wake sender if policy requires
```

---

# 23. Blocking send

Queue full:

```text
NO_WAIT -> WOULD_BLOCK
finite timeout -> block sender
WAIT_FOREVER -> block sender
```

Sender cần giữ source item ở đâu?

Hai design:

## Design A — Copy pending item vào TCB/pending storage

Phức tạp vì item size thay đổi.

## Design B — Khi wake, send API thử lại

Task call stack giữ pointer/source.

Cần bảo đảm source còn hợp lệ trong suốt block.

## Design C — Sender wait entry có fixed staging slot

Tốn RAM.

HairRTOS nên document rõ design đã chọn.

---

# 24. Blocking receive

Queue empty:

```text
NO_WAIT -> WOULD_BLOCK
finite timeout -> block receiver
WAIT_FOREVER -> block receiver
```

Khi wake:

- message đã được direct handoff;
- hoặc receiver retry dequeue.

---

# 25. No-wait và finite timeout

```c
#define HR_NO_WAIT      (0U)
#define HR_WAIT_FOREVER (UINT32_MAX)
```

Finite timeout phải nhỏ hơn maximum horizon nếu dùng signed tick difference.

---

# 26. Wait forever

Task không vào timeout list.

Task chỉ nằm:

- queue sender wait list;
- hoặc receiver wait list.

---

# 27. Sender wait list và receiver wait list

Queue có:

```c
hr_list_t send_waiters;
hr_list_t receive_waiters;
```

TCB dùng:

```text
wait_node
timeout_node
```

Finite waiter đồng thời nằm object wait list và timeout list bằng hai node khác nhau.

---

# 28. Direct handoff

Nếu receiver đang chờ và producer gửi:

```text
Producer copies directly to receiver buffer
Receiver wake
Queue count không tăng
```

Ưu điểm:

- giảm copy/storage;
- latency thấp.

Nhược điểm:

- receiver buffer pointer phải được lưu an toàn;
- object wait metadata phức tạp;
- ISR copy size cần bounded.

Phiên bản đầu có thể không direct handoff; send vào queue rồi wake receiver.

---

# 29. Queue full và queue empty

Full policy:

- block;
- return error;
- overwrite oldest;
- drop newest.

RTOS generic queue thường không overwrite ngầm.

Mailbox/latest-value object có thể chọn overwrite.

---

# 30. Wake-up policy

Khi queue có item:

```text
wake receiver effective priority cao nhất
FIFO among equals
```

Khi queue có slot:

```text
wake sender effective priority cao nhất
FIFO among equals
```

Nếu woken task priority cao hơn current:

```text
PendSV
```

---

# 31. Priority ordering và FIFO

Wait list ordering:

```text
priority ascending numeric
sequence ascending among equal priority
```

Không reorder message FIFO theo task priority. Task wake ordering và message ordering là hai vấn đề khác nhau.

---

# 32. Queue from ISR

API:

```c
hr_status_t hr_queue_send_from_isr(
    hr_queue_t *queue,
    const void *item,
    bool *higher_priority_task_woken);
```

Constraints:

- no blocking;
- bounded copy;
- item size hợp lý;
- no dynamic allocation;
- defer switch;
- critical section ISR-safe.

---

# 33. Không được block trong ISR

ISR không có task stack/lifecycle để chờ.

Không gọi:

```c
hr_queue_send(&queue,
              item,
              HR_WAIT_FOREVER);
```

từ ISR.

---

# 34. Message pool

Message pool cung cấp fixed-size blocks.

```text
Pool:
[free][free][used][free]
```

API:

```c
void *hr_message_pool_alloc(
    hr_message_pool_t *pool);

bool hr_message_pool_free(
    hr_message_pool_t *pool,
    void *block);
```

---

# 35. Fixed-block memory pool

Ưu điểm:

- O(1) alloc/free;
- không external fragmentation;
- bounded behavior;
- dễ dùng từ task;
- có thể có ISR-safe variant nếu lock policy phù hợp.

Nhược điểm:

- block size cố định;
- internal fragmentation;
- cần ownership nghiêm ngặt.

---

# 36. Pool ownership và double free

Block state debug:

```text
FREE
ALLOCATED
QUEUED
CONSUMED
```

Có thể dùng bitmap hoặc magic.

Phát hiện:

- pointer ngoài pool;
- misaligned block pointer;
- double free;
- free block đang queued;
- pool corruption.

---

# 37. Software timer là gì?

Software timer là object do kernel quản lý để tạo action sau một khoảng thời gian.

```text
Start timer
    |
    v
Wait for expiry tick
    |
    v
Timer expires
    |
    v
Callback or message
```

Không cần một hardware timer riêng cho mỗi software timer.

---

# 38. Software timer và hardware timer

## Hardware timer

Peripheral thật:

- counter;
- compare;
- capture;
- PWM;
- IRQ.

## Software timer

Kernel object dựa trên:

- SysTick;
- hardware timer base;
- timer list;
- service task.

Nhiều software timer dùng chung một time base.

---

# 39. Software timer và task delay

Task delay:

```text
Block một task cụ thể
```

Software timer:

```text
Trigger callback hoặc message
không nhất thiết gắn với task đã start timer
```

Task delay không cần callback object.

---

# 40. One-shot timer

Expire một lần rồi inactive.

```text
INACTIVE -> ACTIVE -> EXPIRED -> INACTIVE
```

---

# 41. Periodic timer

Sau expiry tự lên lịch lần tiếp theo.

```text
expiry = previous_expiry + period
```

Dùng absolute schedule để giảm drift.

---

# 42. Timer state machine

```text
INACTIVE
   |
   | start
   v
ACTIVE
   | \
   |  \ stop
   |   v
   | INACTIVE
   |
   | expiry
   v
CALLBACK_PENDING
   |
   | callback complete
   v
ACTIVE (periodic)
or
INACTIVE (one-shot)
```

Có thể thêm:

```text
CANCEL_PENDING
```

nếu timer command bất đồng bộ.

---

# 43. Timer expiry tick

```text
expiry_tick = now + delay
```

Periodic:

```text
next_expiry = previous_expiry + period
```

Không dùng:

```text
next_expiry = callback_finish_time + period
```

nếu muốn giữ phase.

---

# 44. Timer list

Timer object chứa intrusive node:

```c
typedef struct hr_timer
{
    hr_list_node_t node;
    uint32_t expiry_tick;
    uint32_t period_ticks;
    hr_timer_callback_t callback;
    void *argument;
    hr_timer_state_t state;
} hr_timer_t;
```

---

# 45. Sorted timer list

Sort theo expiry.

SysTick hoặc timer service kiểm tra head.

Ưu điểm:

- đơn giản;
- phù hợp số timer nhỏ;
- expiry processing hiệu quả.

Insert O(n).

---

# 46. Delta list

Node lưu khoảng cách so với node trước.

```text
T1: 5
T2: +3
T3: +7
```

Mỗi tick chỉ giảm head.

Ưu điểm:

- tick operation đơn giản.

Nhược điểm:

- insert/remove phức tạp;
- wrap reasoning;
- diagnostics khó hơn.

---

# 47. Timing wheel

Buckets theo time slot.

Phù hợp:

- nhiều timer;
- bounded insertion;
- coarse timing.

Không bắt buộc cho HairRTOS phiên bản học tập.

---

# 48. Timer service task

Khuyến nghị:

```text
SysTick
    |
    +-- update time
    +-- detect due timer
    +-- signal timer service
    |
    v
Timer service task
    |
    +-- remove due timer
    +-- invoke callback
    +-- reschedule periodic timer
```

Callback chạy trong task context, không trong SysTick.

---

# 49. Timer callback contract

Callback:

- không chạy trong ISR nếu dùng service task;
- không được block quá lâu;
- không được gọi API timer gây deadlock theo policy;
- có stack của timer service;
- nên ngắn;
- có thể gửi message cho worker task.

Callback không nên thực hiện:

- long polling;
- flash erase dài;
- blocking I/O lâu;
- unbounded loop.

---

# 50. Không chạy callback dài trong SysTick

Nếu callback chạy trong SysTick:

- interrupt latency tăng;
- kernel tick jitter;
- PendSV bị trì hoãn;
- ISR nesting phức tạp;
- benchmark sai.

SysTick chỉ nên mark due và wake service task.

---

# 51. Timer command queue

Task khác gửi command:

```text
START
STOP
RESET
CHANGE_PERIOD
DELETE
```

Timer service xử lý command trong một context.

Ưu điểm:

- ownership rõ;
- giảm race;
- callback và command serialized.

Nhược điểm:

- command latency;
- queue memory;
- service task cần priority.

---

# 52. Start, stop và restart timer

Start inactive:

```text
state ACTIVE
expiry = now + delay
insert list
```

Stop active:

```text
remove list
state INACTIVE
```

Restart:

```text
remove if active
expiry = now + delay
insert
state ACTIVE
```

---

# 53. Timer cancel race

Race:

```text
Timer expires
Task calls stop
Callback pending
```

Policy cần rõ:

- stop trước dequeue due -> callback không chạy;
- stop sau callback bắt đầu -> không hủy callback đang chạy;
- stop callback pending -> cancel pending nếu state cho phép.

Cần state machine atomic.

---

# 54. Periodic timer drift

Absolute reschedule:

```text
next = previous + period
```

Nếu callback trễ, có các policy:

- catch up nhiều expiry;
- skip missed periods;
- run once rồi schedule future phase;
- count missed expiries.

HairRTOS cần document một policy.

---

# 55. Timer overrun

Callback chạy lâu hơn period.

Ví dụ:

```text
period 10 ms
callback 15 ms
```

Không thể giữ đúng tất cả expiry bằng một service task.

Cần:

- detect overrun;
- skip;
- count missed;
- offload work;
- tăng period.

---

# 56. SysTick trong software timer

SysTick cung cấp coarse tick.

Resolution:

```text
tick period
```

Timer 1 kHz có resolution khoảng 1 ms.

Timer yêu cầu microsecond precision không nên chỉ dựa SysTick 1 kHz.

---

# 57. Benchmark là gì?

Benchmark đo performance trong điều kiện được định nghĩa.

Một số đo không có ý nghĩa nếu thiếu:

- CPU clock;
- compiler;
- optimization;
- interrupt state;
- task priorities;
- message size;
- queue depth;
- sample count;
- timestamp method.

---

# 58. Benchmark khác profiling thế nào?

Benchmark:

```text
Đo một operation/scenario cụ thể
```

Profiling:

```text
Quan sát CPU time của nhiều function trong workload
```

Trace:

```text
Ghi timeline event
```

WCET analysis:

```text
Tìm upper bound có lập luận nghiêm ngặt
```

Benchmark không tự động chứng minh worst-case tuyệt đối.

---

# 59. Các đại lượng cần đo

- context-switch latency;
- event latency;
- message latency;
- ISR-to-task latency;
- timer expiry latency;
- queue throughput;
- allocation latency;
- critical-section duration;
- jitter.

---

# 60. Context-switch latency

Cần định nghĩa điểm bắt đầu và kết thúc.

Ví dụ:

```text
t0 = trước khi pend PendSV
t1 = instruction đầu tiên của next task
latency = t1 - t0
```

Hoặc đo pure PendSV:

```text
t0 = PendSV entry
t1 = PendSV exit/next task marker
```

Hai definition cho kết quả khác nhau.

---

# 61. Event latency

Ví dụ:

```text
Producer signals semaphore at t0
Consumer starts handling at t1
event latency = t1 - t0
```

Bao gồm:

- critical section;
- scheduler decision;
- PendSV;
- interrupt masking;
- higher-priority workload.

---

# 62. Message latency

```text
Producer sends message at t0
Consumer receives/handles at t1
```

Cần chọn:

- send call entry;
- send call success;
- enqueue complete;
- receive call return;
- handler start.

Message size ảnh hưởng copy cost.

---

# 63. Interrupt-to-task latency

```text
External edge
    |
    v
IRQ entry
    |
    v
send_from_isr
    |
    v
PendSV
    |
    v
Task starts
```

Đo bằng logic analyzer có thể đặt GPIO:

- GPIO A tại IRQ entry;
- GPIO B tại task handler.

---

# 64. Timer expiry latency

```text
ideal expiry tick/time
        |
        v
timer callback starts
```

Latency chịu ảnh hưởng:

- tick quantization;
- SysTick priority;
- service task priority;
- callback backlog;
- interrupt masking.

---

# 65. Queue throughput

```text
messages per second
bytes per second
```

Scenario cần nêu:

- item size;
- queue capacity;
- one producer/one consumer;
- priorities;
- copy-by-value hay pointer;
- blocking hay no-wait.

---

# 66. Jitter

Jitter là độ biến thiên timing.

Ví dụ periodic callback:

```text
expected interval = 1000 us
actual = 998, 1004, 1001, 1012...
```

Có thể report:

- min;
- max;
- peak-to-peak;
- standard deviation;
- percentile.

---

# 67. Worst-case và average

Average thấp không bảo đảm latency tối đa tốt.

RTOS quan tâm:

```text
maximum observed
distribution
tail latency
```

Không gọi maximum observed là proven WCET.

---

# 68. Percentile

Ví dụ:

```text
p50
p90
p99
p99.9
```

Với embedded RAM hạn chế, có thể:

- lưu fixed sample array nhỏ;
- histogram bins;
- online statistics;
- stream raw samples qua UART sau test.

Không in trong timing window.

---

# 69. Timestamp backend

Các lựa chọn:

- DWT CYCCNT;
- general-purpose timer;
- SysTick current value;
- GPIO + logic analyzer;
- external timer capture.

Mỗi backend có resolution và overhead khác nhau.

---

# 70. DWT CYCCNT

Cortex-M3 thường có DWT cycle counter trong debug/trace block, nhưng availability và behavior cần xác nhận trên target/debug configuration.

Mã khung:

```c
#define DEMCR      (*(volatile uint32_t *)0xE000EDFCUL)
#define DWT_CTRL   (*(volatile uint32_t *)0xE0001000UL)
#define DWT_CYCCNT (*(volatile uint32_t *)0xE0001004UL)

#define DEMCR_TRCENA (1UL << 24)
#define DWT_CYCCNTENA (1UL << 0)

void benchmark_clock_init(void)
{
    DEMCR |= DEMCR_TRCENA;
    DWT_CYCCNT = 0U;
    DWT_CTRL |= DWT_CYCCNTENA;
}
```

Cần xác nhận bằng reference manual/core documentation và test thật.

---

# 71. General-purpose timer

Timer peripheral có thể chạy ở MHz.

Ưu điểm:

- hoạt động không phụ thuộc debug block;
- có input capture;
- đo GPIO event.

Nhược điểm:

- cấu hình clock phức tạp;
- overflow;
- peripheral resource.

---

# 72. GPIO pulse và logic analyzer

Đo không phụ thuộc firmware timestamp:

```text
GPIO high at start
GPIO low at end
```

Logic analyzer đo pulse width.

Ưu điểm:

- quan sát ngoài;
- tốt cho ISR-to-task;
- thấy jitter.

Nhược điểm:

- GPIO write overhead;
- instrumented path;
- resolution thiết bị.

---

# 73. Benchmark overhead

Nếu đọc timestamp hai lần:

```text
measured = operation + timestamp overhead
```

Đo baseline:

```c
t0 = read();
t1 = read();
overhead = t1 - t0;
```

Có thể subtract hoặc report riêng.

Không subtract mù nếu overhead biến thiên.

---

# 74. Warm-up và cache

Một số MCU có cache/prefetch/branch effects.

Cần:

- warm-up iterations;
- bỏ sample đầu;
- cố định code placement nếu cần;
- ghi compiler optimization.

---

# 75. Cortex-M3 và cache note

STM32F103 không có data cache như MCU cao cấp. Tuy nhiên vẫn có:

- Flash wait states;
- prefetch buffer;
- bus contention;
- interrupt effects.

Không nên viết chung rằng “không có cache nên mọi lần chạy giống nhau”.

---

# 76. Interrupt masking và benchmark

Context-switch latency thay đổi nếu:

- interrupt disabled;
- higher-priority ISR chạy;
- SysTick pending;
- nested interrupt.

Benchmark cần scenario:

```text
interrupts enabled
no external IRQ load
```

hoặc:

```text
with controlled IRQ load
```

---

# 77. Benchmark reproducibility

Report tối thiểu:

```text
MCU:
CPU clock:
Flash wait states:
Compiler:
Compiler version:
Optimization:
Linker:
Kernel configuration:
Tick rate:
Task priorities:
Queue depth:
Message size:
Sample count:
Timestamp source:
Interrupt conditions:
Result units:
```

---

# 78. Histogram

Fixed bins:

```c
typedef struct
{
    uint32_t upper_bound_cycles;
    uint32_t count;
} benchmark_bin_t;
```

Histogram giúp thấy tail mà không lưu mọi sample.

---

# 79. Trace buffer

Record:

```c
typedef struct
{
    uint32_t timestamp;
    uint16_t event;
    uint16_t object_id;
    uint32_t value;
} trace_record_t;
```

Static ring buffer.

Nếu full:

- overwrite old;
- stop recording;
- increment lost count.

Policy cần rõ.

---

# 80. Mã khung queue

```c
typedef struct
{
    uint8_t *storage;
    size_t capacity;
    size_t item_size;

    size_t head;
    size_t tail;
    size_t count;

    hr_list_t send_waiters;
    hr_list_t receive_waiters;
} hr_queue_t;
```

Immediate enqueue:

```c
static void hr_queue_copy_in(
    hr_queue_t *queue,
    const void *item)
{
    uint8_t *destination =
        queue->storage +
        (queue->tail * queue->item_size);

    hr_memory_copy(
        destination,
        item,
        queue->item_size);

    queue->tail++;

    if (queue->tail == queue->capacity)
    {
        queue->tail = 0U;
    }

    queue->count++;
}
```

---

# 81. Mã khung message pool

```c
typedef struct hr_pool_block
{
    struct hr_pool_block *next;
} hr_pool_block_t;

typedef struct
{
    uint8_t *memory;
    size_t block_size;
    size_t block_count;

    hr_pool_block_t *free_list;
    size_t free_count;
} hr_message_pool_t;
```

Init cần align block size đủ chứa pointer.

---

# 82. Mã khung software timer

```c
typedef void (*hr_timer_callback_t)(
    void *argument);

typedef enum
{
    HR_TIMER_INACTIVE = 0,
    HR_TIMER_ACTIVE,
    HR_TIMER_CALLBACK_PENDING
} hr_timer_state_t;

typedef struct hr_timer
{
    hr_list_node_t node;

    const char *name;
    uint32_t expiry_tick;
    uint32_t period_ticks;

    hr_timer_callback_t callback;
    void *argument;

    hr_timer_state_t state;
    bool periodic;
} hr_timer_t;
```

---

# 83. Mã khung timer service

```c
static void hr_timer_service_task(
    void *argument)
{
    (void)argument;

    for (;;)
    {
        hr_semaphore_take(
            &g_timer_service_signal,
            HR_WAIT_FOREVER);

        for (;;)
        {
            hr_timer_t *timer =
                hr_timer_take_due();

            if (timer == NULL)
            {
                break;
            }

            timer->state =
                HR_TIMER_CALLBACK_PENDING;

            timer->callback(
                timer->argument);

            hr_timer_complete_expiry(
                timer);
        }
    }
}
```

Callback chạy ngoài timer-list critical section.

---

# 84. Mã khung timestamp

```c
typedef uint32_t benchmark_timestamp_t;

static inline benchmark_timestamp_t
benchmark_now(void)
{
    return DWT_CYCCNT;
}

static inline uint32_t benchmark_elapsed(
    benchmark_timestamp_t start,
    benchmark_timestamp_t end)
{
    return end - start;
}
```

Unsigned subtraction xử lý một lần wrap nếu interval nhỏ hơn full range.

---

# 85. Mã khung benchmark statistics

```c
typedef struct
{
    uint32_t count;
    uint32_t minimum;
    uint32_t maximum;
    uint64_t sum;
} benchmark_stats_t;
```

Update:

```c
void benchmark_stats_add(
    benchmark_stats_t *stats,
    uint32_t sample)
{
    if (stats->count == 0U)
    {
        stats->minimum = sample;
        stats->maximum = sample;
    }
    else
    {
        if (sample < stats->minimum)
        {
            stats->minimum = sample;
        }

        if (sample > stats->maximum)
        {
            stats->maximum = sample;
        }
    }

    stats->sum += sample;
    stats->count++;
}
```

Average:

```c
uint32_t average =
    (uint32_t)(stats->sum / stats->count);
```

Kiểm tra overflow của `sum` theo sample count.

---

# 86. Chiến lược kiểm thử

## Host tests

- ring buffer wrap;
- queue full/empty;
- item copy;
- sender/receiver timeout;
- waiter ordering;
- direct handoff nếu có;
- message pool;
- double free;
- timer ordering;
- start/stop/restart;
- periodic reschedule;
- cancel race state;
- benchmark statistics;
- histogram.

## Target tests

- queue task-to-task;
- queue from ISR;
- timer service;
- periodic timer drift;
- context-switch latency;
- event latency;
- message latency;
- GPIO validation;
- DWT/general timer comparison.

## Randomized tests

Random queue operations:

```text
send
receive
timeout
wake
wrap
```

Random timer operations:

```text
start
stop
restart
tick
expire
```

Sau mỗi operation:

```c
assert(hr_queue_validate());
assert(hr_timer_validate());
```

---

# 87. Lỗi thường gặp

## 87.1 Queue lưu pointer tới stack local

Use-after-scope.

## 87.2 Queue copy sai item size

Memory corruption.

## 87.3 Head/tail wrap sai

Mất hoặc lặp message.

## 87.4 Count vượt capacity

Full invariant hỏng.

## 87.5 Wake receiver nhưng không có item

Spurious wake không có retry policy.

## 87.6 Give slot và wake sender nhưng count sai

Message duplication/loss.

## 87.7 ISR gọi blocking queue API

Invalid context.

## 87.8 Message pool double free

Free list cycle/corruption.

## 87.9 Timer callback chạy trong SysTick

Interrupt latency tăng.

## 87.10 Periodic timer reschedule từ now

Drift.

## 87.11 Stop timer khi callback pending không có policy

Callback chạy sau stop ngoài dự kiến.

## 87.12 Benchmark in UART trong timing window

Kết quả vô nghĩa.

## 87.13 Không đo timestamp overhead

Latency bị cộng instrumentation.

## 87.14 Chỉ report average

Che tail latency.

## 87.15 Gọi observed max là WCET

Không có bằng chứng worst-case tuyệt đối.

## 87.16 Không ghi CPU clock/compiler flags

Kết quả không tái lập.

---

# 88. Bài thực hành

Mỗi lab cần:

- README riêng;
- Makefile riêng;
- source riêng;
- tests riêng nếu host;
- expected result;
- fault injection;
- câu hỏi;
- tiêu chí hoàn thành.

---

## Bài 01 — Static ring-buffer queue

### Mục tiêu

- Static queue.
- FIFO.
- Ring wrap.
- Queue invariants.

### Tests

- init;
- send one;
- receive one;
- full;
- empty;
- wrap;
- item ordering;
- invalid input.

### Fault injection

Bỏ wrap tail về 0.

### Tiêu chí hoàn thành

- ASan/UBSan pass.
- Không malloc.
- FIFO đúng.
- Count đúng.

---

## Lab 02 — Blocking queue on host

### Mục tiêu

- Sender/receiver wait lists.
- No-wait.
- Finite timeout.
- Wait forever.

### Tests

- receive empty;
- send full;
- wake receiver;
- wake sender;
- timeout race;
- priority order;
- FIFO equal priority.

### Tiêu chí hoàn thành

- Single-winner wake.
- Object/timeout nodes đúng.
- Ready insertion đúng.

---

## Lab 03 — Queue from ISR

### Mục tiêu

- `send_from_isr`.
- Deferred PendSV.
- ISR-to-task latency marker.

### Target

Timer/EXTI ISR gửi message cho high-priority task.

### Tiêu chí hoàn thành

- ISR không block.
- Message đúng.
- High task preempt sau ISR.
- Có GPIO trace.

---

## Lab 04 — Message pool

### Mục tiêu

- Fixed-block pool.
- O(1) alloc/free.
- Pointer queue ownership.

### Tests

- allocate all;
- pool exhausted;
- free/reuse;
- invalid pointer;
- double free;
- alignment.

### Tiêu chí hoàn thành

- Không fragmentation ngoài.
- Ownership documented.
- Sanitizer pass.

---

## Lab 05 — Software timer list

### Mục tiêu

- One-shot.
- Periodic.
- Sorted list.
- Tick wrap.

### Tests

- same expiry;
- ordered expiry;
- start;
- stop;
- restart;
- periodic;
- wrap.

### Tiêu chí hoàn thành

- Timer state đúng.
- List invariant đúng.
- Drift policy rõ.

---

## Lab 06 — Timer service task

### Mục tiêu

- SysTick signal service.
- Callback trong task context.
- Command queue.

### Target

LED timers:

```text
Timer A one-shot
Timer B periodic
Timer C restartable
```

### Tiêu chí hoàn thành

- Callback không chạy trong SysTick.
- Service task priority documented.
- Cancel race test.

---

## Lab 07 — Timestamp backends

### Mục tiêu

So sánh:

- DWT CYCCNT;
- TIM2;
- GPIO pulse.

### Tests

- timestamp overhead;
- wrap;
- conversion cycles to time;
- GPIO measurement.

### Tiêu chí hoàn thành

- Backend contract rõ.
- Clock source ghi lại.
- Baseline overhead report.

---

## Lab 08 — Context-switch benchmark

### Mục tiêu

Đo context-switch latency trên Cortex-M3.

### Scenarios

- cooperative yield;
- preemptive high-priority wake;
- with/without controlled IRQ load.

### Report

```text
samples
min
max
average
histogram
```

### Tiêu chí hoàn thành

- Measurement points định nghĩa rõ.
- UART ngoài timing window.
- Raw/config report.

---

## Lab 09 — Event and message latency

### Mục tiêu

Đo:

- semaphore event latency;
- queue message latency;
- ISR-to-task latency.

### Variables

- message size;
- queue depth;
- task priorities;
- copy vs pointer.

### Tiêu chí hoàn thành

- Scenario reproducible.
- Tail latency report.
- Lost sample count.

---

## Lab 10 — Target Communication and Benchmark Console

### Mục tiêu

Chạy trên STM32F103:

- queue;
- message pool;
- software timer;
- context-switch benchmark;
- message benchmark;
- event benchmark;
- UART report.

### Commands

```text
help
queue
pool
timers
bench-switch
bench-event
bench-message
bench-timer
stats
validate
```

### Tiêu chí hoàn thành

- Queue demo.
- Timer demo.
- Benchmark output.
- Configuration report.
- Stack guards.
- Target validation.

---

# 89. Project tổng kết chủ đề

## Tên project

```text
HairRTOS Communication and Benchmark Console
```

## 89.1 Mục tiêu

Firmware tổng hợp:

- task-to-task queue;
- ISR-to-task queue;
- copy-by-value message;
- pointer message pool;
- one-shot timer;
- periodic timer;
- timer service task;
- latency benchmark;
- throughput benchmark;
- histogram;
- UART console.

## 89.2 Kiến trúc

```text
+---------------------------------------------------+
| Application Tasks                                 |
|                                                   |
| Producer  Consumer  Timer Service  Monitor        |
| Benchmark Sender  Benchmark Receiver  Idle        |
+-------------------------+-------------------------+
                          |
                          v
+---------------------------------------------------+
| Communication                                     |
|                                                   |
| Static Queue                                      |
| ISR-safe Queue API                                |
| Message Pool                                      |
+-------------------------+-------------------------+
                          |
                          v
+---------------------------------------------------+
| Software Timer                                    |
|                                                   |
| Sorted Timer List                                 |
| Timer Command Queue                               |
| Timer Service Task                                |
+-------------------------+-------------------------+
                          |
                          v
+---------------------------------------------------+
| Benchmark                                         |
|                                                   |
| DWT/TIM2 Timestamp                                |
| Trace Buffer                                      |
| Min/Max/Average/Histogram                         |
+-------------------------+-------------------------+
                          |
                          v
+---------------------------------------------------+
| HairRTOS Kernel                                   |
|                                                   |
| Scheduler  PendSV  SysTick  Timeout  Sync         |
+-------------------------+-------------------------+
                          |
                          v
+---------------------------------------------------+
| STM32F103 Platform                                |
+---------------------------------------------------+
```

## 89.3 Task set

### Producer

Tạo message định kỳ.

### Consumer

Block trên queue và xử lý message.

### Timer service

Chạy callbacks ngắn.

### Benchmark sender/receiver

Tạo controlled workload.

### Monitor

UART command và report.

### Idle

Luôn READY.

## 89.4 UART commands

```text
h   help
q   queue state
p   pool state
t   timer state
1   context-switch benchmark
2   event-latency benchmark
3   message-latency benchmark
4   timer-latency benchmark
5   queue-throughput benchmark
s   statistics
v   validate
```

## 89.5 Benchmark report

```text
Benchmark: message latency
MCU: STM32F103C8T6
CPU: 8 MHz HSI
Compiler: ...
Optimization: ...
Tick: 1000 Hz
Message size: 16 bytes
Queue capacity: 8
Samples: 10000
Timestamp: DWT CYCCNT

Minimum: ...
Maximum: ...
Average: ...
p50: ...
p90: ...
p99: ...
Lost samples: ...
```

## 89.6 Requirements

- Static queue storage.
- Static message pool.
- Queue timeout.
- ISR-safe send.
- Timer service task.
- Callback not in SysTick.
- One-shot and periodic timers.
- Timer stop/restart.
- Timestamp overhead measurement.
- No UART in timing window.
- Histogram or sample buffer.
- Host tests.
- Target validation.

---

# 90. Cấu trúc repository đề xuất

```text
05-communication-timer-benchmark/
├── README.md
├── Makefile
├── .gitignore
├── LICENSE
│
├── linker/
│   └── memory.ld
│
├── startup/
│   └── startup.c
│
├── include/
│   ├── compiler.h
│   ├── stm32f1.h
│   ├── clock.h
│   ├── gpio.h
│   ├── uart.h
│   ├── systick.h
│   ├── critical_section.h
│   ├── list.h
│   ├── task.h
│   ├── scheduler.h
│   ├── timeout.h
│   ├── queue.h
│   ├── message_pool.h
│   ├── software_timer.h
│   ├── timer_service.h
│   ├── benchmark_clock.h
│   ├── benchmark_stats.h
│   ├── benchmark_trace.h
│   └── benchmark_console.h
│
├── src/
│   ├── main.c
│   ├── runtime.c
│   ├── clock.c
│   ├── gpio.c
│   ├── uart.c
│   ├── systick.c
│   ├── critical_section.c
│   ├── list.c
│   ├── task.c
│   ├── scheduler.c
│   ├── timeout.c
│   ├── queue.c
│   ├── message_pool.c
│   ├── software_timer.c
│   ├── timer_service.c
│   ├── benchmark_clock.c
│   ├── benchmark_stats.c
│   ├── benchmark_trace.c
│   ├── benchmark_console.c
│   ├── cortex_m3_port.c
│   └── cortex_m3_portasm.S
│
├── labs/
│   ├── README.md
│   ├── 01-static-ring-buffer-queue/
│   ├── 02-blocking-queue-host/
│   ├── 03-queue-from-isr/
│   ├── 04-message-pool/
│   ├── 05-software-timer-list/
│   ├── 06-timer-service-task/
│   ├── 07-timestamp-backends/
│   ├── 08-context-switch-benchmark/
│   ├── 09-event-message-latency/
│   └── 10-target-communication-benchmark-console/
│
├── docs/
│   ├── message-passing.md
│   ├── queue-design.md
│   ├── message-ownership.md
│   ├── message-pool.md
│   ├── software-timer.md
│   ├── timer-service.md
│   ├── benchmark-methodology.md
│   ├── context-switch-latency.md
│   ├── event-message-latency.md
│   └── target-validation.md
│
├── tools/
│   ├── check_structure.py
│   ├── run_host_tests.sh
│   └── check_format.sh
│
└── build/
```

---

# 91. Liên hệ với roadmap HairRTOS

Chủ đề này liên quan trực tiếp:

## Phase 9 — Queue foundation

- static ring buffer;
- blocking send/receive;
- timeout;
- ISR-safe API;
- waiter ordering.

## Phase 12 — Software timer

- one-shot;
- periodic;
- timer list;
- timer service task;
- callback policy.

## Phase 14 — Allocator and object pool

- fixed-block message pool;
- ownership;
- pool diagnostics.

## Phase 15 — Benchmark and diagnostics

- context-switch latency;
- event/message latency;
- timer latency;
- statistics;
- target report.

---

# 92. Tiêu chí hoàn thành

## Communication

- Shared memory và message passing.
- Queue.
- Mailbox.
- Copy-by-value.
- Pointer message.
- Ownership.
- Lifetime.
- Static ring buffer.
- Full/empty.
- Blocking send.
- Blocking receive.
- No-wait.
- Finite timeout.
- Wait forever.
- Priority waiter ordering.
- FIFO equal priority.
- ISR-safe send.
- Message pool.
- Double-free detection.

## Software timer

- One-shot.
- Periodic.
- Timer state.
- Sorted timer list.
- Tick wrap.
- Start.
- Stop.
- Restart.
- Cancel race policy.
- Timer service task.
- Callback contract.
- Drift policy.
- Overrun detection.

## Benchmark

- Measurement definition.
- Timestamp backend.
- Timestamp overhead.
- Context-switch latency.
- Event latency.
- Message latency.
- ISR-to-task latency.
- Timer expiry latency.
- Queue throughput.
- Min.
- Max.
- Average.
- Histogram.
- Percentile hoặc documented alternative.
- Lost sample count.
- Reproducibility report.
- Không UART trong timing window.
- Không gọi observed max là proven WCET.

## Validation

- Host tests.
- ASan.
- UBSan.
- Queue randomized tests.
- Timer randomized tests.
- STM32 build.
- UART console.
- GPIO benchmark validation.
- Stack guards.
- Target report.

---

# 93. Tổng kết

Chủ đề 5 trả lời ba câu hỏi:

```text
Task trao đổi dữ liệu như thế nào?
```

```text
Kernel tạo nhiều timer logic từ một time base như thế nào?
```

```text
Làm sao đo performance của kernel một cách có định nghĩa?
```

Luồng kiến thức:

```text
Message passing
      |
      v
Static queue
      |
      v
Blocking communication
      |
      v
ISR-safe communication
      |
      v
Message pool
      |
      v
Software timer
      |
      v
Timer service task
      |
      v
Timestamp backend
      |
      v
Latency + throughput benchmark
      |
      v
Communication and Benchmark Console
```

Các nguyên tắc cần nhớ:

1. Message ownership phải rõ.
2. Pointer message cần lifetime hợp lệ.
3. Static queue phù hợp RTOS nhỏ.
4. Queue full/empty policy phải rõ.
5. ISR không được block.
6. Sender/receiver wait list cần timeout-safe wake.
7. Một task finite wait dùng wait node và timeout node riêng.
8. Message pool cần double-free detection.
9. Software timer không phải hardware timer.
10. Timer callback không nên chạy trong SysTick.
11. Timer service task giúp serialize callback.
12. Periodic timer nên reschedule từ previous expiry để giảm drift.
13. Timer stop/cancel cần state-machine policy.
14. Benchmark phải định nghĩa measurement points.
15. Context-switch latency và event latency không giống nhau.
16. Message size và queue depth ảnh hưởng kết quả.
17. Timestamp có overhead.
18. Không in UART trong timing window.
19. Average không thay thế maximum và tail distribution.
20. Maximum observed không phải proven WCET.
21. Ghi CPU clock, compiler và optimization.
22. Benchmark cần đủ sample và reproducible conditions.
23. GPIO pulse là phương pháp kiểm chứng ngoài firmware.
24. Host tests kiểm tra queue/timer logic trước target.
25. Target validation là bắt buộc cho benchmark Cortex-M3.

Sau chủ đề này, kết quả cần đạt:

```text
Một static message queue
+
Một fixed-block message pool
+
Một software timer service
+
Một benchmark timestamp backend
+
Một latency/throughput benchmark harness
+
Một Communication and Benchmark Console chạy trên STM32F103
```

---

## Nguồn chương trình

Phạm vi gốc của README được lấy từ Chủ đề 5 trong tài liệu:

```text
self-develop-rtos.pdf

Communication + Timer + Benchmark
    - Hệ thống truyền tin nhắn trong AK-mOS
    - Cơ chế hoạt động của message system
    - Software timer và SysTick
    - Quản lý timer trong AK-mOS
    - Case study benchmark AK-mOS trên Cortex-M3
        - Context-switch latency
        - Event/message latency
```

Những phần như queue API, message ownership, message pool, timer service task, benchmark statistics, DWT/TIM2/GPIO backend, hệ thống lab và project tổng kết là phần mở rộng thực hành dành cho HairRTOS.
