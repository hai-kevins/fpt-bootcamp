# Chủ đề 2 - Scheduling và Context Switch trên ARM Cortex-M3
## Priority Scheduler, SysTick, SVC và PendSV

> Chủ đề này được xây dựng theo phạm vi **“Scheduling + Context Switch”** trong chương trình tự phát triển AKOS.

---

## Mục lục

1. [Mục tiêu học tập](#1-mục-tiêu-học-tập)
2. [Phạm vi của chủ đề](#2-phạm-vi-của-chủ-đề)
3. [Kiến thức cần chuẩn bị](#3-kiến-thức-cần-chuẩn-bị)
4. [Scheduling là gì?](#4-scheduling-là-gì)
5. [Các mô hình scheduling cơ bản](#5-các-mô-hình-scheduling-cơ-bản)
6. [Cooperative và preemptive scheduling](#6-cooperative-và-preemptive-scheduling)
7. [Priority-based scheduling](#7-priority-based-scheduling)
8. [Round-robin giữa các task cùng priority](#8-round-robin-giữa-các-task-cùng-priority)
9. [Kiến trúc scheduler của HairRTOS](#9-kiến-trúc-scheduler-của-hairrtos)
10. [Task, TCB và saved stack pointer](#10-task-tcb-và-saved-stack-pointer)
11. [Kiến trúc ARM Cortex-M3 cần biết](#11-kiến-trúc-arm-cortex-m3-cần-biết)
12. [Thread mode và Handler mode](#12-thread-mode-và-handler-mode)
13. [MSP và PSP](#13-msp-và-psp)
14. [Thanh ghi Cortex-M3 và context của task](#14-thanh-ghi-cortex-m3-và-context-của-task)
15. [Exception entry và hardware stack frame](#15-exception-entry-và-hardware-stack-frame)
16. [Exception return và EXC_RETURN](#16-exception-return-và-exc_return)
17. [Initial task stack frame](#17-initial-task-stack-frame)
18. [Vai trò của SVC](#18-vai-trò-của-svc)
19. [Vai trò của PendSV](#19-vai-trò-của-pendsv)
20. [Vai trò của SysTick](#20-vai-trò-của-systick)
21. [Quan hệ giữa SysTick, scheduler và PendSV](#21-quan-hệ-giữa-systick-scheduler-và-pendsv)
22. [Luồng khởi động task đầu tiên](#22-luồng-khởi-động-task-đầu-tiên)
23. [Luồng cooperative context switch](#23-luồng-cooperative-context-switch)
24. [Luồng preemptive context switch](#24-luồng-preemptive-context-switch)
25. [Thiết kế ready queue và ready bitmap](#25-thiết-kế-ready-queue-và-ready-bitmap)
26. [Scheduler invariants](#26-scheduler-invariants)
27. [Critical section trong scheduler](#27-critical-section-trong-scheduler)
28. [Mã khung scheduler](#28-mã-khung-scheduler)
29. [Mã khung Cortex-M3 port](#29-mã-khung-cortex-m3-port)
30. [Debug context switch bằng GDB](#30-debug-context-switch-bằng-gdb)
31. [Các lỗi thường gặp](#31-các-lỗi-thường-gặp)
32. [Chiến lược kiểm thử](#32-chiến-lược-kiểm-thử)
33. [Bài thực hành](#33-bài-thực-hành)
34. [Project tổng kết chủ đề](#34-project-tổng-kết-chủ-đề)
35. [Cấu trúc repository đề xuất](#35-cấu-trúc-repository-đề-xuất)
36. [Liên hệ với roadmap HairRTOS](#36-liên-hệ-với-roadmap-hairtos)
37. [Tiêu chí hoàn thành](#37-tiêu-chí-hoàn-thành)
38. [Tổng kết](#38-tổng-kết)

---

# 1. Mục tiêu học tập

Sau khi hoàn thành chủ đề này, người học cần có khả năng:

- Giải thích scheduler có trách nhiệm gì trong RTOS.
- Phân biệt:
  - cooperative scheduling;
  - preemptive scheduling;
  - fixed-priority scheduling;
  - round-robin scheduling.
- Giải thích được tại sao task có priority cao hơn phải được chạy trước.
- Giải thích được tại sao task cùng priority cần FIFO hoặc round-robin.
- Hiểu các khái niệm:
  - current task;
  - next task;
  - ready queue;
  - ready bitmap;
  - time slice;
  - yield;
  - preemption;
  - deferred context switch.
- Hiểu các chế độ thực thi của Cortex-M3:
  - Thread mode;
  - Handler mode.
- Phân biệt:
  - MSP;
  - PSP.
- Phân biệt phần context được CPU tự động lưu và phần kernel phải tự lưu.
- Giải thích hardware exception frame:
  - `R0-R3`;
  - `R12`;
  - `LR`;
  - `PC`;
  - `xPSR`.
- Giải thích tại sao `R4-R11` được PendSV lưu thủ công.
- Tạo được initial task stack frame.
- Dùng SVC để khởi động task đầu tiên.
- Dùng PendSV để thực hiện context switch.
- Dùng SysTick để tạo kernel tick và yêu cầu scheduling.
- Thiết lập PendSV ở mức priority thấp nhất phù hợp.
- Cài đặt được cooperative context switching.
- Cài đặt được fixed-priority scheduler.
- Cài đặt được preemption cơ bản.
- Cài đặt round-robin giữa các task cùng priority.
- Kiểm tra task giữ được:
  - biến cục bộ;
  - vị trí thực thi;
  - stack riêng;
  - register context.
- Debug được lỗi:
  - HardFault;
  - stack alignment;
  - sai `EXC_RETURN`;
  - sai saved stack pointer;
  - hỏng `R4-R11`;
  - priority configuration sai.
- Viết host tests cho scheduler policy trước khi ghép với assembly.

---

# 2. Phạm vi của chủ đề

Theo chương trình tự phát triển RTOS, Chủ đề 2 gồm:

```text
Scheduling + Context Switch
    |
    +-- Priority-based scheduling models
    +-- ARM Cortex-M3 architecture cơ bản
    +-- SysTick timer trong scheduling
    +-- PendSV handler cho context switch
    +-- Context switch implementation trong RTOS
```

README này mở rộng phạm vi thực hành theo hướng:

```text
Lý thuyết scheduling
        |
        v
Cortex-M3 exception model
        |
        v
Initial task stack
        |
        v
SVC start-first-task
        |
        v
PendSV cooperative switch
        |
        v
Fixed-priority scheduler
        |
        v
SysTick preemption
        |
        v
Round-robin
        |
        v
Stress test và project tổng kết
```

## Ranh giới của chủ đề

Chủ đề này tập trung vào:

- scheduler decision;
- task context;
- Cortex-M3 port;
- SVC;
- PendSV;
- SysTick;
- cooperative switch;
- preemptive switch;
- fixed priority;
- equal-priority round-robin.

Các nội dung sau chỉ được dùng ở mức tối thiểu hoặc để mô phỏng:

- task state;
- delay;
- timeout;
- wait list.

Các nội dung chưa triển khai đầy đủ trong chủ đề này:

- queue;
- semaphore;
- mutex;
- priority inheritance;
- software timer;
- event framework;
- dynamic event pool.

---

# 3. Kiến thức cần chuẩn bị

## 3.1 Từ Chủ đề 1

Người học cần hiểu:

- startup code;
- vector table;
- linker script;
- Flash và SRAM;
- `.data`;
- `.bss`;
- main stack;
- stack alignment;
- register-level programming;
- UART debug;
- SysTick bare-metal cơ bản.

## 3.2 Ngôn ngữ C

- `struct`;
- pointer;
- pointer arithmetic;
- function pointer;
- `enum`;
- bit mask;
- `volatile`;
- `static`;
- `const`;
- integer width;
- `uintptr_t`;
- alignment;
- assert;
- intrusive list hoặc queue cơ bản.

## 3.3 Assembly Cortex-M cơ bản

Cần đọc được các lệnh:

```asm
MRS
MSR
LDR
STR
STMDB
LDMIA
PUSH
POP
BL
BX
CPSID
CPSIE
ISB
DSB
```

Không cần viết toàn bộ firmware bằng assembly. Chỉ phần chuyển context cần assembly hoặc naked function có kiểm soát.

## 3.4 Công cụ

- `arm-none-eabi-gcc` hoặc Clang cross-compiler;
- Makefile;
- GDB;
- OpenOCD;
- ST-Link;
- UART terminal;
- `objdump`;
- `nm`;
- `readelf`;
- logic analyzer là tùy chọn.

---

# 4. Scheduling là gì?

Scheduler là thành phần quyết định task nào được sử dụng CPU tiếp theo.

```text
Ready tasks
    |
    v
Scheduler policy
    |
    v
Selected next task
    |
    v
Context switch
    |
    v
CPU runs selected task
```

Scheduler không nhất thiết trực tiếp lưu hoặc phục hồi register.

Trong thiết kế HairRTOS:

```text
Scheduler
    |
    +-- quản lý policy;
    +-- chọn TCB tiếp theo;
    +-- không lưu R4-R11;
    +-- không ghi PSP;
    +-- không thực hiện exception return.

Architecture port
    |
    +-- lưu register;
    +-- đổi saved stack pointer;
    +-- phục hồi register;
    +-- return về task.
```

Cần phân biệt:

```text
Scheduling decision != Context-switch mechanism
```

## Scheduling decision

Trả lời:

```text
Task nào nên chạy tiếp?
```

## Context-switch mechanism

Trả lời:

```text
Làm cách nào dừng task hiện tại và tiếp tục task được chọn?
```

---

# 5. Các mô hình scheduling cơ bản

## 5.1 Cooperative scheduling

Task đang chạy tiếp tục dùng CPU cho tới khi:

- tự gọi `yield()`;
- tự block;
- kết thúc;
- hoặc chủ động chuyển quyền.

```text
Task A ---- yield ----> Task B ---- yield ----> Task A
```

Ưu điểm:

- dễ triển khai;
- dễ debug;
- ít preemption point;
- phù hợp bước đầu học context switch.

Nhược điểm:

- task lỗi có thể giữ CPU mãi;
- latency phụ thuộc sự hợp tác của task;
- task priority cao có thể phải chờ task thấp yield.

## 5.2 Preemptive scheduling

Kernel có thể dừng task hiện tại khi task phù hợp hơn trở thành READY.

```text
Low task running
        |
        v
High task becomes READY
        |
        v
PendSV requested
        |
        v
High task preempts Low task
```

Ưu điểm:

- phản hồi tốt hơn cho task priority cao;
- phù hợp hệ thống có deadline;
- task thấp không cần tự yield để task cao được chạy.

Nhược điểm:

- concurrency phức tạp hơn;
- shared data dễ race;
- cần critical section;
- debug khó hơn;
- context switch có thể xảy ra tại nhiều vị trí.

## 5.3 Fixed-priority scheduling

Mỗi task có priority cố định hoặc có effective priority được tính từ base priority.

HairRTOS định nghĩa:

```text
Số priority nhỏ hơn
=
Priority cao hơn
```

Ví dụ:

```text
Priority 0: cao nhất
Priority 1
Priority 2
Priority 3
Priority 4: idle hoặc thấp nhất
```

## 5.4 Round-robin

Các task cùng priority luân phiên theo FIFO hoặc time slice.

```text
Priority 2 ready queue:

[A][B][C]

After A time slice:
[B][C][A]
```

## 5.5 Earliest Deadline First

EDF chọn task có deadline gần nhất.

EDF là một mô hình scheduling quan trọng nhưng không phải policy đầu tiên của HairRTOS. Chủ đề này tập trung fixed-priority scheduling.

---

# 6. Cooperative và preemptive scheduling

## 6.1 Cooperative switch

Trigger:

```text
hr_task_yield()
```

Luồng:

```text
Task A
  |
  v
hr_task_yield()
  |
  v
Pend PendSV
  |
  v
PendSV saves Task A
  |
  v
Scheduler selects Task B
  |
  v
PendSV restores Task B
```

## 6.2 Preemptive switch

Trigger có thể là:

- SysTick;
- ISR làm task priority cao READY;
- kernel service giải phóng task priority cao.

Luồng:

```text
Interrupt
  |
  v
Task High becomes READY
  |
  v
Set PendSV pending
  |
  v
ISR finishes
  |
  v
PendSV runs
  |
  v
Task High starts
```

Điểm quan trọng:

> Không context-switch trực tiếp trong ISR thiết bị.

ISR chỉ:

- cập nhật trạng thái tối thiểu;
- đánh dấu task READY nếu API cho phép;
- đặt cờ `higher_priority_task_woken`;
- pend PendSV.

PendSV thực hiện switch sau khi exception hiện tại hoàn tất.

---

# 7. Priority-based scheduling

Giả sử ready tasks:

| Task | Priority | State |
|---|---:|---|
| A | 0 | READY |
| B | 1 | READY |
| C | 1 | READY |
| D | 3 | READY |

Task được chọn:

```text
Task A
```

Nếu A block:

```text
Task B
```

Nếu B hết time slice:

```text
Task C
```

Nếu A trở lại READY khi C đang chạy:

```text
A preempts C
```

## 7.1 Không để lower-priority task chạy khi higher-priority task vẫn READY

`yield()` không có nghĩa là nhường CPU cho mọi priority.

Nếu task priority 1 gọi yield trong khi chính nó vẫn READY và không có task khác cùng priority:

```text
Task priority 3 không được chạy.
```

Lý do:

```text
Priority 1 vẫn là highest-priority READY task.
```

## 7.2 Starvation

Task priority thấp có thể bị starvation nếu task priority cao luôn READY.

Cách xử lý không nằm hoàn toàn trong scheduler core. Application cần:

- block khi không có việc;
- dùng delay;
- thiết kế priority đúng;
- giới hạn CPU usage;
- tránh busy loop.

---

# 8. Round-robin giữa các task cùng priority

Giả sử:

```text
Ready queue priority 2:
[A][B][C]
```

Task A đang chạy.

Khi A:

- hết time slice;
- hoặc gọi yield;

queue trở thành:

```text
[B][C][A]
```

Scheduler chọn B.

## 8.1 FIFO invariant

Task mới READY ở cùng priority thường được thêm vào cuối queue:

```text
Before:
[A][B]

Task C becomes READY:
[A][B][C]
```

## 8.2 Time slice

Mỗi task cùng priority có thể được cấp:

```text
N kernel ticks
```

Ví dụ:

```text
Time slice = 2 ticks

Tick 0-1: Task A
Tick 2-3: Task B
Tick 4-5: Task C
```

## 8.3 Không round-robin giữa priority khác nhau

Round-robin chỉ áp dụng trong cùng priority.

```text
Priority 1: A
Priority 2: B, C
```

Nếu A luôn READY:

```text
B và C không chạy.
```

---

# 9. Kiến trúc scheduler của HairRTOS

HairRTOS sử dụng:

```text
Fixed-priority preemptive scheduler
```

Policy:

- số priority nhỏ hơn là cao hơn;
- mỗi priority có FIFO ready queue;
- ready bitmap ghi lại queue nào không rỗng;
- next task là head của queue có priority cao nhất;
- task cùng priority xoay vòng theo time slice;
- idle task luôn READY ở priority thấp nhất được dành riêng.

Kiến trúc:

```text
+-----------------------------------------------+
| Ready bitmap                                  |
| bit 0 bit 1 bit 2 bit 3 ...                   |
+-----------------------------------------------+
        |      |      |
        v      v      v
     Queue 0 Queue 1 Queue 2 ...
        |      |      |
        v      v      v
       TCB    TCB    TCB
```

Scheduler chỉ chọn TCB:

```c
hr_task_t *hr_scheduler_select_next(void);
```

PendSV sử dụng TCB được chọn để đổi stack context.

---

# 10. Task, TCB và saved stack pointer

Task tối thiểu cần:

```text
Task Control Block
+
Task stack
```

TCB khái quát:

```c
typedef struct hr_task
{
    uint32_t *saved_sp;          /* Phải ở field đầu tiên nếu assembly phụ thuộc offset 0. */

    uint32_t *stack_low;
    uint32_t *stack_high;

    const char *name;

    uint8_t state;
    uint8_t base_priority;
    uint8_t effective_priority;

    uint32_t time_slice_remaining;

    struct hr_task *ready_next;
    struct hr_task *ready_previous;
} hr_task_t;
```

## 10.1 Tại sao `saved_sp` quan trọng?

Khi Task A bị dừng:

```text
PSP sau khi save R4-R11
        |
        v
Task A TCB.saved_sp
```

Khi Task B được chạy:

```text
Task B TCB.saved_sp
        |
        v
Restore R4-R11
        |
        v
Write PSP
```

## 10.2 Saved stack pointer nên là field đầu tiên

Nếu assembly dùng:

```asm
str r0, [r2]
```

thì assembly giả định `saved_sp` ở offset 0.

Nếu thay đổi layout TCB mà không sửa assembly:

```text
Context switch corruption
```

Nên có compile-time assert:

```c
_Static_assert(offsetof(hr_task_t, saved_sp) == 0U,
               "saved_sp must be the first TCB field");
```

---

# 11. Kiến trúc ARM Cortex-M3 cần biết

Cortex-M3 cung cấp exception model rất phù hợp cho RTOS nhỏ.

Các exception quan trọng:

```text
Reset
NMI
HardFault
SVCall
PendSV
SysTick
External IRQs
```

RTOS thường sử dụng:

| Exception | Vai trò |
|---|---|
| SVC | Khởi động task đầu tiên hoặc system call |
| PendSV | Context switch trì hoãn |
| SysTick | Kernel tick |
| External IRQ | Sự kiện phần cứng |

## 11.1 Thumb state

Cortex-M chỉ thực thi Thumb instruction set.

Initial `xPSR` phải đặt Thumb bit:

```c
#define INITIAL_XPSR (1UL << 24)
```

Nếu bit T không được đặt:

```text
Exception return có thể dẫn tới fault.
```

## 11.2 Stack alignment

AAPCS và exception model yêu cầu stack alignment phù hợp, thông thường 8 byte tại boundary quan trọng.

Initial task stack cần làm tròn xuống:

```c
sp = (uint32_t *)((uintptr_t)stack_end & ~(uintptr_t)0x7U);
```

## 11.3 Không có FPU context

STM32F103/Cortex-M3 không có FPU context cần lưu.

Do đó context cơ bản:

```text
Hardware frame: R0-R3, R12, LR, PC, xPSR
Software frame: R4-R11
```

---

# 12. Thread mode và Handler mode

## Thread mode

Thread mode chạy:

- `main()`;
- task code;
- application code.

Task HairRTOS chạy ở Thread mode.

## Handler mode

Handler mode chạy:

- SVC handler;
- PendSV handler;
- SysTick handler;
- external IRQ;
- fault handler.

```text
Thread mode
    |
    | exception entry
    v
Handler mode
    |
    | exception return
    v
Thread mode
```

## Privilege

Thiết kế đầu tiên có thể chạy task ở privileged Thread mode.

Unprivileged task và MPU là phần mở rộng, không bắt buộc cho chủ đề này.

---

# 13. MSP và PSP

Cortex-M3 có hai stack pointer:

```text
MSP — Main Stack Pointer
PSP — Process Stack Pointer
```

## 13.1 MSP

MSP dùng cho:

- reset;
- startup;
- `main()` trước kernel start;
- Handler mode;
- exception handlers.

## 13.2 PSP

PSP dùng cho task.

```text
Task A -> PSP points into Stack A
Task B -> PSP points into Stack B
```

## 13.3 Contract của HairRTOS

```text
Task code       -> PSP
Exception code  -> MSP
```

Lợi ích:

- task stack tách khỏi handler stack;
- context switch chỉ thay PSP;
- exception nesting vẫn dùng MSP;
- stack corruption dễ phân tích hơn.

---

# 14. Thanh ghi Cortex-M3 và context của task

## 14.1 General-purpose registers

```text
R0-R3    argument, return value, temporary
R4-R11   callee-saved
R12      temporary
R13      SP
R14      LR
R15      PC
xPSR     program status
```

## 14.2 Hardware-saved registers

Khi exception entry, CPU tự động stack:

```text
R0
R1
R2
R3
R12
LR
PC
xPSR
```

## 14.3 Software-saved registers

PendSV lưu thủ công:

```text
R4-R11
```

## 14.4 Context hoàn chỉnh

```text
Task context
+--------------------------------+
| R4-R11                         | Software frame
+--------------------------------+
| R0-R3, R12, LR, PC, xPSR       | Hardware frame
+--------------------------------+
```

## 14.5 Các register đặc biệt

Một kernel nâng cao có thể cần xem xét:

- CONTROL;
- PRIMASK;
- BASEPRI;
- FAULTMASK;
- critical nesting;
- floating-point registers trên core có FPU.

Trong phiên bản đầu:

- tasks privileged;
- no FPU;
- critical sections không được giữ xuyên qua context switch;
- không cần lưu toàn bộ special registers cho mỗi task.

---

# 15. Exception entry và hardware stack frame

Giả sử Task A đang chạy bằng PSP.

Khi SysTick xảy ra:

```text
Task A registers
        |
        v
CPU tự động push hardware frame vào PSP
        |
        v
CPU chuyển sang Handler mode
        |
        v
Handler sử dụng MSP
```

Stack Task A sau exception entry:

```text
High address
+------------------+
| xPSR             |
+------------------+
| PC               |
+------------------+
| LR               |
+------------------+
| R12              |
+------------------+
| R3               |
+------------------+
| R2               |
+------------------+
| R1               |
+------------------+
| R0               |
+------------------+ <- PSP sau hardware stacking
Low address
```

Nếu PendSV tiếp tục save R4-R11:

```text
High address
+------------------+
| Hardware frame   |
+------------------+
| R11              |
| ...              |
| R4               |
+------------------+ <- saved_sp
Low address
```

Lưu ý:

> Hình biểu diễn địa chỉ và thứ tự phải được đối chiếu với code `STMDB/LDMIA`.  
> Điều quan trọng là save và restore phải đối xứng.

---

# 16. Exception return và EXC_RETURN

Khi exception xảy ra, `LR` trong handler không phải địa chỉ hàm thông thường. Nó chứa giá trị `EXC_RETURN`.

Các giá trị thường gặp:

```text
0xFFFFFFF1
Return to Handler mode using MSP

0xFFFFFFF9
Return to Thread mode using MSP

0xFFFFFFFD
Return to Thread mode using PSP
```

RTOS thường dùng:

```text
0xFFFFFFFD
```

để quay về task ở Thread mode bằng PSP.

## 16.1 Tại sao không `return` như hàm C bình thường?

Exception return phải yêu cầu CPU:

- unstack hardware frame;
- phục hồi `R0-R3`;
- phục hồi `R12`;
- phục hồi `LR`;
- phục hồi `PC`;
- phục hồi `xPSR`;
- đổi mode phù hợp;
- chọn PSP hoặc MSP.

Lệnh:

```asm
bx lr
```

với `LR = EXC_RETURN` kích hoạt cơ chế đó.

---

# 17. Initial task stack frame

Một task chưa từng chạy không có hardware frame thật.

Kernel phải tạo một stack frame giả để lần exception return đầu tiên trông giống task đã từng bị interrupt.

## 17.1 Stack contract

Initial frame cần:

```text
xPSR -> Thumb bit set
PC   -> task entry
LR   -> task-return error handler
R0   -> task argument
R1-R3, R12 -> initial values
R4-R11     -> initial values
```

## 17.2 Mã khung

```c
typedef void (*hr_task_entry_t)(void *argument);

static uint32_t *hr_port_build_initial_stack(
    uint32_t *stack_begin,
    size_t stack_word_count,
    hr_task_entry_t entry,
    void *argument)
{
    uint32_t *sp;

    if ((stack_begin == NULL) ||
        (stack_word_count < 32U) ||
        (entry == NULL))
    {
        return NULL;
    }

    sp = stack_begin + stack_word_count;
    sp = (uint32_t *)((uintptr_t)sp & ~(uintptr_t)0x7U);

    /* Hardware exception frame. */
    *(--sp) = 0x01000000UL;                  /* xPSR */
    *(--sp) = (uint32_t)(uintptr_t)entry;    /* PC */
    *(--sp) = (uint32_t)(uintptr_t)
              hr_task_return_error;          /* LR */
    *(--sp) = 0x12121212UL;                  /* R12 */
    *(--sp) = 0x03030303UL;                  /* R3 */
    *(--sp) = 0x02020202UL;                  /* R2 */
    *(--sp) = 0x01010101UL;                  /* R1 */
    *(--sp) = (uint32_t)(uintptr_t)argument; /* R0 */

    /* Software-saved frame restored by LDMIA {R4-R11}. */
    *(--sp) = 0x11111111UL; /* R11 */
    *(--sp) = 0x10101010UL; /* R10 */
    *(--sp) = 0x09090909UL; /* R9 */
    *(--sp) = 0x08080808UL; /* R8 */
    *(--sp) = 0x07070707UL; /* R7 */
    *(--sp) = 0x06060606UL; /* R6 */
    *(--sp) = 0x05050505UL; /* R5 */
    *(--sp) = 0x04040404UL; /* R4 */

    return sp;
}
```

## 17.3 Task return error handler

Task entry không nên return trong RTOS nhỏ.

```c
__attribute__((noreturn))
void hr_task_return_error(void)
{
    hr_kernel_panic(HR_PANIC_TASK_RETURNED);

    for (;;)
    {
        __asm volatile("bkpt 0");
    }
}
```

## 17.4 Test initial frame trên host

Có thể kiểm tra:

- `saved_sp` aligned 8 byte;
- slot R0 chứa argument;
- slot PC chứa task entry;
- xPSR bit 24 được set;
- LR chứa task-return handler;
- stack không vượt range.

---

# 18. Vai trò của SVC

SVC:

```text
Supervisor Call
```

Trong chủ đề này, SVC dùng để khởi động task đầu tiên.

## 18.1 Vì sao cần SVC?

Trước kernel start:

```text
main() chạy Thread mode bằng MSP
```

Task cần:

```text
Thread mode bằng PSP
+
hardware frame được exception return phục hồi
```

SVC tạo một exception boundary hợp lệ:

```text
main()
  |
  v
SVC instruction
  |
  v
SVC_Handler on MSP
  |
  v
Load first task PSP
  |
  v
EXC_RETURN 0xFFFFFFFD
  |
  v
First task on PSP
```

## 18.2 Kernel start contract

```c
void hr_kernel_start(void)
{
    hr_task_t *first;

    hr_kernel_validate();

    first = hr_scheduler_select_next();
    g_current_task = first;

    hr_port_start_first_task();

    hr_kernel_panic(HR_PANIC_KERNEL_START_RETURNED);
}
```

`hr_kernel_start()` không được return trong hoạt động bình thường.

---

# 19. Vai trò của PendSV

PendSV:

```text
Pendable Service
```

Đây là exception được thiết kế phù hợp cho context switch trì hoãn.

## 19.1 Tại sao dùng PendSV?

Giả sử UART ISR làm task priority cao READY.

Không nên switch giữa lúc UART ISR chưa xong.

Thay vào đó:

```text
UART ISR
  |
  +-- update kernel state
  +-- pend PendSV
  |
  v
UART ISR return
  |
  v
PendSV runs at lowest priority
  |
  v
Context switch
```

## 19.2 PendSV phải có priority thấp

PendSV nên có mức priority cấu hình thấp nhất để:

- không preempt ISR quan trọng;
- chỉ chạy sau ISR đang hoạt động;
- gom nhiều yêu cầu switch thành một lần;
- context switch tại boundary dễ kiểm soát.

## 19.3 PendSV không được làm gì?

PendSV không nên:

- allocate memory;
- gọi UART;
- gọi GPIO application callback;
- chạy state machine;
- block;
- chờ lock;
- gọi `printf`;
- thực hiện logic application;
- chạy callback timer.

PendSV chỉ nên:

```text
save
select
restore
return
```

---

# 20. Vai trò của SysTick

SysTick là timer hệ thống tích hợp trong Cortex-M.

Trong kernel, SysTick thường:

- tăng kernel tick;
- cập nhật time slice;
- release task theo thời gian;
- xử lý timeout tối thiểu;
- yêu cầu PendSV khi cần.

## 20.1 SysTick không trực tiếp đổi context

Không tốt:

```text
SysTick_Handler
    |
    +-- save full task context
    +-- restore task khác
```

Khuyến nghị:

```text
SysTick_Handler
    |
    +-- update tick
    +-- scheduler bookkeeping
    +-- pend PendSV
```

## 20.2 Tick frequency

Ví dụ:

```text
CPU clock = 72 MHz
Kernel tick = 1 kHz
Reload = 72000 - 1
```

Công thức tổng quát:

```text
reload = core_clock_hz / tick_hz - 1
```

Cần kiểm tra:

- nguồn clock SysTick;
- reload không vượt giới hạn counter;
- tick thực tế bằng logic analyzer hoặc GPIO;
- sai số clock;
- overflow của tick counter.

---

# 21. Quan hệ giữa SysTick, scheduler và PendSV

```text
SysTick exception
        |
        v
Increment kernel tick
        |
        v
Update current time slice
        |
        v
Any higher-priority task READY?
        |
        +-- no --> return
        |
        +-- yes
              |
              v
         Pend PendSV
              |
              v
      SysTick exception return
              |
              v
         PendSV handler
              |
              v
       Save current context
              |
              v
        Select next task
              |
              v
       Restore next context
```

## 21.1 SysTick priority và PendSV priority

PendSV phải thấp hơn SysTick.

Ví dụ quan hệ logic:

```text
External safety IRQ    higher priority
UART IRQ
SysTick
PendSV                 lowest priority
```

Giá trị số cụ thể phụ thuộc số bit priority được implement và cách NVIC encode priority.

Không nên hard-code shift mà không đọc tài liệu MCU hoặc CMSIS definition.

---

# 22. Luồng khởi động task đầu tiên

```text
Reset
  |
  v
Startup initializes .data and .bss
  |
  v
main() runs on MSP
  |
  v
Create static task TCBs and stacks
  |
  v
Build initial stack frames
  |
  v
Insert tasks into ready queues
  |
  v
Select highest-priority ready task
  |
  v
Trigger SVC
  |
  v
SVC_Handler
  |
  v
Load first task saved_sp
  |
  v
Restore R4-R11
  |
  v
Write PSP
  |
  v
Set exception return to Thread + PSP
  |
  v
CPU restores hardware frame
  |
  v
Task entry(argument)
```

## 22.1 Task entry argument

Initial R0 phải chứa:

```c
void *argument
```

Ví dụ:

```c
static void worker_task(void *argument)
{
    worker_config_t *config = argument;
}
```

## 22.2 Task không quay lại `main()`

Sau kernel start:

```text
main() không tiếp tục vòng lặp ứng dụng.
```

CPU chạy task, ISR và kernel exception.

---

# 23. Luồng cooperative context switch

Task A gọi:

```c
hr_task_yield();
```

Luồng:

```text
Task A on PSP
  |
  v
Mark current priority for rotation
  |
  v
Set PendSV pending
  |
  v
Return to Task A briefly
  |
  v
PendSV exception entry
  |
  v
CPU saves hardware frame to Task A PSP
  |
  v
PendSV saves R4-R11
  |
  v
Store PSP into Task A TCB
  |
  v
Rotate ready queue
  |
  v
Select Task B
  |
  v
Load Task B saved_sp
  |
  v
Restore R4-R11
  |
  v
Write PSP
  |
  v
Exception return
  |
  v
CPU restores Task B hardware frame
  |
  v
Task B continues
```

## 23.1 Điều cần chứng minh

Hai task có biến local:

```c
void task_a(void *argument)
{
    uint32_t local_a = 0U;

    for (;;)
    {
        local_a++;
        hr_task_yield();
    }
}
```

```c
void task_b(void *argument)
{
    uint32_t local_b = 1000U;

    for (;;)
    {
        local_b += 2U;
        hr_task_yield();
    }
}
```

Sau hàng triệu switch:

- `local_a` tăng đúng;
- `local_b` tăng đúng;
- PC của từng task được giữ;
- stack không overlap;
- guard word không bị phá.

---

# 24. Luồng preemptive context switch

Giả sử Task Low đang chạy.

```text
Task Low running
        |
        v
SysTick or external IRQ
        |
        v
Task High becomes READY
        |
        v
Compare priorities
        |
        v
higher_priority_task_woken = true
        |
        v
Pend PendSV
        |
        v
Return from active ISR
        |
        v
PendSV
        |
        v
Switch Low -> High
```

## 24.1 Deferred preemption

Preemption bị trì hoãn tới sau ISR:

```text
ISR không bị ngắt giữa logic cập nhật kernel.
```

## 24.2 Không cần yield từ task thấp

Task High không chờ Task Low gọi `yield()`.

Đây là khác biệt cốt lõi giữa:

```text
cooperative
```

và:

```text
preemptive
```

---

# 25. Thiết kế ready queue và ready bitmap

## 25.1 Ready queue theo priority

```c
typedef struct
{
    hr_task_t *head;
    hr_task_t *tail;
    size_t count;
} hr_ready_queue_t;
```

Kernel có:

```c
static hr_ready_queue_t g_ready_queues[HR_PRIORITY_COUNT];
```

## 25.2 Ready bitmap

```c
static uint32_t g_ready_bitmap;
```

Nếu priority 0, 2 và 5 có task:

```text
bit 0 = 1
bit 2 = 1
bit 5 = 1
```

## 25.3 Tìm highest ready priority

Nếu số nhỏ hơn là priority cao hơn:

```c
static uint8_t highest_ready_priority(uint32_t bitmap)
{
    return (uint8_t)__builtin_ctz(bitmap);
}
```

Cần có fallback portable nếu compiler builtin không được dùng.

Không gọi `ctz(0)`.

Idle task bảo đảm bitmap không bằng 0 khi kernel RUNNING.

## 25.4 Enqueue

```c
static void ready_enqueue_tail(hr_task_t *task)
{
    hr_ready_queue_t *queue;

    queue = &g_ready_queues[task->effective_priority];

    task->ready_next = NULL;
    task->ready_previous = queue->tail;

    if (queue->tail != NULL)
    {
        queue->tail->ready_next = task;
    }
    else
    {
        queue->head = task;
    }

    queue->tail = task;
    queue->count++;

    g_ready_bitmap |=
        (1UL << task->effective_priority);
}
```

## 25.5 Rotate current priority

```c
static void ready_rotate(uint8_t priority)
{
    hr_ready_queue_t *queue = &g_ready_queues[priority];
    hr_task_t *first;

    if (queue->count <= 1U)
    {
        return;
    }

    first = queue->head;

    queue->head = first->ready_next;
    queue->head->ready_previous = NULL;

    first->ready_next = NULL;
    first->ready_previous = queue->tail;
    queue->tail->ready_next = first;
    queue->tail = first;
}
```

---

# 26. Scheduler invariants

Scheduler cần bảo vệ các invariant:

- RUNNING task nằm ở head của ready queue tương ứng.
- READY task thuộc đúng một ready queue.
- BLOCKED task không nằm trong ready queue.
- Một task không nằm trong nhiều ready queue.
- Ready bitmap khớp với trạng thái rỗng của queue.
- Queue count khớp số node thực tế.
- `head == NULL` khi và chỉ khi `tail == NULL`.
- Idle task luôn READY.
- Idle task không được block.
- Current task không được `NULL` khi kernel RUNNING.
- `current_task->state == RUNNING`.
- Task được chọn phải ở trạng thái READY hoặc RUNNING theo transition nội bộ.
- Priority phải nằm trong range.
- Saved stack pointer phải nằm trong stack range.
- Saved stack pointer phải aligned.
- Stack guard phải còn nguyên trước khi restore.

## 26.1 Chỉ helper nội bộ được đổi state

Application không được:

```c
task->state = HR_TASK_READY;
```

Nên dùng:

```c
hr_task_transition_to_ready(task);
```

Helper xử lý đồng thời:

- state;
- list membership;
- bitmap;
- statistics;
- assertions.

---

# 27. Critical section trong scheduler

Scheduler structures có thể được truy cập từ:

- task context;
- SysTick;
- ISR-safe service;
- PendSV selection path.

Cần ngăn race.

## 27.1 PRIMASK-based critical section

Phiên bản đầu có thể:

```c
uint32_t hr_port_critical_enter(void)
{
    uint32_t primask;

    __asm volatile(
        "mrs %0, primask\n"
        "cpsid i\n"
        : "=r"(primask)
        :
        : "memory");

    return primask;
}
```

```c
void hr_port_critical_exit(uint32_t state)
{
    __asm volatile(
        "msr primask, %0\n"
        :
        : "r"(state)
        : "memory");
}
```

Điểm quan trọng:

- lưu trạng thái interrupt cũ;
- hỗ trợ nested use ở mức policy;
- không luôn `cpsie i` khi exit;
- critical section phải ngắn;
- không block;
- không gọi user callback;
- không `printf`.

## 27.2 BASEPRI

BASEPRI cho phép mask một nhóm priority mà vẫn cho IRQ rất cao chạy.

Đây là tối ưu sau khi PRIMASK implementation đã đúng và interrupt priority contract đã rõ.

Không nên dùng BASEPRI khi chưa hiểu:

- NVIC priority encoding;
- implemented priority bits;
- kernel-aware IRQ priority range.

---

# 28. Mã khung scheduler

> Đây là mã khung đào tạo. Nó chưa bao gồm đầy đủ task state, timeout, mutex priority inheritance và SMP.

## 28.1 Global state

```c
static hr_ready_queue_t g_ready_queues[HR_PRIORITY_COUNT];
static uint32_t g_ready_bitmap;
static hr_task_t *g_current_task;
static bool g_rotation_requested;
```

## 28.2 Select next

```c
hr_task_t *hr_scheduler_select_next(void)
{
    uint8_t priority;
    hr_ready_queue_t *queue;

    HR_ASSERT(g_ready_bitmap != 0U);

    priority = highest_ready_priority(g_ready_bitmap);
    queue = &g_ready_queues[priority];

    HR_ASSERT(queue->head != NULL);

    if (g_rotation_requested &&
        (g_current_task != NULL) &&
        (g_current_task->effective_priority == priority))
    {
        ready_rotate(priority);
    }

    g_rotation_requested = false;

    return queue->head;
}
```

## 28.3 Request yield

```c
void hr_task_yield(void)
{
    uint32_t state;

    state = hr_port_critical_enter();

    g_rotation_requested = true;
    hr_port_request_context_switch();

    hr_port_critical_exit(state);
}
```

## 28.4 Preemption decision

```c
bool hr_scheduler_should_preempt(const hr_task_t *newly_ready)
{
    if ((newly_ready == NULL) ||
        (g_current_task == NULL))
    {
        return false;
    }

    return newly_ready->effective_priority <
           g_current_task->effective_priority;
}
```

## 28.5 Time-slice update

```c
bool hr_scheduler_tick(void)
{
    hr_task_t *current = g_current_task;

    if (current == NULL)
    {
        return false;
    }

    if (current->time_slice_remaining > 0U)
    {
        current->time_slice_remaining--;
    }

    if (current->time_slice_remaining != 0U)
    {
        return false;
    }

    current->time_slice_remaining = HR_TIME_SLICE_TICKS;

    if (ready_count(current->effective_priority) > 1U)
    {
        g_rotation_requested = true;
        return true;
    }

    return false;
}
```

---

# 29. Mã khung Cortex-M3 port

> Phần assembly cần được kiểm tra bằng disassembly và GDB.  
> Không copy trực tiếp vào production kernel nếu chưa xác nhận ABI, alignment, symbol naming và compiler behavior.

## 29.1 Request PendSV

```c
#define SCB_ICSR_ADDRESS       (0xE000ED04UL)
#define SCB_ICSR_PENDSVSET     (1UL << 28)

static inline void hr_port_request_context_switch(void)
{
    *(volatile uint32_t *)SCB_ICSR_ADDRESS =
        SCB_ICSR_PENDSVSET;

    __asm volatile("dsb" ::: "memory");
    __asm volatile("isb" ::: "memory");
}
```

## 29.2 SVC trigger

```c
__attribute__((noreturn))
void hr_port_start_first_task(void)
{
    __asm volatile("svc 0");

    for (;;)
    {
    }
}
```

## 29.3 SVC handler skeleton

```asm
.syntax unified
.thumb

.global SVC_Handler
.type SVC_Handler, %function

SVC_Handler:
    ldr     r0, =g_current_task
    ldr     r0, [r0]
    ldr     r0, [r0]          /* r0 = current->saved_sp */

    ldmia   r0!, {r4-r11}
    msr     psp, r0
    isb

    movs    r0, #2            /* CONTROL.SPSEL = 1 */
    msr     control, r0
    isb

    ldr     lr, =0xFFFFFFFD
    bx      lr
```

## 29.4 PendSV handler skeleton

```asm
.syntax unified
.thumb

.global PendSV_Handler
.type PendSV_Handler, %function

PendSV_Handler:
    mrs     r0, psp
    isb

    stmdb   r0!, {r4-r11}

    ldr     r1, =g_current_task
    ldr     r2, [r1]
    str     r0, [r2]          /* current->saved_sp = PSP */

    /*
     * Push 8 bytes to preserve EXC_RETURN and maintain stack alignment
     * before calling C code.
     */
    push    {r3, lr}
    bl      hr_scheduler_commit_switch
    pop     {r3, lr}

    ldr     r1, =g_current_task
    ldr     r2, [r1]
    ldr     r0, [r2]          /* r0 = next->saved_sp */

    ldmia   r0!, {r4-r11}
    msr     psp, r0
    isb

    bx      lr
```

## 29.5 Commit switch

```c
void hr_scheduler_commit_switch(void)
{
    hr_task_t *next;

    next = hr_scheduler_select_next();

    HR_ASSERT(next != NULL);

    if (g_current_task != next)
    {
        if (g_current_task != NULL)
        {
            g_current_task->state = HR_TASK_READY;
        }

        next->state = HR_TASK_RUNNING;
        g_current_task = next;
    }
}
```

## 29.6 SysTick handler skeleton

```c
void SysTick_Handler(void)
{
    bool switch_required = false;

    g_kernel_tick++;

    if (hr_scheduler_tick())
    {
        switch_required = true;
    }

    if (hr_release_due_tasks(g_kernel_tick))
    {
        switch_required = true;
    }

    if (switch_required)
    {
        hr_port_request_context_switch();
    }
}
```

Trong lab đầu của Chủ đề 2, `hr_release_due_tasks()` có thể chỉ là mô phỏng release table. Delayed list hoàn chỉnh được phát triển ở chủ đề task state và blocking.

---

# 30. Debug context switch bằng GDB

## 30.1 Breakpoint quan trọng

```gdb
break SVC_Handler
break PendSV_Handler
break SysTick_Handler
break HardFault_Handler
break hr_scheduler_select_next
```

## 30.2 Xem MSP và PSP

```gdb
monitor reg
p/x $msp
p/x $psp
p/x $control
p/x $xpsr
```

Tùy GDB server, tên special register có thể khác.

## 30.3 Xem current task

```gdb
p g_current_task
p *g_current_task
p/x g_current_task->saved_sp
```

## 30.4 Xem saved frame

```gdb
x/16wx g_current_task->saved_sp
```

Kiểm tra:

- R4-R11 pattern;
- R0 argument;
- PC;
- xPSR;
- stack range.

## 30.5 Disassembly PendSV

```bash
arm-none-eabi-objdump -d build/firmware.elf \
    | less
```

Cần xác nhận:

- handler không có compiler-generated prologue ngoài dự kiến;
- save/restore đối xứng;
- `BL` không phá EXC_RETURN;
- stack alignment đúng;
- symbol TCB đúng offset.

## 30.6 HardFault inspection

Khi HardFault:

- xác định exception dùng MSP hay PSP;
- lấy stacked PC;
- xem stacked LR;
- xem CFSR;
- xem HFSR;
- xem BFAR;
- xem MMFAR.

Một fault sau `bx lr` thường liên quan:

- xPSR Thumb bit;
- PC không hợp lệ;
- PSP sai;
- stack frame sai;
- EXC_RETURN sai;
- alignment sai.

---

# 31. Các lỗi thường gặp

## 31.1 Quên set Thumb bit trong xPSR

Sai:

```c
xpsr = 0U;
```

Đúng:

```c
xpsr = 0x01000000UL;
```

## 31.2 PC chứa địa chỉ không hợp lệ

- function pointer sai;
- địa chỉ bị mất bit;
- stack bị ghi đè;
- cast không dùng `uintptr_t`.

## 31.3 LR của initial task không có return handler

Nếu task return:

```text
PC có thể nhảy tới địa chỉ rác.
```

## 31.4 Stack không aligned 8 byte

Có thể gây:

- fault;
- ABI violation;
- lỗi khi handler gọi C;
- hành vi khác nhau theo compiler optimization.

## 31.5 Save R4-R11 nhưng restore sai thứ tự

```text
Task chạy được vài lần rồi biến local bị hỏng.
```

## 31.6 Không giữ EXC_RETURN qua lời gọi C

PendSV dùng `LR` chứa EXC_RETURN.

Nếu `BL` ghi đè LR mà không lưu:

```text
Không thể exception return đúng.
```

## 31.7 Push một register trước khi gọi C

```asm
push {lr}
bl scheduler
```

Có thể làm MSP lệch 8-byte alignment.

Nên giữ alignment phù hợp, ví dụ push hai register.

## 31.8 PendSV priority không thấp nhất

PendSV có thể preempt ISR và làm kernel state không nhất quán.

## 31.9 Context switch trực tiếp trong SysTick

Làm handler phức tạp và khó reasoning khi exception nesting.

## 31.10 Scheduler và PendSV cùng thay đổi ready queue

Cần ownership rõ:

```text
Scheduler C code cập nhật policy.
PendSV assembly chỉ save/restore context.
```

## 31.11 Ready bitmap không đồng bộ với queue

Ví dụ:

```text
bitmap bit = 1
queue empty
```

Scheduler dereference `NULL`.

## 31.12 Task nằm trong hai ready queue

Có thể chạy cùng TCB từ hai priority khác nhau và phá state.

## 31.13 Idle task bị remove

Khi không task READY:

```text
bitmap = 0
ctz(0) -> undefined behavior
```

## 31.14 Dùng UART trong PendSV

- kéo dài latency;
- thay đổi stack;
- có thể gây nested interrupt;
- che lỗi timing;
- vi phạm port contract.

## 31.15 Dùng biến global thay vì kiểm tra local context

Hai task chỉ tăng global counter không chứng minh context riêng được giữ.

Cần biến local hoặc register pattern riêng.

---

# 32. Chiến lược kiểm thử

## 32.1 Tách policy khỏi mechanism

Host test:

```text
Ready queues
Priority selection
FIFO
Round-robin
Bitmap
State transitions
```

Target test:

```text
PSP
SVC
PendSV
SysTick
Exception return
Register preservation
```

## 32.2 Host tests bắt buộc

- empty ready set bị reject;
- một task;
- nhiều priority;
- nhiều task cùng priority;
- FIFO ordering;
- rotate queue;
- newly ready higher priority;
- lower priority không preempt higher;
- bitmap update;
- duplicate insertion detection;
- invalid priority detection.

## 32.3 Target tests bắt buộc

- first task chạy bằng PSP;
- exception chạy bằng MSP;
- task argument đúng;
- task return hook hoạt động;
- local variables được giữ;
- hai task switch lâu dài;
- PendSV priority đúng;
- SysTick frequency đúng;
- higher task preempts lower task;
- equal-priority round-robin;
- stack guard còn nguyên;
- no HardFault.

## 32.4 Stress test

```text
10^6 hoặc nhiều hơn context switches
```

Không cần UART mỗi switch.

Nên:

- tăng counter trong RAM;
- kiểm tra pattern;
- kiểm tra stack guard định kỳ;
- in tổng kết sau test;
- toggle GPIO theo sampling nếu cần.

## 32.5 Fault injection

Chủ động tạo lỗi:

- bỏ save R7;
- đổi thứ tự restore;
- bỏ Thumb bit;
- làm PSP lệch 4 byte;
- đặt PendSV priority cao;
- không lưu EXC_RETURN;
- trỏ PC sai;
- phá ready bitmap.

Mục tiêu:

```text
Quan sát triệu chứng
+
Xác định root cause
+
Ghi lại phương pháp debug
```

---

# 33. Bài thực hành

## Bài 01 — Cortex-M3 exception and stack inspection

### Mục tiêu

Quan sát:

- Thread mode;
- Handler mode;
- MSP;
- PSP;
- hardware exception frame;
- vector table.

### Yêu cầu

- Tạo một SVC handler đơn giản.
- Ghi lại MSP trước và trong SVC.
- Cấu hình PSP thử nghiệm nhưng chưa chạy task.
- Dùng GDB xem stack frame.
- In địa chỉ qua UART sau khi thoát handler.

### Kết quả cần ghi

```text
MSP before SVC:
MSP inside SVC:
PSP:
CONTROL:
Stacked PC:
Stacked xPSR:
```

### Fault injection

- Gọi SVC từ một vị trí khác.
- So sánh hardware frame.
- Thử đọc sai stack pointer và giải thích kết quả.

### Tiêu chí hoàn thành

- Xác định đúng mode.
- Xác định hardware frame.
- Giải thích MSP/PSP.
- Có ảnh hoặc log GDB.
- Không sửa stack frame ngoài chủ đích.

---

## Bài 02 — Initial task stack builder

### Mục tiêu

Tạo initial stack frame hoàn toàn bằng C.

### Loại lab

```text
Host-first
```

Có thể test trên Ubuntu trước khi dùng target.

### Yêu cầu

- Tạo task stack tĩnh.
- Fill stack bằng `0xA5`.
- Align stack 8 byte.
- Đặt:
  - xPSR;
  - PC;
  - LR;
  - R0;
  - R4-R11.
- Trả về `saved_sp`.

### Host tests

- stack quá nhỏ;
- pointer `NULL`;
- entry `NULL`;
- alignment;
- argument;
- Thumb bit;
- task-return LR;
- no out-of-bounds write.

### Tiêu chí hoàn thành

- ASan pass.
- UBSan pass.
- Frame layout được vẽ.
- `saved_sp` nằm trong stack.
- Có static assertion cho assumptions.

---

## Bài 03 — Start first task using SVC

### Mục tiêu

Rời `main()` bằng MSP và chạy task đầu tiên bằng PSP.

### Yêu cầu

- Tạo một TCB.
- Tạo stack task.
- Build initial frame.
- Set `g_current_task`.
- Gọi SVC.
- SVC restore R4-R11.
- Set PSP.
- Exception return với `0xFFFFFFFD`.
- Task nháy LED hoặc tăng counter.

### Test

Task nhận argument:

```c
typedef struct
{
    uint32_t magic;
} task_argument_t;
```

Task xác nhận:

```c
argument->magic == EXPECTED_MAGIC
```

### Fault injection

- bỏ Thumb bit;
- dùng EXC_RETURN về MSP;
- đặt PC sai;
- stack misalignment.

### Tiêu chí hoàn thành

- Task chạy trên PSP.
- Handler chạy trên MSP.
- Argument đúng.
- `hr_kernel_start()` không return.
- GDB xác nhận PSP nằm trong task stack.

---

## Bài 04 — Cooperative PendSV context switch

### Mục tiêu

Switch giữa hai task khi task gọi `yield()`.

### Yêu cầu

- Hai TCB tĩnh.
- Hai stack tĩnh.
- PendSV save/restore R4-R11.
- Scheduler cooperative đơn giản.
- Task A và B giữ local counter riêng.

### Ví dụ

```c
static void task_a(void *argument)
{
    uint32_t local = 0xA0000000UL;

    for (;;)
    {
        local++;
        g_task_a_observed = local;
        hr_task_yield();
    }
}
```

### Stress test

- ít nhất hàng trăm nghìn switch;
- không UART trong mỗi switch;
- guard word được kiểm tra.

### Tiêu chí hoàn thành

- Local counter đúng.
- Không stack overlap.
- Không HardFault.
- Save/restore đối xứng.
- PendSV không gọi driver.

---

## Bài 05 — Priority scheduler policy on host

### Mục tiêu

Kiểm thử scheduler policy độc lập với Cortex-M assembly.

### Loại lab

```text
Host-only hoặc host-first
```

### Yêu cầu

- Ready queue theo priority.
- Ready bitmap.
- FIFO enqueue.
- Highest-priority selection.
- Queue rotation.
- Idle task rule.
- Duplicate insertion detection.

### Tests

```text
one task
multiple priorities
equal priorities
rotation
bitmap clear/set
idle fallback
invalid priority
double insertion
```

### Tiêu chí hoàn thành

- GCC hoặc Clang host build.
- `-Wall -Wextra -Werror`.
- ASan pass.
- UBSan pass.
- Không include STM32 header.
- Policy test độc lập PendSV.

---

## Bài 06 — Fixed-priority scheduler on target

### Mục tiêu

Ghép scheduler policy với PendSV cooperative switch.

### Task setup

```text
Task High   priority 0
Task Mid    priority 1
Task Low    priority 2
Idle        lowest
```

Vì chưa có blocking đầy đủ, lab có thể dùng release flag hoặc cooperative ready transitions có kiểm soát.

### Yêu cầu

- task priority cao nhất READY được chọn;
- task thấp không chạy khi task cao còn READY;
- current task luôn là head queue;
- scheduler trace lưu trong RAM.

### Tiêu chí hoàn thành

- Priority ordering đúng.
- Trace đúng expected sequence.
- Ready bitmap đúng.
- Scheduler không thao tác register.
- PendSV không chứa policy phức tạp.

---

## Bài 07 — SysTick kernel tick and time slice

### Mục tiêu

Dùng SysTick để:

- tăng tick;
- giảm time slice;
- yêu cầu PendSV.

### Yêu cầu

- tick 1 kHz hoặc giá trị được cấu hình;
- counter 32-bit;
- time slice cho equal-priority tasks;
- GPIO hoặc counter xác nhận tick;
- SysTick không restore task context trực tiếp.

### Tests

- reload calculation;
- time slice 1 tick;
- time slice nhiều tick;
- chỉ một task trong queue;
- nhiều task cùng priority;
- tick counter wrap observation.

### Tiêu chí hoàn thành

- Tick frequency được xác nhận.
- PendSV thấp hơn SysTick.
- Time slice rotation đúng.
- Không UART trong SysTick.

---

## Bài 08 — Preemption from interrupt

### Mục tiêu

Chứng minh task priority cao preempt task thấp sau exception return.

### Kịch bản

```text
Task Low đang chạy
        |
        v
Button/Timer IRQ
        |
        v
Task High được mark READY
        |
        v
PendSV pending
        |
        v
IRQ return
        |
        v
Task High chạy
```

### Yêu cầu

- ISR ngắn;
- không context-switch trực tiếp;
- `higher_priority_task_woken`;
- `yield_from_isr()` chỉ pend PendSV;
- trace thứ tự vào RAM.

### Tiêu chí hoàn thành

- High chạy trước Low tiếp tục.
- ISR không gọi blocking API.
- PendSV chỉ chạy sau ISR.
- Trace chứng minh deferred preemption.

---

## Bài 09 — Equal-priority round-robin

### Mục tiêu

Chứng minh không starvation giữa task cùng priority.

### Task setup

```text
Worker A priority 2
Worker B priority 2
Worker C priority 2
```

### Yêu cầu

- FIFO ready queue;
- time slice;
- rotation;
- counter mỗi task;
- chênh lệch execution count nằm trong giới hạn hợp lý.

### Kết quả mong đợi

```text
A count ≈ B count ≈ C count
```

Không yêu cầu hoàn toàn bằng nhau tại mọi thời điểm.

### Tiêu chí hoàn thành

- FIFO đúng.
- Task không bị bỏ quên.
- Lower priority không chen khi priority 2 còn READY.
- Rotation chỉ xảy ra khi cần.

---

## Bài 10 — Context-switch stress and diagnostics

### Mục tiêu

Kiểm tra độ bền của implementation.

### Yêu cầu

- nhiều task;
- nhiều priority;
- equal-priority group;
- hàng triệu switch;
- local variables;
- register patterns;
- stack guard;
- high-water mark;
- switch counter;
- panic hook.

### Không được làm

- `printf()` mỗi switch;
- allocate memory trong PendSV;
- đổi task stack trong application;
- sửa TCB state trực tiếp.

### Report

```text
Total switches:
Task counters:
Stack high-water:
Guard status:
Fault count:
Scheduler invariant status:
```

### Tiêu chí hoàn thành

- Stress run pass.
- Không corruption.
- Không guard failure.
- Scheduler invariant pass.
- Có report target validation.

---

# 34. Project tổng kết chủ đề

## Tên project

```text
Scheduler Playground
```

## 34.1 Mục tiêu

Xây dựng một firmware độc lập minh họa đầy đủ:

- static task creation;
- initial task stack;
- SVC first-task startup;
- PendSV context switch;
- fixed-priority scheduler;
- SysTick tick;
- preemption;
- equal-priority round-robin;
- scheduler diagnostics;
- stack checking.

## 34.2 Kiến trúc

```text
+------------------------------------------------------+
| Application Tasks                                    |
|                                                      |
| High Event Task                                      |
| Worker A               same priority                 |
| Worker B               same priority                 |
| Background Task                                      |
| Idle Task                                            |
+--------------------------+---------------------------+
                           |
                           v
+------------------------------------------------------+
| Scheduler                                            |
|                                                      |
| Ready queues per priority                            |
| Ready bitmap                                         |
| Time slice                                           |
| Current/next task                                    |
+--------------------------+---------------------------+
                           |
                           v
+------------------------------------------------------+
| Cortex-M3 Port                                       |
|                                                      |
| Initial stack frame                                  |
| SVC                                                  |
| PendSV                                               |
| SysTick                                              |
| Critical section                                     |
+--------------------------+---------------------------+
                           |
                           v
+------------------------------------------------------+
| STM32F103 Platform                                   |
|                                                      |
| Startup  Clock  GPIO  UART  Button/Timer IRQ         |
+------------------------------------------------------+
```

## 34.3 Task set

### High Event Task

- priority cao;
- được release định kỳ hoặc bởi button IRQ;
- chạy ngắn;
- ghi trace;
- sau đó trở lại trạng thái không READY theo cơ chế demo tối thiểu.

### Worker A và Worker B

- cùng priority;
- dùng round-robin;
- mỗi task có local counter;
- tạo bằng chứng time slicing.

### Background Task

- priority thấp;
- chạy khi task cao hơn không READY;
- cập nhật counter nền.

### Idle Task

- priority thấp nhất;
- luôn READY;
- có thể dùng `WFI` khi phù hợp;
- không được block hoặc return.

## 34.4 UART diagnostic commands

Nếu project có command shell tối thiểu:

```text
help
tasks
ready
stats
switches
stacks
trace
preempt-demo
round-robin-demo
stress-start
stress-stop
reset
```

Lưu ý:

- command parser không được chạy trong PendSV;
- UART ISR không được gọi scheduler API task-only;
- output dài nên chạy trong monitor task hoặc trước kernel start;
- trace nên snapshot trước khi in.

## 34.5 Scheduler trace

Trace event:

```c
typedef enum
{
    TRACE_TASK_START,
    TRACE_TASK_YIELD,
    TRACE_TICK,
    TRACE_PENDSV_ENTER,
    TRACE_SWITCH,
    TRACE_ISR_RELEASE,
    TRACE_TASK_RUN
} trace_event_type_t;
```

Record:

```c
typedef struct
{
    uint32_t tick;
    uint32_t sequence;
    trace_event_type_t type;
    uint8_t from_task;
    uint8_t to_task;
} trace_record_t;
```

Trace buffer là static ring buffer.

Không cấp phát động.

## 34.6 Project requirements

- Root `Makefile` chỉ quản lý project tổng kết.
- Mỗi lab có `Makefile` riêng.
- Không dùng HAL hoặc SPL nếu mục tiêu là register-level.
- Static TCB và static stack.
- SVC start first task.
- PendSV save/restore R4-R11.
- SysTick chỉ cập nhật tick và request switch.
- PendSV priority thấp nhất.
- Scheduler policy host-tested.
- Stack 8-byte aligned.
- Idle task luôn READY.
- Panic khi task return.
- Có stack guard.
- Có stress test.
- Có target validation report.

## 34.7 Definition of Done

Project hoàn thành khi:

- firmware build không warning;
- ELF/BIN/HEX/MAP được tạo;
- first task chạy PSP;
- exception chạy MSP;
- local variables giữ đúng qua context switch;
- priority scheduler đúng;
- round-robin đúng;
- preemption đúng;
- stress test pass;
- stack guards pass;
- README mô tả thiết kế;
- GDB evidence được ghi lại;
- firmware được xác nhận trên board thật.

---

# 35. Cấu trúc repository đề xuất

```text
02-scheduling-context-switch/
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
│   ├── gpio.h
│   ├── uart.h
│   ├── systick.h
│   ├── critical_section.h
│   ├── task.h
│   ├── scheduler.h
│   ├── scheduler_trace.h
│   └── cortex_m3_port.h
│
├── src/
│   ├── main.c
│   ├── runtime.c
│   ├── clock.c
│   ├── gpio.c
│   ├── uart.c
│   ├── systick.c
│   ├── critical_section.c
│   ├── task.c
│   ├── scheduler.c
│   ├── scheduler_trace.c
│   ├── cortex_m3_port.c
│   └── cortex_m3_portasm.S
│
├── labs/
│   ├── 01-exception-stack-inspection/
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── startup/
│   │   ├── linker/
│   │   ├── include/
│   │   └── src/
│   │
│   ├── 02-initial-task-stack/
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── include/
│   │   ├── src/
│   │   └── tests/
│   │
│   ├── 03-svc-first-task/
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── startup/
│   │   ├── linker/
│   │   ├── include/
│   │   └── src/
│   │
│   ├── 04-cooperative-pendsv/
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── startup/
│   │   ├── linker/
│   │   ├── include/
│   │   └── src/
│   │
│   ├── 05-priority-scheduler-host/
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── include/
│   │   ├── src/
│   │   └── tests/
│   │
│   ├── 06-fixed-priority-target/
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── startup/
│   │   ├── linker/
│   │   ├── include/
│   │   └── src/
│   │
│   ├── 07-systick-time-slice/
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── startup/
│   │   ├── linker/
│   │   ├── include/
│   │   └── src/
│   │
│   ├── 08-isr-preemption/
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── startup/
│   │   ├── linker/
│   │   ├── include/
│   │   └── src/
│   │
│   ├── 09-round-robin/
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── startup/
│   │   ├── linker/
│   │   ├── include/
│   │   └── src/
│   │
│   ├── 10-context-switch-stress/
│   |   ├── README.md
│   |   ├── Makefile
│   |   ├── startup/
│   |   ├── linker/
│   |   ├── include/
│   |   ├── src/
|   |   └── report/
|   |
│   └── README.md
│
├── docs/
│   ├── scheduling-models.md
│   ├── cortex-m3-exception-model.md
│   ├── msp-psp.md
│   ├── initial-task-frame.md
│   ├── svc-first-task.md
│   ├── pendsv-context-switch.md
│   ├── systick-scheduling.md
│   ├── scheduler-invariants.md
│   ├── gdb-context-switch.md
│   └── target-validation.md
│
└── build/
```

---

# 36. Liên hệ với roadmap HairRTOS

Chủ đề 2 liên quan trực tiếp các phase sau.

## Phase 2 — Kernel data structures

Chuẩn bị:

- intrusive list;
- FIFO ready queue;
- ready set;
- host tests.

Phần này hỗ trợ scheduler policy nhưng chưa context switch.

## Phase 3 — TCB and initial task stack

Bao phủ:

- TCB;
- static task creation;
- stack bounds;
- initial Cortex-M frame;
- task-return panic handler.

## Phase 4 — Start first task using SVC

Bao phủ:

- SVC handler;
- PSP task;
- MSP exception;
- first task start;
- kernel lifecycle RUNNING.

## Phase 5 — PendSV cooperative context switch

Bao phủ:

- save/restore R4-R11;
- current/next TCB;
- `yield()`;
- cooperative switching.

## Phase 6 — Fixed-priority scheduler

Bao phủ:

- ready queues;
- ready bitmap;
- priority selection;
- FIFO among equals.

## Phase 7 — SysTick, delay and timeout base

Trong Chủ đề 2 chỉ cần:

- kernel tick;
- basic release/time-slice support.

Delayed list và blocking đầy đủ có thể được hoàn thiện ở chủ đề task state.

## Phase 8 — Preemption and round-robin

Bao phủ:

- higher-priority preemption;
- deferred PendSV;
- equal-priority time slice;
- idle scheduling.

---

# 37. Tiêu chí hoàn thành

## Scheduling theory

- Giải thích scheduler là gì.
- Phân biệt scheduling decision và context switch.
- Phân biệt cooperative và preemptive.
- Giải thích fixed-priority policy.
- Giải thích round-robin.
- Giải thích starvation.
- Giải thích idle task.

## Cortex-M3 architecture

- Phân biệt Thread mode và Handler mode.
- Phân biệt MSP và PSP.
- Liệt kê hardware-saved registers.
- Liệt kê software-saved registers.
- Giải thích exception entry.
- Giải thích exception return.
- Giải thích `EXC_RETURN`.
- Giải thích Thumb bit.
- Giải thích stack alignment.

## Initial task

- Build initial frame.
- R0 chứa argument.
- PC trỏ task entry.
- LR trỏ task-return handler.
- xPSR Thumb bit được set.
- saved stack pointer đúng range.
- Host tests pass.

## SVC

- Start task đầu tiên bằng SVC.
- Task chạy bằng PSP.
- Handler chạy bằng MSP.
- Kernel start không return.
- GDB xác nhận đúng.

## PendSV

- PendSV priority thấp.
- Save R4-R11.
- Store current PSP vào TCB.
- Select next TCB.
- Restore R4-R11.
- Write next PSP.
- Preserve EXC_RETURN.
- Không gọi driver hoặc allocator.

## Scheduler

- Ready queues per priority.
- Ready bitmap.
- Highest-priority selection.
- FIFO among equals.
- Round-robin.
- Idle task always ready.
- Invariants được assert.
- Host tests pass với sanitizer.

## SysTick and preemption

- Kernel tick hoạt động.
- Time slice hoạt động.
- SysTick chỉ request PendSV.
- Higher task preempts lower.
- ISR preemption được deferred.
- Equal-priority tasks không starvation.

## Validation

- Local variables giữ đúng.
- Stress test pass.
- Stack guard pass.
- Không HardFault.
- Có UART/GDB evidence.
- Có target validation report.
- Root Makefile chỉ quản lý final project.
- Mỗi lab có Makefile riêng.

---

# 38. Tổng kết

Chủ đề 2 giải quyết hai câu hỏi trung tâm:

```text
Task nào nên chạy tiếp?
```

và:

```text
CPU chuyển từ task hiện tại sang task tiếp theo như thế nào?
```

Luồng kiến thức:

```text
Scheduling models
        |
        v
Fixed priority + round-robin
        |
        v
TCB + task stack
        |
        v
Cortex-M3 exception model
        |
        v
MSP + PSP
        |
        v
Initial task frame
        |
        v
SVC starts first task
        |
        v
PendSV saves/restores context
        |
        v
SysTick requests scheduling
        |
        v
Preemption + stress validation
```

Các nguyên tắc cần nhớ:

1. Scheduler chọn task; architecture port đổi context.
2. Task chạy bằng PSP; exception chạy bằng MSP.
3. CPU tự lưu `R0-R3`, `R12`, `LR`, `PC`, `xPSR`.
4. PendSV tự lưu `R4-R11`.
5. Initial task phải có stack frame giả hợp lệ.
6. xPSR phải đặt Thumb bit.
7. Stack phải aligned.
8. SVC phù hợp để start task đầu tiên.
9. PendSV phù hợp để defer context switch.
10. PendSV phải có priority thấp.
11. SysTick không nên trực tiếp đổi task.
12. ISR chỉ request switch; PendSV thực hiện switch.
13. Task priority cao READY phải được chọn trước.
14. Task cùng priority cần FIFO hoặc round-robin.
15. Idle task phải luôn READY.
16. Ready bitmap và ready queue phải nhất quán.
17. Không được gọi allocator, driver hoặc callback trong PendSV.
18. Policy scheduler phải test trên host trước khi ghép assembly.
19. Local variable preservation là bằng chứng context switch đúng.
20. Stress test và GDB inspection là bắt buộc.

Sau chủ đề này, kết quả cần đạt:

```text
Một scheduler policy đã được host-test
+
Một initial task frame đúng Cortex-M3
+
Một SVC first-task startup hoạt động
+
Một PendSV context switch ổn định
+
Một SysTick preemptive scheduler cơ bản
+
Một Scheduler Playground chạy trên STM32F103
```

Chủ đề tiếp theo có thể đi sâu vào:

```text
Cấu trúc dữ liệu trong Kernel + Task
```

hoặc, nếu roadmap thực hành đã làm data structure trước context switch, tiếp tục hoàn thiện:

```text
Task state + Blocking + Synchronization
```

---

## Nguồn chương trình

README này bám theo phạm vi Chủ đề 2 trong tài liệu:

```text
self-develop-rtos.pdf

Scheduling + Context Switch
    - Priority-based scheduling models
    - ARM Cortex-M3 architecture cơ bản
    - SysTick timer trong scheduling
    - PendSV handler cho context switch
    - Context switch implementation
```

Phần triển khai thực hành được mở rộng theo các specification của HairRTOS:

```text
Phase 2 — Intrusive lists and kernel data structures
Phase 3 — TCB and initial task stack
Phase 4 — Start first task using SVC
Phase 5 — PendSV cooperative context switch
Phase 6 — Fixed-priority scheduler
Phase 7 — SysTick, delay and timeout base
Phase 8 — Preemption and round-robin
```
