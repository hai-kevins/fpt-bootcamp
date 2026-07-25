# Chủ đề 1 - Giới thiệu RTOS và quản lý bộ nhớ trong Kernel
## RTOS Introduction, Getting Started

> Chủ đề này được xây dựng theo nội dung **“RTOS introduction, getting started with AKOS”** trong chương trình tự phát triển AKOS.

---

## Mục lục

1. [Mục tiêu học tập](#1-mục-tiêu-học-tập)
2. [Phạm vi của chủ đề](#2-phạm-vi-của-chủ-đề)
3. [Kiến thức cần chuẩn bị](#3-kiến-thức-cần-chuẩn-bị)
4. [RTOS là gì?](#4-rtos-là-gì)
5. [Real-time không có nghĩa là chạy thật nhanh](#5-real-time-không-có-nghĩa-là-chạy-thật-nhanh)
6. [Các khái niệm thời gian quan trọng](#6-các-khái-niệm-thời-gian-quan-trọng)
7. [Phân loại hệ thống real-time](#7-phân-loại-hệ-thống-real-time)
8. [Super-loop và RTOS](#8-super-loop-và-rtos)
9. [Khi nào nên và không nên dùng RTOS](#9-khi-nào-nên-và-không-nên-dùng-rtos)
10. [Cấu trúc tổng quát của một RTOS](#10-cấu-trúc-tổng-quát-của-một-rtos)
11. [Setup base source để nghiên cứu RTOS](#11-setup-base-source-để-nghiên-cứu-rtos)
12. [Bố cục bộ nhớ của MCU](#12-bố-cục-bộ-nhớ-của-mcu)
13. [Bộ nhớ được sử dụng trong Kernel như thế nào?](#13-bộ-nhớ-được-sử-dụng-trong-kernel-như-thế-nào)
14. [Cấp phát tĩnh và cấp phát động](#14-cấp-phát-tĩnh-và-cấp-phát-động)
15. [Fragmentation và các vấn đề bộ nhớ](#15-fragmentation-và-các-vấn-đề-bộ-nhớ)
16. [First-fit allocator](#16-first-fit-allocator)
17. [Thiết kế allocator lab](#17-thiết-kế-allocator-lab)
18. [Luồng allocate và free](#18-luồng-allocate-và-free)
19. [Mã khung first-fit allocator](#19-mã-khung-first-fit-allocator)
20. [Thống kê và đo fragmentation](#20-thống-kê-và-đo-fragmentation)
21. [Quản lý lỗi và invariant](#21-quản-lý-lỗi-và-invariant)
22. [Kiểm thử allocator](#22-kiểm-thử-allocator)
23. [Bài thực hành](#23-bài-thực-hành)
24. [Project tổng kết chủ đề](#24-project-tổng-kết-chủ-đề)
25. [Liên hệ với roadmap HairRTOS](#25-liên-hệ-với-roadmap-hairrtos)
26. [Lỗi thường gặp](#26-lỗi-thường-gặp)
27. [Tiêu chí hoàn thành](#27-tiêu-chí-hoàn-thành)
28. [Cấu trúc repository đề xuất](#28-cấu-trúc-repository-đề-xuất)
29. [Tổng kết](#29-tổng-kết)

---

# 1. Mục tiêu học tập

Sau khi hoàn thành chủ đề này, người học cần có khả năng:

- Giải thích được RTOS là gì.
- Giải thích được tại sao một hệ thống real-time cần tính xác định.
- Phân biệt được:
  - hệ thống chạy nhanh;
  - hệ thống phản hồi đúng deadline;
  - hệ thống có tính deterministic.
- Hiểu các khái niệm:
  - response time;
  - latency;
  - deadline;
  - jitter;
  - worst-case execution time;
  - determinism.
- So sánh được super-loop và RTOS.
- Hiểu bố cục bộ nhớ của MCU.
- Phân biệt được:
  - `.text`;
  - `.rodata`;
  - `.data`;
  - `.bss`;
  - stack;
  - heap.
- Giải thích được kernel cần bộ nhớ cho những đối tượng nào.
- Phân biệt static allocation và dynamic allocation.
- Hiểu internal fragmentation và external fragmentation.
- Giải thích được thuật toán first-fit.
- Tự cài đặt được một first-fit allocator cơ bản.
- Thực hiện được:
  - alignment;
  - block splitting;
  - adjacent block coalescing;
  - invalid-free detection;
  - double-free detection;
  - out-of-memory handling.
- Viết được host test cho allocator.
- Đo được mức sử dụng heap và fragmentation.
- Giải thích được vì sao kernel real-time thường ưu tiên hướng `static-first`.

---

# 2. Phạm vi của chủ đề

Theo chương trình tự phát triển RTOS, chủ đề đầu tiên gồm các nội dung chính:

```text
RTOS introduction, getting started with AKOS
    |
    +-- Setup base source to experiment RTOS concepts
    +-- What is a Real-Time Operating System?
    +-- Bài toán bộ nhớ trong RTOS
    +-- Phân loại cấp phát bộ nhớ
    +-- Fragmentation và memory issues
    +-- First-fit schema implementation
```

---

# 3. Kiến thức cần chuẩn bị

Người học nên nắm trước:

## 3.1 Ngôn ngữ C

- Biến và kiểu dữ liệu.
- Mảng.
- `struct`.
- `enum`.
- Con trỏ.
- Con trỏ tới `void`.
- Ép kiểu con trỏ.
- Con trỏ số học.
- Toán tử bit.
- `sizeof`.
- `offsetof`.
- `static`.
- `const`.
- `volatile`.

Ví dụ:

```c
typedef struct
{
    uint32_t size;
    uint32_t flags;
} block_header_t;
```

## 3.2 Bộ nhớ

- Địa chỉ.
- Byte và word.
- Alignment.
- Little-endian.
- Stack và heap.
- Layout Flash/SRAM.
- Linker script ở mức cơ bản.

## 3.3 Công cụ

- GCC hoặc Clang.
- Makefile.
- Git.
- GDB.
- `size`.
- `nm`.
- `objdump`.
- AddressSanitizer.
- UndefinedBehaviorSanitizer.

---

# 4. RTOS là gì?

RTOS là viết tắt của:

```text
Real-Time Operating System
```

Một RTOS cung cấp cơ chế tổ chức nhiều luồng công việc trong hệ thống nhúng, ví dụ:

```text
Sensor task
Communication task
Control task
Display task
Logger task
```

Các task không thực sự chạy đồng thời trên MCU một nhân. Kernel chuyển CPU giữa các task theo chính sách scheduling.

```text
Time ---------------------------------------------------->

CPU:
[ Task A ][ Task B ][ Task A ][ Task C ][ Task A ][ ... ]
```

Một RTOS nhỏ thường cung cấp:

- Task management.
- Scheduler.
- Context switch.
- Delay và timeout.
- Queue hoặc message passing.
- Semaphore.
- Mutex.
- Software timer.
- Interrupt-safe API.
- Diagnostic và benchmark.

RTOS không tự biến một chương trình thành real-time. Firmware chỉ đạt yêu cầu real-time khi:

- task priority được thiết kế đúng;
- thời gian thực thi được kiểm soát;
- blocking được phân tích;
- interrupt latency được giới hạn;
- critical section đủ ngắn;
- tài nguyên bộ nhớ đủ;
- deadline được kiểm chứng.

---

# 5. Real-time không có nghĩa là chạy thật nhanh

Một hệ thống có thể chạy rất nhanh nhưng không phải real-time.

Ví dụ:

```text
Lần 1: phản hồi sau 1 ms
Lần 2: phản hồi sau 2 ms
Lần 3: phản hồi sau 80 ms
```

Thời gian trung bình có thể nhỏ, nhưng lần phản hồi 80 ms có thể làm hệ thống trễ deadline.

Một hệ thống real-time quan tâm đến:

```text
Có phản hồi trước deadline trong trường hợp xấu nhất hay không?
```

Ví dụ:

```text
Deadline: 10 ms

Hệ thống A:
1 ms, 2 ms, 3 ms, 50 ms
=> Có lúc trễ deadline.

Hệ thống B:
7 ms, 8 ms, 8 ms, 9 ms
=> Chậm hơn trung bình nhưng có tính xác định tốt hơn.
```

Kết luận:

```text
Fast average response != Real-time guarantee
```

---

# 6. Các khái niệm thời gian quan trọng

## 6.1 Latency

Latency là khoảng thời gian từ khi một sự kiện xảy ra tới khi hệ thống bắt đầu phản ứng.

```text
Interrupt xảy ra
      |
      | interrupt latency
      v
ISR bắt đầu chạy
```

Hoặc:

```text
Event được post
      |
      | event latency
      v
Task nhận event
```

## 6.2 Response time

Response time là tổng thời gian từ khi yêu cầu xuất hiện tới khi công việc hoàn tất.

```text
Request
   |
   +-- waiting time
   +-- scheduling delay
   +-- execution time
   v
Response complete
```

## 6.3 Deadline

Deadline là thời điểm muộn nhất mà công việc phải hoàn thành.

```text
Release time                    Deadline
     |-----------------------------|
                 Task
```

## 6.4 Jitter

Jitter là độ dao động của thời điểm thực thi hoặc thời gian phản hồi.

Ví dụ một task chu kỳ 10 ms:

```text
Mong muốn:
0, 10, 20, 30, 40 ms

Thực tế:
0, 11, 20, 32, 41 ms
```

Sai lệch này là jitter.

## 6.5 WCET

WCET:

```text
Worst-Case Execution Time
```

Là thời gian thực thi lớn nhất của một đoạn code trong điều kiện được phân tích.

Không nên chỉ đo thời gian trung bình.

## 6.6 Determinism

Determinism là khả năng dự đoán giới hạn thời gian của hệ thống.

Ví dụ:

```text
Queue send: tối đa X cycle
Context switch: tối đa Y cycle
Critical section: tối đa Z cycle
```

Một API có thời gian trung bình rất nhanh nhưng không có giới hạn trên rõ ràng có thể không phù hợp cho đường xử lý real-time quan trọng.

---

# 7. Phân loại hệ thống real-time

## 7.1 Hard real-time

Trễ deadline có thể gây hậu quả nghiêm trọng.

Ví dụ:

- Airbag control.
- Motor protection.
- Medical control.
- Safety shutdown.
- Flight control.

```text
Miss deadline => System failure
```

## 7.2 Firm real-time

Kết quả sau deadline gần như không còn giá trị, nhưng một lần trễ chưa chắc gây thảm họa.

Ví dụ:

- Một frame xử lý camera bị quá hạn.
- Một gói dữ liệu điều khiển cũ.
- Một mẫu sensor không còn giá trị.

## 7.3 Soft real-time

Trễ deadline làm giảm chất lượng nhưng hệ thống vẫn hoạt động.

Ví dụ:

- Audio có tiếng giật.
- UI phản hồi chậm.
- Video drop frame.
- Logger ghi trễ.

---

# 8. Super-loop và RTOS

## 8.1 Super-loop

```c
int main(void)
{
    board_init();

    while (1)
    {
        button_process();
        sensor_process();
        communication_process();
        display_process();
    }
}
```

Ưu điểm:

- Đơn giản.
- Ít RAM.
- Dễ bắt đầu.
- Không có context switch.
- Phù hợp hệ thống nhỏ.

Nhược điểm:

- Một hàm chạy lâu ảnh hưởng toàn hệ thống.
- Khó xác định latency khi số module tăng.
- Logic blocking dễ lan rộng.
- Khó biểu diễn các công việc có priority khác nhau.
- Khó mở rộng thành nhiều chức năng độc lập.

## 8.2 RTOS

```text
+----------------------+
| High-priority task   |
+----------------------+
| Medium-priority task |
+----------------------+
| Low-priority task    |
+----------------------+
| Idle task            |
+----------------------+
          |
          v
       Scheduler
          |
          v
          CPU
```

Ưu điểm:

- Tách chức năng thành task.
- Có priority.
- Có blocking thay cho busy-wait.
- Có synchronization.
- Có IPC.
- Dễ tổ chức hệ thống nhiều chức năng.

Nhược điểm:

- Tốn RAM cho TCB và stack.
- Tăng độ phức tạp.
- Có context-switch overhead.
- Có thể xuất hiện race condition.
- Có thể deadlock.
- Có thể priority inversion.
- Cần thiết kế priority và ownership cẩn thận.

## 8.3 Không phải project nào cũng cần RTOS

Một firmware nhỏ như:

```text
Đọc button
Điều khiển LED
Gửi UART đơn giản
```

có thể dùng super-loop tốt hơn.

RTOS có ích khi hệ thống có:

- nhiều luồng xử lý độc lập;
- nhiều deadline;
- nhiều nguồn interrupt;
- giao tiếp song song;
- blocking I/O;
- priority khác nhau;
- nhiều state dài hạn;
- yêu cầu mở rộng và bảo trì.

---

# 9. Khi nào nên và không nên dùng RTOS

## Nên cân nhắc dùng RTOS khi

- Có nhiều chức năng hoạt động đồng thời về mặt logic.
- Có task điều khiển quan trọng hơn task giao diện.
- Có nhiều timeout khác nhau.
- Có communication stack.
- Có sensor processing.
- Có task phải chờ dữ liệu.
- Cần message passing.
- Cần tổ chức ownership rõ ràng.
- Cần tách module để kiểm thử.
- Có yêu cầu latency và deadline.

## Chưa cần RTOS khi

- Chương trình rất nhỏ.
- Một state machine đơn đã đủ.
- RAM quá hạn chế.
- Không có yêu cầu concurrency.
- Không có nhiều deadline.
- Event-driven super-loop đã giải quyết tốt.
- Team chưa đủ khả năng phân tích concurrency.

## Anti-pattern

Không nên dùng RTOS chỉ để thay:

```c
delay_ms(1000U);
```

bằng:

```c
rtos_task_delay(1000U);
```

mà không thiết kế lại:

- task boundary;
- priority;
- data ownership;
- communication;
- blocking policy;
- error handling.

---

# 10. Cấu trúc tổng quát của một RTOS

```text
+------------------------------------------------------+
|                    Application                       |
|                                                      |
|  Sensor Task  Control Task  Logger Task  UI Task     |
+-------------------------+----------------------------+
                          |
                          v
+------------------------------------------------------+
|                    RTOS Kernel                       |
|                                                      |
|  Task  Scheduler  Queue  Semaphore  Mutex  Timer     |
+-------------------------+----------------------------+
                          |
                          v
+------------------------------------------------------+
|                 Architecture Port                    |
|                                                      |
|  SVC  PendSV  SysTick  Critical Section  Context     |
+-------------------------+----------------------------+
                          |
                          v
+------------------------------------------------------+
|                     Platform                         |
|                                                      |
|  Startup  Clock  GPIO  UART  Timer  Interrupt        |
+-------------------------+----------------------------+
                          |
                          v
+------------------------------------------------------+
|                ARM Cortex-M3 / STM32F103              |
+------------------------------------------------------+
```

Trong chủ đề 1, chưa triển khai scheduler và context switch. Mục tiêu là chuẩn bị nền tảng để các chủ đề sau không phụ thuộc vào code HAL hoặc môi trường build không rõ ràng.

---

# 11. Setup base source để nghiên cứu RTOS

Một base source tối thiểu cần có:

```text
Startup code
Linker script
Vector table
Clock setup
GPIO
UART
SysTick tạm thời
Build system
Flash target
Debug target
Map file
```

## 11.1 Startup code

Startup code cần:

- đặt vector table tại Flash base;
- thiết lập initial MSP;
- copy `.data` từ Flash sang SRAM;
- clear `.bss`;
- gọi system initialization;
- gọi `main()`.

Luồng khởi động:

```text
Reset
  |
  v
CPU đọc initial MSP
  |
  v
CPU đọc Reset_Handler
  |
  v
Copy .data
  |
  v
Clear .bss
  |
  v
Clock initialization
  |
  v
main()
```

## 11.2 Linker script

Linker script mô tả:

- Flash bắt đầu ở đâu;
- SRAM bắt đầu ở đâu;
- dung lượng mỗi vùng;
- vị trí vector table;
- vị trí `.text`;
- vị trí `.data`;
- vị trí `.bss`;
- stack top;
- symbol cho startup code.

Ví dụ khái quát:

```ld
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}
```

## 11.3 UART debug

UART giúp quan sát:

- boot status;
- memory addresses;
- allocator block list;
- heap statistics;
- assertion failure;
- test result.

Không nên dùng UART `printf()` trong đoạn benchmark timing vì thời gian UART làm sai kết quả.

## 11.4 SysTick tạm thời

Trong nền tảng bare-metal ban đầu, SysTick có thể được dùng làm time base.

Về sau, khi kernel tick được triển khai, trách nhiệm này được chuyển sang kernel time service.

---

# 12. Bố cục bộ nhớ của MCU

Một chương trình embedded thường có bố cục:

```text
FLASH
+----------------------------------+
| Vector Table                     |
+----------------------------------+
| .text                            |
| Machine code                     |
+----------------------------------+
| .rodata                          |
| Constants                        |
+----------------------------------+
| Initial image of .data           |
+----------------------------------+

SRAM
+----------------------------------+ Low address
| .data                            |
| Initialized global/static data   |
+----------------------------------+
| .bss                             |
| Zero-initialized data            |
+----------------------------------+
| Kernel objects                   |
| TCB, queues, timers, mutexes      |
+----------------------------------+
| Heap / memory pools              |
| Optional                         |
+----------------------------------+
| Free RAM                         |
+----------------------------------+
| Main stack / MSP                 |
| grows downward                   |
+----------------------------------+ High address
```

## 12.1 `.text`

Chứa mã máy.

```c
uint32_t add(uint32_t a, uint32_t b)
{
    return a + b;
}
```

## 12.2 `.rodata`

Chứa dữ liệu chỉ đọc.

```c
static const char g_banner[] = "RTOS Memory Lab";
```

## 12.3 `.data`

Chứa biến global/static có giá trị khởi tạo khác 0.

```c
static uint32_t g_tick_hz = 1000U;
```

Giá trị ban đầu nằm trong Flash và được copy sang RAM khi reset.

## 12.4 `.bss`

Chứa biến global/static chưa khởi tạo hoặc khởi tạo bằng 0.

```c
static uint32_t g_system_tick;
static uint8_t g_uart_rx_buffer[128];
```

`.bss` phải được clear trước khi vào `main()`.

## 12.5 Stack

Stack dùng cho:

- biến cục bộ;
- tham số;
- địa chỉ quay về;
- saved registers;
- exception frame.

Trong RTOS, mỗi task thường có stack riêng.

```text
Task A -> Stack A
Task B -> Stack B
Task C -> Stack C
```

## 12.6 Heap

Heap là vùng bộ nhớ dành cho cấp phát động.

```c
void *pointer = malloc(size);
free(pointer);
```

Trong kernel real-time, heap phải được đánh giá về:

- thời gian allocate;
- thời gian free;
- fragmentation;
- failure behavior;
- thread safety;
- ISR safety;
- debug khả năng.

---

# 13. Bộ nhớ được sử dụng trong Kernel như thế nào?

Kernel cần bộ nhớ cho nhiều loại object.

## 13.1 Task Control Block

```text
TCB
+-- saved stack pointer
+-- stack bounds
+-- task state
+-- base priority
+-- effective priority
+-- wake tick
+-- list nodes
+-- runtime statistics
```

## 13.2 Task stack

Mỗi task cần stack riêng.

```text
Task count tăng
    |
    v
Tổng RAM dành cho stack tăng
```

Ví dụ:

```text
4 tasks x 512 bytes = 2048 bytes
```

Với STM32F103C8 có 20 KiB SRAM, stack cần được tính cẩn thận.

## 13.3 Queue storage

Một queue thường cần:

```text
Queue object
+ Ring buffer storage
+ Wait list for senders
+ Wait list for receivers
```

Ví dụ:

```text
16 messages x 16 bytes = 256 bytes payload storage
```

## 13.4 Synchronization objects

- Semaphore.
- Mutex.
- Event flags.
- Condition object.

## 13.5 Timer objects

Mỗi software timer có thể chứa:

- period;
- expiry time;
- callback;
- state;
- list node.

## 13.6 Event objects

Event-driven framework có thể dùng:

- static event;
- stack event;
- copied event;
- fixed-size event pool;
- dynamic event.

## 13.7 Kernel metadata

- Ready lists.
- Delayed list.
- All-task list.
- Ready bitmap.
- Runtime counters.
- Diagnostic data.

---

# 14. Cấp phát tĩnh và cấp phát động

## 14.1 Cấp phát tĩnh

Object được khai báo trước.

```c
static uint8_t task_a_stack[512];
static rtos_task_t task_a_tcb;
```

Ưu điểm:

- Capacity biết trước.
- Không có external fragmentation.
- Không fail bất ngờ sau khi hệ thống đã chạy lâu.
- Dễ tính RAM.
- Dễ review.
- Phù hợp hệ thống safety-critical hơn.

Nhược điểm:

- Có thể cấp dư.
- Kém linh hoạt.
- Số object bị giới hạn cố định.
- Application phải quản lý storage.

## 14.2 Cấp phát trên stack

```c
void process(void)
{
    uint8_t buffer[64];
}
```

Ưu điểm:

- Rất nhanh.
- Tự giải phóng khi thoát scope.
- Không fragmentation.

Nhược điểm:

- Lifetime ngắn.
- Không được trả pointer tới biến local sau khi hàm kết thúc.
- Buffer lớn có thể làm stack overflow.
- Trong RTOS, mỗi task stack có giới hạn riêng.

## 14.3 Cấp phát động

```c
void *buffer = heap_alloc(128U);

if (buffer == NULL)
{
    handle_out_of_memory();
}
```

Ưu điểm:

- Linh hoạt.
- Object được tạo theo nhu cầu.
- Có thể giảm memory reservation ban đầu.

Nhược điểm:

- Có thể hết bộ nhớ tại runtime.
- Có thể fragmentation.
- Thời gian allocate/free có thể thay đổi.
- Dễ leak.
- Dễ double free.
- Dễ use-after-free.
- Cần synchronization khi nhiều context truy cập.

## 14.4 Fixed-size memory pool

```text
Pool:
[32 bytes][32 bytes][32 bytes][32 bytes]
```

Ưu điểm:

- Allocation thường `O(1)`.
- Không external fragmentation trong pool.
- Capacity rõ ràng.
- Phù hợp Event, Message, Packet, Timer node.

Nhược điểm:

- Object lớn hơn block không thể cấp.
- Có internal fragmentation.
- Cần nhiều pool nếu có nhiều kích thước object.

---

# 15. Fragmentation và các vấn đề bộ nhớ

## 15.1 Internal fragmentation

Internal fragmentation là phần bộ nhớ nằm trong block đã cấp nhưng application không sử dụng.

Ví dụ:

```text
Requested: 13 bytes
Allocator gives: 16 bytes
Unused inside block: 3 bytes
```

Nguyên nhân:

- alignment;
- fixed block size;
- size class;
- minimum block size.

## 15.2 External fragmentation

External fragmentation xảy ra khi tổng bộ nhớ rảnh còn đủ nhưng bị chia thành nhiều block nhỏ không liên tục.

Ví dụ:

```text
[USED 32][FREE 16][USED 32][FREE 16][USED 32][FREE 16]
```

Tổng free:

```text
16 + 16 + 16 = 48 bytes
```

Nhưng yêu cầu 32 bytes có thể thất bại nếu không có block free liên tục đủ lớn.

## 15.3 Memory leak

```c
void *buffer = heap_alloc(64U);

/* Quên heap_free(buffer). */
```

Mỗi lần lặp làm mất thêm 64 byte.

## 15.4 Double free

```c
heap_free(buffer);
heap_free(buffer);
```

Double free có thể:

- làm hỏng free list;
- cấp cùng một block cho hai object;
- gây crash không ổn định.

## 15.5 Invalid free

```c
uint8_t local;
heap_free(&local);
```

Pointer không thuộc heap.

Hoặc:

```c
uint8_t *buffer = heap_alloc(64U);
heap_free(&buffer[4]);
```

Pointer trỏ vào giữa payload, không phải đầu block.

## 15.6 Use-after-free

```c
uint8_t *buffer = heap_alloc(32U);
heap_free(buffer);

buffer[0] = 0x55U;
```

Pointer đã không còn ownership hợp lệ.

## 15.7 Heap corruption

Nguyên nhân phổ biến:

- ghi vượt payload;
- header bị ghi đè;
- sai alignment;
- split sai kích thước;
- coalesce sai liên kết;
- free pointer không hợp lệ;
- concurrent access không khóa.

---

# 16. First-fit allocator

First-fit tìm block free đầu tiên có kích thước đủ lớn.

Ví dụ danh sách block:

```text
[USED 32]
[FREE 16]
[FREE 64]
[FREE 128]
```

Yêu cầu:

```text
Allocate 48 bytes
```

First-fit bỏ qua block 16 byte và chọn block 64 byte đầu tiên.

```text
Search from heap start
        |
        v
Block 32: used -> skip
        |
        v
Block 16: too small -> skip
        |
        v
Block 64: fits -> allocate
```

## 16.1 Ưu điểm

- Dễ hiểu.
- Dễ cài đặt.
- Thường tìm được block nhanh nếu block phù hợp nằm đầu list.
- Phù hợp lab để học allocator.

## 16.2 Nhược điểm

- Thời gian tìm kiếm phụ thuộc số block.
- Có thể tạo nhiều lỗ nhỏ ở đầu heap.
- Worst-case allocation có thể duyệt toàn bộ block list.
- Không bảo đảm thời gian cố định.
- Không phải lựa chọn tốt nhất cho hard real-time path.

## 16.3 Độ phức tạp

Với `n` block:

```text
Allocate worst case: O(n)
Free: tùy cấu trúc, thường O(1) hoặc O(n)
```

Nếu coalescing dựa trên physical neighbors được lưu trong header, `free()` có thể gộp lân cận trực tiếp.

---

# 17. Thiết kế allocator lab

## 17.1 Phạm vi

Allocator lab cần hỗ trợ:

- aligned allocation;
- first-fit search;
- block header;
- exact fit;
- splitting;
- forward coalescing;
- backward coalescing;
- invalid free;
- double free;
- exhaustion;
- statistics.

Không yêu cầu:

- `realloc`;
- multi-core;
- lock-free;
- use trong ISR;
- use từ PendSV;
- thay thế C library heap;
- production hard real-time guarantee.

## 17.2 Mô hình block

```text
Heap memory
+------------------+--------------------------+
| Block Header     | Payload                  |
+------------------+--------------------------+
```

Nhiều block:

```text
[Header A | Payload A][Header B | Payload B][Header C | Payload C]
```

Header có thể chứa:

```c
typedef struct heap_block
{
    size_t payload_size;
    struct heap_block *previous;
    struct heap_block *next;
    uint32_t magic;
    bool is_free;
} heap_block_t;
```

## 17.3 Magic value

```c
#define HEAP_BLOCK_MAGIC (0x48454150UL)
```

`0x48454150` tương ứng dạng ASCII gần với `"HEAP"`.

Magic giúp phát hiện:

- pointer không trỏ vào block hợp lệ;
- header bị corruption;
- invalid free.

Magic không thay thế hoàn toàn boundary checking.

## 17.4 Alignment

Giả sử alignment 8 byte:

```c
#define HEAP_ALIGNMENT (8U)
```

Công thức làm tròn:

```c
aligned = (size + alignment - 1U) & ~(alignment - 1U);
```

Alignment phải là lũy thừa của hai.

Ví dụ:

```text
Request 1  -> 8
Request 7  -> 8
Request 8  -> 8
Request 9  -> 16
Request 13 -> 16
```

## 17.5 Minimum split size

Không nên split nếu phần còn lại quá nhỏ.

```text
Remaining size must fit:
    block header
    + minimum aligned payload
```

Điều kiện:

```text
old_payload >= requested_aligned
              + sizeof(block_header)
              + minimum_payload
```

Nếu không đủ, cấp toàn bộ block hiện tại.

---

# 18. Luồng allocate và free

## 18.1 Allocate

```text
heap_alloc(requested_size)
        |
        v
Validate requested_size
        |
        v
Align size
        |
        v
Search first free block that fits
        |
        +-- not found --> return NULL
        |
        v
Can split?
        |
        +-- yes --> create new free block
        |
        v
Mark selected block allocated
        |
        v
Update statistics
        |
        v
Return payload pointer
```

## 18.2 Free

```text
heap_free(pointer)
        |
        v
Check pointer != NULL
        |
        v
Check pointer belongs to heap
        |
        v
Get header from payload pointer
        |
        v
Check magic
        |
        v
Check block is currently allocated
        |
        v
Mark block free
        |
        v
Coalesce next free block
        |
        v
Coalesce previous free block
        |
        v
Update statistics
        |
        v
Return status
```

## 18.3 Splitting

Trước:

```text
[ FREE payload = 128 ]
```

Yêu cầu 32 byte:

```text
[ USED 32 ][ HEADER ][ FREE remaining ]
```

Sau split:

```text
+----------+------------+----------+------------------+
| Header A | Used 32    | Header B | Free remaining   |
+----------+------------+----------+------------------+
```

## 18.4 Coalescing

Trước:

```text
[FREE A][FREE B]
```

Sau:

```text
[FREE A + header B + payload B]
```

Coalescing giúp giảm external fragmentation.

---

# 19. Mã khung first-fit allocator

> Đoạn code dưới đây là khung học tập để minh họa thiết kế.  
> Lab hoàn chỉnh cần bổ sung assert, test, overflow checking, statistics và kiểm tra pointer nghiêm ngặt.

## 19.1 Public API

```c
#ifndef HEAP_LAB_H
#define HEAP_LAB_H

#include <stddef.h>
#include <stdint.h>

typedef enum
{
    HEAP_STATUS_OK = 0,
    HEAP_STATUS_INVALID_ARGUMENT,
    HEAP_STATUS_NOT_INITIALIZED,
    HEAP_STATUS_POINTER_OUT_OF_RANGE,
    HEAP_STATUS_INVALID_POINTER,
    HEAP_STATUS_DOUBLE_FREE,
    HEAP_STATUS_CORRUPTED
} heap_status_t;

typedef struct
{
    size_t heap_size;
    size_t used_payload_bytes;
    size_t free_payload_bytes;
    size_t largest_free_block;
    size_t block_count;
    size_t free_block_count;
    size_t allocation_count;
    size_t free_count;
    size_t failed_allocation_count;
} heap_stats_t;

heap_status_t heap_lab_init(void *memory, size_t size);
void *heap_lab_alloc(size_t size);
heap_status_t heap_lab_free(void *pointer);
heap_status_t heap_lab_get_stats(heap_stats_t *stats);
bool heap_lab_validate(void);

#endif
```

## 19.2 Internal block

```c
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define HEAP_ALIGNMENT          (8U)
#define HEAP_BLOCK_MAGIC        (0x48454150UL)
#define HEAP_MIN_PAYLOAD_SIZE   (8U)

typedef struct heap_block
{
    size_t payload_size;
    struct heap_block *previous;
    struct heap_block *next;
    uint32_t magic;
    bool is_free;
} heap_block_t;

typedef struct
{
    uint8_t *begin;
    uint8_t *end;
    heap_block_t *first;
    bool initialized;
    size_t allocation_count;
    size_t free_count;
    size_t failed_allocation_count;
} heap_context_t;

static heap_context_t g_heap;
```

## 19.3 Align size

```c
static bool is_power_of_two(size_t value)
{
    return (value != 0U) &&
           ((value & (value - 1U)) == 0U);
}

static bool align_up_size(size_t value,
                          size_t alignment,
                          size_t *result)
{
    if ((result == NULL) || !is_power_of_two(alignment))
    {
        return false;
    }

    if (value > (SIZE_MAX - (alignment - 1U)))
    {
        return false;
    }

    *result = (value + alignment - 1U) &
              ~(alignment - 1U);

    return true;
}
```

## 19.4 Lấy payload từ header

```c
static void *block_to_payload(heap_block_t *block)
{
    if (block == NULL)
    {
        return NULL;
    }

    return (void *)((uint8_t *)block +
                    sizeof(heap_block_t));
}
```

## 19.5 Lấy header từ payload

```c
static heap_block_t *payload_to_block(void *payload)
{
    if (payload == NULL)
    {
        return NULL;
    }

    return (heap_block_t *)((uint8_t *)payload -
                               sizeof(heap_block_t));
}
```

## 19.6 Tìm block first-fit

```c
static heap_block_t *find_first_fit(size_t requested_size)
{
    heap_block_t *block = g_heap.first;

    while (block != NULL)
    {
        if (block->magic != HEAP_BLOCK_MAGIC)
        {
            return NULL;
        }

        if (block->is_free &&
            (block->payload_size >= requested_size))
        {
            return block;
        }

        block = block->next;
    }

    return NULL;
}
```

## 19.7 Kiểm tra có thể split

```c
static bool block_can_split(const heap_block_t *block,
                            size_t requested_size)
{
    size_t required_size;

    if (block == NULL)
    {
        return false;
    }

    if (requested_size >
        (SIZE_MAX - sizeof(heap_block_t) -
         HEAP_MIN_PAYLOAD_SIZE))
    {
        return false;
    }

    required_size = requested_size +
                    sizeof(heap_block_t) +
                    HEAP_MIN_PAYLOAD_SIZE;

    return block->payload_size >= required_size;
}
```

## 19.8 Split block

```c
static void split_block(heap_block_t *block,
                        size_t requested_size)
{
    uint8_t *new_header_address;
    heap_block_t *new_block;

    if ((block == NULL) ||
        !block_can_split(block, requested_size))
    {
        return;
    }

    new_header_address =
        (uint8_t *)block_to_payload(block) +
        requested_size;

    new_block = (heap_block_t *)new_header_address;

    new_block->payload_size =
        block->payload_size -
        requested_size -
        sizeof(heap_block_t);

    new_block->previous = block;
    new_block->next = block->next;
    new_block->magic = HEAP_BLOCK_MAGIC;
    new_block->is_free = true;

    if (new_block->next != NULL)
    {
        new_block->next->previous = new_block;
    }

    block->next = new_block;
    block->payload_size = requested_size;
}
```

## 19.9 Allocate

```c
void *heap_lab_alloc(size_t size)
{
    size_t aligned_size;
    heap_block_t *block;

    if (!g_heap.initialized || (size == 0U))
    {
        return NULL;
    }

    if (!align_up_size(size,
                       HEAP_ALIGNMENT,
                       &aligned_size))
    {
        g_heap.failed_allocation_count++;
        return NULL;
    }

    block = find_first_fit(aligned_size);

    if (block == NULL)
    {
        g_heap.failed_allocation_count++;
        return NULL;
    }

    if (block_can_split(block, aligned_size))
    {
        split_block(block, aligned_size);
    }

    block->is_free = false;
    g_heap.allocation_count++;

    return block_to_payload(block);
}
```

## 19.10 Merge với block kế tiếp

```c
static bool merge_with_next(heap_block_t *block)
{
    heap_block_t *next;

    if (block == NULL)
    {
        return false;
    }

    next = block->next;

    if ((next == NULL) ||
        !block->is_free ||
        !next->is_free)
    {
        return false;
    }

    if ((block->magic != HEAP_BLOCK_MAGIC) ||
        (next->magic != HEAP_BLOCK_MAGIC))
    {
        return false;
    }

    block->payload_size +=
        sizeof(heap_block_t) +
        next->payload_size;

    block->next = next->next;

    if (block->next != NULL)
    {
        block->next->previous = block;
    }

    next->magic = 0U;
    next->previous = NULL;
    next->next = NULL;
    next->payload_size = 0U;

    return true;
}
```

## 19.11 Free

```c
heap_status_t heap_lab_free(void *pointer)
{
    heap_block_t *block;

    if (!g_heap.initialized)
    {
        return HEAP_STATUS_NOT_INITIALIZED;
    }

    if (pointer == NULL)
    {
        return HEAP_STATUS_INVALID_ARGUMENT;
    }

    if (((uint8_t *)pointer < g_heap.begin) ||
        ((uint8_t *)pointer >= g_heap.end))
    {
        return HEAP_STATUS_POINTER_OUT_OF_RANGE;
    }

    block = payload_to_block(pointer);

    if (((uint8_t *)block < g_heap.begin) ||
        ((uint8_t *)block >= g_heap.end))
    {
        return HEAP_STATUS_INVALID_POINTER;
    }

    if (block->magic != HEAP_BLOCK_MAGIC)
    {
        return HEAP_STATUS_INVALID_POINTER;
    }

    if (block->is_free)
    {
        return HEAP_STATUS_DOUBLE_FREE;
    }

    block->is_free = true;
    g_heap.free_count++;

    (void)merge_with_next(block);

    if ((block->previous != NULL) &&
        block->previous->is_free)
    {
        block = block->previous;
        (void)merge_with_next(block);
    }

    return HEAP_STATUS_OK;
}
```

## 19.12 Điểm còn phải hoàn thiện

Khung trên chưa đủ để gọi là production allocator.

Cần bổ sung:

- kiểm tra alignment của vùng heap;
- tránh arithmetic overflow khi init;
- kiểm tra pointer có đúng bằng payload start;
- xác nhận physical adjacency trước khi merge;
- kiểm tra toàn bộ list invariant;
- poison payload sau `free`;
- canary ở đầu/cuối payload;
- critical section hoặc lock;
- unit tests;
- randomized stress test;
- fragmentation statistics.

---

# 20. Thống kê và đo fragmentation

Allocator nên báo tối thiểu:

```text
Heap size
Used payload bytes
Free payload bytes
Largest free block
Block count
Free block count
Allocation count
Free count
Failed allocation count
```

## 20.1 External fragmentation indicator

Có thể dùng chỉ số đơn giản:

```text
external_fragmentation =
    1 - largest_free_block / total_free_bytes
```

Diễn giải:

```text
0.0  -> toàn bộ free memory nằm trong một block lớn
gần 1 -> free memory bị chia thành nhiều block nhỏ
```

Khi `total_free_bytes == 0`, cần xử lý riêng để tránh chia cho 0.

## 20.2 Internal fragmentation

Có thể theo dõi:

```text
aligned_size - requested_size
```

cho từng allocation.

Tổng internal fragmentation:

```text
sum(aligned_size - requested_size)
```

## 20.3 Ví dụ

```text
Free blocks:
16, 32, 80 bytes

Total free:
128 bytes

Largest:
80 bytes

Indicator:
1 - 80/128 = 0.375
```

Đây chỉ là một chỉ số quan sát, không phải định nghĩa duy nhất cho mọi allocator.

---

# 21. Quản lý lỗi và invariant

## 21.1 Invariant của block list

Allocator cần duy trì:

- `first->previous == NULL`.
- Block cuối có `next == NULL`.
- `block->next->previous == block`.
- Mọi block đều nằm trong heap range.
- Mọi header đều có magic hợp lệ.
- Payload size không vượt heap.
- Hai block liên tiếp không overlap.
- Sau coalescing không nên còn hai free block vật lý liền nhau.
- Tổng kích thước header và payload không vượt heap size.

## 21.2 Heap validation

```c
bool heap_lab_validate(void)
{
    const heap_block_t *block;
    const heap_block_t *previous = NULL;
    const uint8_t *cursor;

    if (!g_heap.initialized)
    {
        return false;
    }

    block = g_heap.first;
    cursor = g_heap.begin;

    while (block != NULL)
    {
        if ((const uint8_t *)block != cursor)
        {
            return false;
        }

        if (block->magic != HEAP_BLOCK_MAGIC)
        {
            return false;
        }

        if (block->previous != previous)
        {
            return false;
        }

        cursor = (const uint8_t *)block +
                 sizeof(heap_block_t) +
                 block->payload_size;

        if (cursor > g_heap.end)
        {
            return false;
        }

        previous = block;
        block = block->next;
    }

    return cursor == g_heap.end;
}
```

## 21.3 Error policy

Lab có thể trả status:

```text
OK
INVALID_ARGUMENT
NOT_INITIALIZED
OUT_OF_RANGE
INVALID_POINTER
DOUBLE_FREE
CORRUPTED
```

Production kernel cần xác định rõ:

- return error;
- assert;
- fatal hook;
- system reset;
- diagnostic counter.

Không nên âm thầm bỏ qua heap corruption.

---

# 22. Kiểm thử allocator

## 22.1 Unit test trên host

Host test giúp:

- chạy nhanh;
- dùng sanitizer;
- kiểm tra nhiều trường hợp;
- không cần flash MCU;
- debug dễ hơn.

Compiler flags gợi ý:

```bash
-std=c11
-Wall
-Wextra
-Werror
-pedantic
-fsanitize=address,undefined
-fno-omit-frame-pointer
```

## 22.2 Test tối thiểu

### Init

- Heap hợp lệ.
- Heap quá nhỏ.
- Pointer `NULL`.
- Vùng nhớ không aligned.
- Init nhiều lần.

### Allocation

- Request 1 byte.
- Request đúng alignment.
- Request cần làm tròn.
- Exact fit.
- Split.
- Không split vì phần dư quá nhỏ.
- Allocate tới exhaustion.
- Request `0`.
- Request gây overflow.

### Free

- Free block đầu.
- Free block giữa.
- Free block cuối.
- Free `NULL`.
- Free pointer ngoài heap.
- Free pointer vào giữa payload.
- Double free.

### Coalescing

- Merge forward.
- Merge backward.
- Merge cả hai phía.
- Allocate lại sau merge.

### Statistics

- Used bytes đúng.
- Free bytes đúng.
- Largest free block đúng.
- Failed allocation count đúng.
- Block count đúng.

### Randomized test

Sinh chuỗi:

```text
allocate
allocate
free
allocate
free
free
...
```

Sau mỗi operation:

```c
assert(heap_lab_validate());
```

## 22.3 Fault injection

Chủ động làm hỏng:

- magic;
- payload size;
- `next`;
- `previous`;
- block state.

Xác nhận validator phát hiện lỗi.

---

# 23. Bài thực hành

## Bài 1 — Quan sát memory layout

### Mục tiêu

Hiểu firmware được đặt trong Flash và RAM như thế nào.

### Yêu cầu

Tạo các biến:

```c
const uint32_t g_const_value = 0x11111111U;
uint32_t g_initialized_value = 0x22222222U;
uint32_t g_zero_value;
static uint8_t g_buffer[128];
```

Build và kiểm tra:

```bash
arm-none-eabi-size
arm-none-eabi-nm
arm-none-eabi-objdump
```

Đọc file `.map` và xác định:

- `g_const_value`;
- `g_initialized_value`;
- `g_zero_value`;
- `g_buffer`;
- vector table;
- stack top.

### Tiêu chí hoàn thành

- Xác định đúng variable thuộc section nào.
- Giải thích vì sao `.data` có cả Flash image và RAM address.
- Giải thích vì sao `.bss` không cần lưu toàn bộ zero trong firmware image.
- Chụp lại một đoạn map file có giải thích.

---

## Bài 2 — Kiểm chứng startup code

### Mục tiêu

Chứng minh `.data` được copy và `.bss` được clear.

### Thực hiện

Tạm thời tạo lỗi có chủ đích:

1. Bỏ copy `.data`.
2. Quan sát biến initialized sai.
3. Khôi phục.
4. Bỏ clear `.bss`.
5. Quan sát biến zero-initialized không đúng.
6. Khôi phục.

### Tiêu chí hoàn thành

- Giải thích được luồng Reset Handler.
- Xác định được `_sidata`, `_sdata`, `_edata`.
- Xác định được `_sbss`, `_ebss`.
- Firmware hoạt động đúng sau khi khôi phục startup.

---

## Bài 3 — Stack pattern và high-water mark

### Mục tiêu

Hiểu stack usage.

### Yêu cầu

- Tạo một vùng stack test.
- Fill bằng `0xA5`.
- Gọi các hàm có biến local.
- Quét pattern còn lại.
- Tính high-water mark.

### Thí nghiệm

```c
static void consume_stack(void)
{
    volatile uint8_t local_buffer[128];

    for (size_t i = 0U; i < sizeof(local_buffer); ++i)
    {
        local_buffer[i] = (uint8_t)i;
    }
}
```

### Tiêu chí hoàn thành

- Giải thích stack growth direction.
- Đo được mức sử dụng stack.
- Không tối ưu mất buffer thí nghiệm.
- Ghi lại kết quả ở `docs/stack-experiment.md`.

---

## Bài 4 — Static allocation budget

### Mục tiêu

Tính RAM cho một thiết kế RTOS giả định.

### Cấu hình

```text
4 task stacks
4 TCBs
2 queues
3 mutexes
4 software timers
event pool
main stack
```

### Yêu cầu

Tạo bảng:

| Object | Count | Size mỗi object | Tổng |
|---|---:|---:|---:|
| Task stack | 4 | ... | ... |
| TCB | 4 | ... | ... |
| Queue storage | 2 | ... | ... |
| Timer | 4 | ... | ... |
| Event pool | ... | ... | ... |

### Tiêu chí hoàn thành

- Tổng RAM không vượt SRAM target.
- Có reserve cho main stack và driver buffer.
- Có giải thích object nào có thể giảm.
- Có so sánh static allocation và dynamic allocation.

---

## Bài 5 — First-fit không split

### Mục tiêu

Cài first-fit đơn giản trước khi thêm splitting.

### Yêu cầu

- Heap có linked block list.
- Một block free chỉ được cấp toàn bộ.
- Search từ đầu list.
- Mark used/free.
- Test exact fit và exhaustion.

### Tiêu chí hoàn thành

- First-fit tìm đúng block đầu tiên.
- Free trả block về trạng thái rảnh.
- Double free được phát hiện.
- Host tests pass với sanitizer.

---

## Bài 6 — Block splitting

### Mục tiêu

Giảm lãng phí khi block lớn hơn request.

### Yêu cầu

- Align requested size.
- Tạo block header mới.
- Cập nhật `previous` và `next`.
- Không tạo block dư quá nhỏ.

### Test quan trọng

```text
Heap 256 bytes
Allocate 32 bytes
Expected:
[USED 32][FREE remaining]
```

### Tiêu chí hoàn thành

- Payload trả về aligned.
- Block list không overlap.
- Exact-fit không split.
- Validator pass sau mỗi allocation.

---

## Bài 7 — Coalescing

### Mục tiêu

Giảm external fragmentation.

### Kịch bản

```text
Allocate A
Allocate B
Allocate C

Free B
Free A
```

Kỳ vọng:

```text
A và B được merge
```

Sau đó:

```text
Free C
```

Kỳ vọng:

```text
Toàn bộ heap trở lại một free block
```

### Tiêu chí hoàn thành

- Forward merge hoạt động.
- Backward merge hoạt động.
- Merge cả hai phía hoạt động.
- Header cũ bị vô hiệu hóa.
- Có test allocation sau coalescing.

---

## Bài 8 — Invalid free và corruption detection

### Mục tiêu

Không để allocator âm thầm hỏng.

### Test

- Free pointer ngoài heap.
- Free pointer nằm giữa block.
- Double free.
- Sửa magic.
- Sửa `next`.
- Ghi quá payload để phá canary.

### Tiêu chí hoàn thành

- Mỗi lỗi có status rõ ràng.
- Validator phát hiện corruption.
- Test không gây undefined behavior ngoài phạm vi kiểm soát.
- Có tài liệu mô tả failure policy.

---

## Bài 9 — Fragmentation experiment

### Mục tiêu

Quan sát internal và external fragmentation.

### Kịch bản gợi ý

```text
Allocate A = 24
Allocate B = 80
Allocate C = 24
Allocate D = 80

Free B
Free D

Try allocate E = 120
```

Ghi lại:

- total free bytes;
- largest free block;
- block count;
- external fragmentation indicator;
- kết quả allocation.

### Tiêu chí hoàn thành

- Giải thích được vì sao tổng free đủ nhưng allocation có thể fail.
- So sánh kết quả trước và sau coalescing.
- In được block map.

---

## Bài 10 — Chạy allocator trên STM32

### Mục tiêu

Tích hợp lab với nền tảng bare-metal.

### Yêu cầu

- Reserve một mảng heap tĩnh.
- Init allocator.
- Chạy chuỗi allocate/free.
- In block map và statistics qua UART.
- Không allocate trong ISR.
- Không allocate trong SysTick.
- Không thay allocator thành dependency của kernel.

### Ví dụ

```c
static uint8_t g_heap_storage[2048]
    __attribute__((aligned(8)));
```

### Tiêu chí hoàn thành

- Firmware build không warning.
- UART hiển thị đúng sequence.
- Heap validator pass.
- Không có HardFault.
- Kết quả host và target nhất quán.

---

# 24. Project tổng kết chủ đề

## Tên project

```text
Memory Explorer
```

## 24.1 Mục tiêu

Xây dựng một ứng dụng bare-metal trên STM32F103 cho phép quan sát trực tiếp:

- Flash/RAM layout;
- section addresses;
- stack usage;
- heap block map;
- allocate/free;
- fragmentation;
- allocator statistics;
- error detection.

## 24.2 Giao diện UART command

Các lệnh gợi ý:

```text
help
memory
heap-init
alloc <size>
free <id>
blocks
stats
validate
fragment-demo
reset
```

Ví dụ:

```text
> alloc 32
id=0 ptr=0x20000480 aligned=32

> alloc 80
id=1 ptr=0x200004C8 aligned=80

> free 0
OK

> blocks
[0] FREE size=32
[1] USED size=80
[2] FREE size=...
```

## 24.3 Kiến trúc

```text
+--------------------+
| UART RX            |
+---------+----------+
          |
          v
+--------------------+
| Command Parser     |
+---------+----------+
          |
          v
+--------------------+
| Memory Explorer    |
+----+----------+----+
     |          |
     v          v
+---------+  +----------------+
| Linker  |  | First-fit Heap |
| Symbols |  | Lab            |
+---------+  +----------------+
     |          |
     +-----+----+
           |
           v
+--------------------+
| UART Report        |
+--------------------+
```

## 24.4 Module

```text
platform/
    uart.c
    gpio.c
    systick.c

memory/
    memory_layout.c
    stack_monitor.c

heap_lab/
    heap_lab.c
    heap_validate.c
    heap_stats.c

app/
    command_parser.c
    memory_explorer.c
    main.c
```

## 24.5 Các yêu cầu bắt buộc

- Không dùng `malloc()` để xây first-fit allocator.
- Vùng heap phải là mảng tĩnh.
- Tất cả allocation trả về phải aligned.
- Có splitting.
- Có coalescing.
- Có invalid-free detection.
- Có double-free detection.
- Có out-of-memory behavior rõ ràng.
- Có block-list validator.
- Có host unit test.
- Có target demo.
- Có README ghi lại kết quả.

## 24.6 Thí nghiệm bắt buộc

### Experiment A — Exact fit

```text
Allocate một block có kích thước đúng bằng free block.
```

### Experiment B — Split

```text
Allocate block nhỏ từ free block lớn.
```

### Experiment C — Fragmentation

```text
Allocate xen kẽ block nhỏ và lớn.
Free các block lớn.
Thử allocate một block rất lớn.
```

### Experiment D — Coalescing

```text
Free các block liền kề.
Xác nhận chúng được merge.
```

### Experiment E — Error detection

```text
Double free
Invalid pointer
Middle-of-block pointer
Corrupted magic
```

## 24.7 Kết quả đầu ra

Project cần xuất được báo cáo dạng:

```text
Memory Explorer
------------------------
Flash start       : 0x08000000
Flash end         : ...
RAM start         : 0x20000000
RAM end           : ...
.data start       : ...
.data end         : ...
.bss start        : ...
.bss end          : ...
main stack top    : ...

Heap
----
total             : 2048
used payload      : ...
free payload      : ...
largest free      : ...
block count       : ...
free block count  : ...
alloc count       : ...
free count        : ...
failed alloc      : ...
fragmentation     : ...
validation        : PASS
```

---

# 25. Liên hệ với roadmap HairRTOS

## Phase 1 — Bare-metal foundation

Chủ đề này sử dụng các deliverable đã có:

- startup;
- vector table;
- linker script;
- clock 72 MHz;
- GPIO;
- UART;
- bare-metal SysTick;
- GNU Arm build;
- Clang validation;
- ELF/BIN/HEX/MAP.

Phase 1 giúp trả lời:

```text
Firmware khởi động như thế nào?
Code và data nằm ở đâu?
Có bao nhiêu RAM?
UART debug hoạt động ra sao?
```

## Phase 14 — Memory allocator lab

Allocator lab bao phủ:

- alignment;
- first-fit;
- block header;
- splitting;
- coalescing;
- invalid free;
- double free;
- exhaustion;
- internal fragmentation;
- external fragmentation;
- statistics;
- randomized host tests.

## Ranh giới kiến trúc

```text
HairRTOS production kernel
        |
        +-- static-first
        +-- không phụ thuộc allocator lab

labs/memory-allocator
        |
        +-- educational
        +-- first-fit
        +-- host-testable
        +-- optional target demo
```

Không được gọi allocator từ:

- PendSV;
- SysTick;
- critical scheduler path;
- ISR;
- context-switch assembly.

---

# 26. Lỗi thường gặp

## 26.1 Nhầm RTOS với hệ thống chạy nhanh

Sai:

```text
Có RTOS => hệ thống chắc chắn real-time
```

Đúng:

```text
RTOS chỉ cung cấp cơ chế.
Thiết kế và kiểm chứng mới quyết định hệ thống có đạt deadline hay không.
```

## 26.2 Dùng dynamic allocation mà không có failure policy

Không tốt:

```c
object = heap_lab_alloc(size);
object->state = READY;
```

Đúng:

```c
object = heap_lab_alloc(size);

if (object == NULL)
{
    handle_allocation_failure();
    return;
}
```

## 26.3 Không align payload

Hậu quả:

- unaligned access;
- performance giảm;
- fault trên một số architecture;
- UB khi cast sang type yêu cầu alignment cao.

## 26.4 Split nhưng cập nhật thiếu liên kết

Ví dụ quên:

```c
new_block->next->previous = new_block;
```

Hậu quả:

- backward traversal sai;
- coalescing sai;
- validator fail.

## 26.5 Coalesce các block không liền kề vật lý

Không được chỉ dựa vào việc hai node đứng cạnh nhau trong free list.

Phải xác nhận địa chỉ vật lý:

```text
end_of(block) == address_of(next)
```

## 26.6 Free pointer vào giữa payload

```c
heap_free(&buffer[4]);
```

Phải bị từ chối.

## 26.7 Dùng `volatile` để thay synchronization

`volatile` không bảo đảm:

- atomicity;
- mutual exclusion;
- memory ordering đầy đủ;
- thread safety.

## 26.8 In UART trong critical path

UART có thể làm:

- timing thay đổi;
- latency tăng;
- test không còn đại diện;
- allocator lock kéo dài.

Nên lưu trace vào RAM rồi in sau.

## 26.9 Dùng allocator lab làm hidden dependency

Không được để task creation âm thầm gọi heap nếu policy công khai là static-first.

API phải thể hiện ownership storage rõ ràng.

---

# 27. Tiêu chí hoàn thành

Người học được xem là hoàn thành chủ đề khi có thể:

## RTOS foundation

- Giải thích RTOS là gì.
- Giải thích real-time khác fast như thế nào.
- Phân biệt hard, firm và soft real-time.
- Giải thích latency, response time, deadline, jitter và WCET.
- So sánh super-loop và RTOS.
- Nêu được trường hợp không cần RTOS.
- Vẽ được kiến trúc tổng quát của một RTOS.

## MCU memory layout

- Phân biệt Flash và SRAM.
- Phân biệt `.text`, `.rodata`, `.data`, `.bss`, stack và heap.
- Đọc được map file.
- Xác định được vector table.
- Xác định được stack top.
- Giải thích được quá trình copy `.data`.
- Giải thích được quá trình clear `.bss`.
- Tính được static RAM budget.

## Allocation

- Phân biệt static, stack, heap và fixed-size pool.
- Giải thích internal fragmentation.
- Giải thích external fragmentation.
- Giải thích memory leak.
- Giải thích double free.
- Giải thích use-after-free.
- Nêu được vì sao dynamic allocation khó dùng trong real-time path.

## First-fit allocator

- Thiết kế block header.
- Align requested size.
- Tìm first-fit block.
- Xử lý exact fit.
- Split block.
- Coalesce forward.
- Coalesce backward.
- Detect invalid pointer.
- Detect double free.
- Xử lý out-of-memory.
- Validate block list.
- Thu thập allocator statistics.
- Đo fragmentation.

## Test và project

- Host tests pass.
- ASan pass.
- UBSan pass.
- Randomized test pass.
- Target demo chạy trên STM32F103.
- UART command hoạt động.
- Có report thí nghiệm.
- Có README trên GitHub.
- Allocator lab không trở thành dependency của production kernel.

---

# 28. Cấu trúc repository đề xuất

```text
01-rtos-introduction-memory-management/
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
|
├── include/
│   ├── command_parser.h
│   ├── compiler.h
│   ├── gpio.h
│   ├── heap.h
│   ├── heap_stats.h
│   ├── memory_explorer.h
│   ├── memory_layout.h
│   ├── stack_monitor.h
│   ├── stm32f1.h
│   ├── systick.h
│   └── uart.h
|
├── src/
│   ├── command_parser.c
│   ├── gpio.c
│   ├── heap.c
│   ├── heap_stats.c
│   ├── main.c
│   ├── memory_explorer.c
│   ├── memory_layout.c
│   ├── runtime.c
│   ├── stack_monitor.c
│   ├── systick.c
│   └── uart.c
|
├── labs/
│   ├── 01-memory-layout/
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── startup/
│   │   ├── linker/
│   │   ├── include/
│   │   └── src/
|   |
│   ├── 02-startup-sections/
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── startup/
│   │   ├── linker/
│   │   ├── include/
│   │   └── src/
|   |
│   ├── 03-stack-high-water/
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── include/
│   │   ├── src/
│   │   └── tests/
|   |
│   ├── 04-static-memory-budget/
│   │   ├── memory-budget.md
│   │   ├── README.md
│   │   ├── Makefile
│   │   └── src/
|   |
│   ├── 05-first-fit-basic/
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── include/
│   │   ├── src/
│   │   └── tests/
|   |
│   ├── 06-block-splitting/
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── include/
│   │   ├── src/
│   │   └── tests/
|   |
│   ├── 07-block-coalescing/
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── include/
│   │   ├── src/
│   │   └── tests/
|   |
│   ├── 08-invalid-free/
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── include/
│   │   ├── src/
│   │   └── tests/
|   |
│   ├── 09-fragmentation/
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── tests/
│   │   ├── include/
│   │   ├── src/
│   │   └── report/
|   |
│   ├── 10-target-demo/
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── startup/
│   │   ├── linker/
│   │   ├── include/
│   │   ├── src/
│   │   └── report/
|   |
│   └── README.md
|
├── docs/
│   ├── allocation-models.md
│   ├── allocator-design.md
│   ├── memory-map.md
│   ├── rtos-introduction.md
│   └── startup-flow.md
|
└── build/

```

---

# 29. Tổng kết

Chủ đề 1 xây dựng nền tảng cho toàn bộ quá trình tự phát triển RTOS.

Luồng kiến thức chính:

```text
RTOS là gì?
    |
    v
Real-time requirements
    |
    v
MCU memory layout
    |
    v
Kernel memory objects
    |
    v
Static vs dynamic allocation
    |
    v
Fragmentation
    |
    v
First-fit allocator
    |
    v
Split + coalesce + validation
    |
    v
Host tests + target demo
```

Các nguyên tắc cần nhớ:

1. Real-time không đồng nghĩa với chạy nhanh.
2. Deadline và worst-case quan trọng hơn thời gian trung bình.
3. RTOS không tự bảo đảm hệ thống đạt real-time.
4. Bộ nhớ của task, queue và timer phải được tính trước.
5. Static allocation cho capacity dễ dự đoán.
6. Dynamic allocation có failure và fragmentation.
7. First-fit dễ học nhưng allocation time không cố định.
8. Split làm giảm lãng phí nhưng tăng độ phức tạp metadata.
9. Coalescing giúp giảm external fragmentation.
10. Invalid free và double free phải được phát hiện.
11. Allocator cần validator và statistics.
12. Host tests nên chạy với ASan và UBSan.
13. Không allocate trong ISR, PendSV hoặc SysTick.
14. Allocator lab không được trở thành hidden dependency của production kernel.
15. Chỉ khi hiểu rõ bộ nhớ mới nên tiếp tục sang TCB, scheduler và context switch.

Sau chủ đề này, người học chưa cần có scheduler chạy thật. Kết quả cần đạt là:

```text
Một bare-metal foundation đáng tin cậy
+
Một mô hình bộ nhớ rõ ràng
+
Một first-fit allocator lab đã được kiểm thử
+
Một project Memory Explorer chạy trên target
```

Đây là nền tảng để tiếp tục sang chủ đề:

```text
Scheduling + Context Switch
```

---

## Nguồn chương trình

README này được xây dựng theo phạm vi Chủ đề 1 trong tài liệu:

```text
RTOS introduction, getting started with AKOS
    - Setup base source to experiment RTOS concepts
    - What is a Real-Time Operating System?
    - Bài toán bộ nhớ trong RTOS
    - Phân loại cấp phát bộ nhớ
    - Fragmentation và memory issues
    - First-fit schema implementation trong AK-mOS
```

Thuật toán và mã minh họa trong README phục vụ mục đích đào tạo, không được xem là allocator production nếu chưa hoàn thiện kiểm thử concurrency, corruption protection, timing analysis và target validation.
