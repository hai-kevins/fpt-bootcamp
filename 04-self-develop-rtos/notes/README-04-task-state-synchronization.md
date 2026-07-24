# Chủ đề 4 - Task State và Synchronization
## Efficient Blocking, Delayed Task List, Semaphore và Mutex

> README này được xây dựng theo phạm vi **Chủ đề 4 — “Task state + Synchronization”** trong chương trình tự phát triển AKOS.

---

## Mục lục

1. [Mục tiêu học tập](#1-mục-tiêu-học-tập)
2. [Phạm vi của chủ đề](#2-phạm-vi-của-chủ-đề)
3. [Kiến thức cần chuẩn bị](#3-kiến-thức-cần-chuẩn-bị)
4. [Task state là gì?](#4-task-state-là-gì)
5. [Các trạng thái cơ bản](#5-các-trạng-thái-cơ-bản)
6. [Task state machine](#6-task-state-machine)
7. [READY và RUNNING](#7-ready-và-running)
8. [BLOCKED](#8-blocked)
9. [SUSPENDED](#9-suspended)
10. [CREATED và TERMINATED có cần không?](#10-created-và-terminated-có-cần-không)
11. [Efficient blocking là gì?](#11-efficient-blocking-là-gì)
12. [Busy-wait và blocking](#12-busy-wait-và-blocking)
13. [Một blocking API phải làm gì?](#13-một-blocking-api-phải-làm-gì)
14. [Ready list và priority scheduling](#14-ready-list-và-priority-scheduling)
15. [Delayed task list](#15-delayed-task-list)
16. [Wake tick và timeout](#16-wake-tick-và-timeout)
17. [So sánh tick an toàn khi wrap-around](#17-so-sánh-tick-an-toàn-khi-wrap-around)
18. [Delay tương đối](#18-delay-tương-đối)
19. [Delay tuyệt đối và delay-until](#19-delay-tuyệt-đối-và-delay-until)
20. [Periodic task và drift](#20-periodic-task-và-drift)
21. [Timeout model](#21-timeout-model)
22. [Wait forever và no-wait](#22-wait-forever-và-no-wait)
23. [Task đồng thời nằm trong wait list và timeout list](#23-task-đồng-thời-nằm-trong-wait-list-và-timeout-list)
24. [Wake-up race](#24-wake-up-race)
25. [Single-winner wake-up rule](#25-single-winner-wake-up-rule)
26. [Synchronization là gì?](#26-synchronization-là-gì)
27. [Race condition](#27-race-condition)
28. [Atomicity và critical section](#28-atomicity-và-critical-section)
29. [Semaphore](#29-semaphore)
30. [Binary semaphore](#30-binary-semaphore)
31. [Counting semaphore](#31-counting-semaphore)
32. [Mutex](#32-mutex)
33. [Semaphore và mutex khác nhau thế nào?](#33-semaphore-và-mutex-khác-nhau-thế-nào)
34. [Mutex ownership](#34-mutex-ownership)
35. [Recursive mutex có cần không?](#35-recursive-mutex-có-cần-không)
36. [Priority inversion](#36-priority-inversion)
37. [Priority inheritance](#37-priority-inheritance)
38. [Base priority và effective priority](#38-base-priority-và-effective-priority)
39. [Waiter ordering](#39-waiter-ordering)
40. [FIFO giữa waiter cùng priority](#40-fifo-giữa-waiter-cùng-priority)
41. [ISR-safe synchronization API](#41-isr-safe-synchronization-api)
42. [Không được block trong ISR](#42-không-được-block-trong-isr)
43. [Task wake-up từ ISR](#43-task-wake-up-từ-isr)
44. [Thiết kế task state trong TCB](#44-thiết-kế-task-state-trong-tcb)
45. [Các list node cần thêm vào TCB](#45-các-list-node-cần-thêm-vào-tcb)
46. [Wait result và wake reason](#46-wait-result-và-wake-reason)
47. [Task transition helpers](#47-task-transition-helpers)
48. [Block current task](#48-block-current-task)
49. [Wake blocked task](#49-wake-blocked-task)
50. [Suspend và resume](#50-suspend-và-resume)
51. [Suspend khác block ở đâu?](#51-suspend-khác-block-ở-đâu)
52. [Suspend task đang BLOCKED](#52-suspend-task-đang-blocked)
53. [Kernel invariants](#53-kernel-invariants)
54. [Synchronization object invariants](#54-synchronization-object-invariants)
55. [Lock ordering và deadlock](#55-lock-ordering-và-deadlock)
56. [Starvation](#56-starvation)
57. [Diagnostic và trace](#57-diagnostic-và-trace)
58. [Mã khung task state](#58-mã-khung-task-state)
59. [Mã khung tick comparison](#59-mã-khung-tick-comparison)
60. [Mã khung delayed list](#60-mã-khung-delayed-list)
61. [Mã khung task delay](#61-mã-khung-task-delay)
62. [Mã khung binary semaphore](#62-mã-khung-binary-semaphore)
63. [Mã khung counting semaphore](#63-mã-khung-counting-semaphore)
64. [Mã khung mutex](#64-mã-khung-mutex)
65. [Mã khung priority inheritance](#65-mã-khung-priority-inheritance)
66. [Chiến lược kiểm thử](#66-chiến-lược-kiểm-thử)
67. [Lỗi thường gặp](#67-lỗi-thường-gặp)
68. [Bài thực hành](#68-bài-thực-hành)
69. [Project tổng kết chủ đề](#69-project-tổng-kết-chủ-đề)
70. [Cấu trúc repository đề xuất](#70-cấu-trúc-repository-đề-xuất)
71. [Liên hệ với roadmap HairRTOS](#71-liên-hệ-với-roadmap-hairtos)
72. [Tiêu chí hoàn thành](#72-tiêu-chí-hoàn-thành)
73. [Tổng kết](#73-tổng-kết)

---

# 1. Mục tiêu học tập

Sau khi hoàn thành chủ đề này, người học cần có khả năng:

- Giải thích được task state trong RTOS.
- Phân biệt:
  - READY;
  - RUNNING;
  - BLOCKED;
  - SUSPENDED.
- Vẽ và giải thích được state machine của task.
- Giải thích được efficient blocking.
- Phân biệt busy-wait và blocking.
- Chuyển task giữa ready list, wait list và delayed list.
- Cài đặt được:
  - `delay()`;
  - `delay_until()`;
  - finite timeout;
  - no-wait;
  - wait forever.
- So sánh tick an toàn khi counter wrap.
- Xử lý được race giữa:
  - timeout;
  - object availability;
  - ISR wake-up;
  - task suspension.
- Thiết kế được single-winner wake-up rule.
- Giải thích synchronization và mutual exclusion.
- Phân biệt:
  - binary semaphore;
  - counting semaphore;
  - mutex.
- Giải thích mutex ownership.
- Giải thích priority inversion.
- Cài đặt priority inheritance cơ bản.
- Sắp waiter theo effective priority và FIFO giữa các task cùng priority.
- Thiết kế ISR-safe nonblocking API.
- Giải thích vì sao ISR không được block.
- Thêm các node cần thiết vào TCB:
  - ready node;
  - wait node;
  - timeout node;
  - all-task node.
- Viết host tests cho task state, delayed list, semaphore và mutex.
- Chạy project tổng kết trên STM32F103.
- Quan sát task state, wait object, timeout và mutex owner qua UART.

---

# 2. Phạm vi của chủ đề

Theo chương trình AKOS, Chủ đề 4 gồm:

```text
Task state + Synchronization
    |
    +-- Efficient blocking of threads
    +-- Task state: Ready, Running, Blocked, Suspended
    +-- Ready list và priority scheduling
    +-- Delayed task list
    +-- Synchronization and mutual exclusion mechanisms
```

README này mở rộng thành lộ trình thực hành:

```text
Task state machine
        |
        v
Efficient blocking
        |
        v
Delayed list + timeout
        |
        v
Delay + delay-until
        |
        v
Semaphore
        |
        v
Mutex ownership
        |
        v
Priority inversion
        |
        v
Priority inheritance
        |
        v
ISR-safe wake-up
        |
        v
Synchronization Playground
```

## Ranh giới của chủ đề

Chủ đề này tập trung vào:

- task states;
- blocking;
- delayed task list;
- timeout;
- semaphore;
- mutex;
- ownership;
- priority inversion;
- priority inheritance cơ bản;
- ISR wake-up;
- synchronization diagnostics.

Chủ đề này chưa tập trung vào:

- message queue payload;
- software timer service;
- event framework;
- publish/subscribe;
- benchmark suite hoàn chỉnh.

---

# 3. Kiến thức cần chuẩn bị

## 3.1 Từ Chủ đề 1

- Static allocation.
- Stack.
- Alignment.
- Critical section cơ bản.
- Host testing.
- Assertions.
- Memory ownership.

## 3.2 Từ Chủ đề 2

- Fixed-priority scheduling.
- Preemption.
- PendSV.
- SysTick.
- SVC.
- Current task.
- Higher-priority task wake-up.

## 3.3 Từ Chủ đề 3

- Intrusive list.
- List ownership.
- TCB.
- Ready queues.
- Ready bitmap.
- All-task registry.
- Multiple list nodes trong một TCB.

## 3.4 Ngôn ngữ C

- `struct`.
- `enum`.
- Function pointer.
- Pointer.
- Bit operations.
- Integer wrap-around.
- `_Static_assert`.
- `volatile`.
- Critical-section wrapper.

---

# 4. Task state là gì?

Task state mô tả quan hệ hiện tại giữa task và scheduler.

Ví dụ:

```text
Task A đang chạy trên CPU
Task B có thể chạy nhưng đang chờ CPU
Task C đang chờ semaphore
Task D bị application suspend
```

Tương ứng:

```text
Task A -> RUNNING
Task B -> READY
Task C -> BLOCKED
Task D -> SUSPENDED
```

State không chỉ là một enum để in diagnostic.

State phải nhất quán với:

- list membership;
- current task pointer;
- waiting object;
- timeout node;
- ready bitmap;
- wake reason.

---

# 5. Các trạng thái cơ bản

## READY

Task đủ điều kiện chạy nhưng chưa được CPU chọn.

Task READY phải nằm trong ready queue tương ứng.

## RUNNING

Task hiện đang được CPU thực thi.

Trên MCU một nhân, chỉ có một task RUNNING tại một thời điểm.

## BLOCKED

Task chưa đủ điều kiện chạy vì đang chờ:

- timeout;
- semaphore;
- mutex;
- queue data;
- event;
- external condition.

Task BLOCKED không nằm trong ready queue.

## SUSPENDED

Task bị loại khỏi scheduling theo lệnh quản lý.

Task SUSPENDED không được tự động wake bởi:

- timeout;
- semaphore;
- mutex;
- ISR event.

Chỉ explicit resume mới đưa task trở lại scheduler.

---

# 6. Task state machine

State machine cơ bản:

```text
                create
       UNUSED ----------> CREATED
                              |
                              | make ready
                              v
                          READY
                            |
                            | scheduler selects
                            v
                         RUNNING
                         /  |   \
                        /   |    \
               preempt /    |     \ block
                      v     |      v
                    READY   |   BLOCKED
                            |      |
                            |      | event/timeout
                            |      v
                            +---- READY

READY/RUNNING/BLOCKED
          |
          | suspend
          v
      SUSPENDED
          |
          | resume
          v
        READY
```

## State transition không được thực hiện rời rạc

Sai:

```c
task->state = HR_TASK_BLOCKED;
```

mà không:

- remove ready node;
- insert wait node;
- insert timeout node;
- clear current task relation;
- request scheduling.

Cần helper duy nhất quản lý toàn bộ transition.

---

# 7. READY và RUNNING

READY task:

- nằm trong đúng ready queue;
- có priority hợp lệ;
- ready bitmap bit được set;
- không nằm trong delayed list;
- không nằm trong wait list;
- không SUSPENDED.

RUNNING task:

- là current task;
- thường vẫn là head của ready queue;
- có state RUNNING;
- không có wait object;
- không có finite timeout đang active.

Khi preempt:

```text
RUNNING -> READY
```

Task không bị remove khỏi ready queue nếu scheduler giữ current task ở head.

---

# 8. BLOCKED

Task BLOCKED không tiêu thụ CPU chỉ để kiểm tra điều kiện.

Ví dụ:

```c
while (!semaphore_available())
{
}
```

là busy-wait.

Blocking đúng:

```c
hr_semaphore_take(&semaphore,
                  HR_WAIT_FOREVER);
```

Kernel:

```text
Remove current from ready queue
Insert into semaphore wait list
Record waiting object
Optionally insert timeout node
Set state BLOCKED
Pend PendSV
```

Khi semaphore được give:

```text
Remove highest waiter
Cancel timeout if active
Clear waiting object
Set wake result
Insert ready queue
Pend PendSV if higher priority
```

---

# 9. SUSPENDED

SUSPENDED là trạng thái quản trị.

Use cases:

- dừng tạm task;
- diagnostic;
- mode switch;
- shutdown một subsystem;
- test scheduler;
- fault containment ở mức đơn giản.

API:

```c
hr_status_t hr_task_suspend(hr_task_t *task);
hr_status_t hr_task_resume(hr_task_t *task);
```

Task SUSPENDED không được tự wake bởi object hoặc timeout.

---

# 10. CREATED và TERMINATED có cần không?

Nguồn AKOS nêu bốn state:

```text
Ready
Running
Blocked
Suspended
```

HairRTOS có thể thêm:

```text
CREATED
UNUSED
TERMINATED
```

để hỗ trợ lifecycle và diagnostics.

## CREATED

Task đã được tạo nhưng chưa đưa vào ready queue.

## UNUSED

TCB chưa đại diện task hợp lệ.

## TERMINATED

Chỉ cần nếu kernel hỗ trợ task exit/delete.

HairRTOS static-first phiên bản đầu có thể không hỗ trợ delete task.

---

# 11. Efficient blocking là gì?

Efficient blocking nghĩa là task không giữ CPU khi chưa có điều kiện tiếp tục.

Busy-wait:

```text
Task giữ CPU
    |
    +-- kiểm tra flag;
    +-- kiểm tra lại;
    +-- kiểm tra lại;
    +-- ...
```

Efficient blocking:

```text
Task chưa có điều kiện
    |
    v
Kernel remove khỏi ready list
    |
    v
CPU chạy task khác
    |
    v
Event xảy ra
    |
    v
Task trở lại ready list
```

Lợi ích:

- giảm CPU waste;
- idle task có thể dùng `WFI`;
- giảm power;
- task khác có cơ hội chạy;
- latency và priority rõ hơn.

---

# 12. Busy-wait và blocking

## Busy-wait

```c
while (!uart_rx_ready())
{
}
```

Ưu điểm:

- đơn giản;
- latency thấp trong đoạn rất ngắn;
- có thể phù hợp startup trước scheduler.

Nhược điểm:

- giữ CPU;
- task priority thấp có thể làm hệ thống mất thời gian;
- tăng power;
- khó mở rộng.

## Blocking

```c
hr_semaphore_take(&uart_rx_sem,
                  HR_WAIT_FOREVER);
```

Ưu điểm:

- CPU chạy task khác;
- phù hợp scheduling;
- hỗ trợ timeout;
- dễ kết hợp ISR.

Nhược điểm:

- cần kernel object;
- cần state transition;
- có context-switch overhead;
- cần xử lý race.

---

# 13. Một blocking API phải làm gì?

Một blocking API đúng cần:

1. Validate object và timeout.
2. Enter critical section.
3. Kiểm tra điều kiện ngay lập tức.
4. Nếu thành công, consume resource và return.
5. Nếu `NO_WAIT`, return timeout/unavailable.
6. Nếu cần block:
   - record waiting object;
   - record requested operation;
   - insert object wait list;
   - insert delayed list nếu finite timeout;
   - remove ready queue;
   - set BLOCKED;
   - pend PendSV.
7. Exit critical section.
8. Khi task chạy lại, return wake result.

---

# 14. Ready list và priority scheduling

Ready queues từ Chủ đề 3 vẫn là trung tâm.

Khi task BLOCKED:

```text
ready_remove(task)
```

Khi task wake:

```text
ready_insert(task)
```

Nếu task wake có priority cao hơn current task:

```text
PendSV pending
```

Ready bitmap phải cập nhật đồng thời với list.

---

# 15. Delayed task list

Delayed list lưu task chờ theo thời gian.

Mỗi task finite delay có:

```text
wake_tick
timeout_node
```

Cấu trúc:

```text
Delayed list ordered by wake tick:

Task B wake 105
Task D wake 110
Task A wake 125
```

SysTick chỉ cần kiểm tra đầu list.

```text
current tick >= head wake tick?
    |
    +-- no -> stop
    |
    +-- yes -> wake head, repeat
```

## Sorted list

Ưu điểm:

- SysTick không quét toàn bộ task;
- chỉ xử lý task tới hạn;
- insert O(n);
- tick processing O(k), với k là số task hết hạn.

## Timing wheel

Là lựa chọn khác nhưng phức tạp hơn. Không cần cho phiên bản đầu.

---

# 16. Wake tick và timeout

Delay tương đối:

```text
wake_tick = current_tick + delay_ticks
```

Finite timeout:

```text
deadline = current_tick + timeout_ticks
```

Tick là unsigned integer và có thể wrap.

Ví dụ 32-bit:

```text
0xFFFFFFFE
0xFFFFFFFF
0x00000000
0x00000001
```

Không được so sánh tuyệt đối bằng logic naive.

---

# 17. So sánh tick an toàn khi wrap-around

Với khoảng thời gian nhỏ hơn nửa range, dùng signed difference:

```c
static bool hr_tick_reached(uint32_t now,
                            uint32_t deadline)
{
    return (int32_t)(now - deadline) >= 0;
}
```

Kiểm tra trước deadline:

```c
static bool hr_tick_before(uint32_t a,
                           uint32_t b)
{
    return (int32_t)(a - b) < 0;
}
```

## Contract

Finite timeout tối đa nên nhỏ hơn:

```text
2^31 ticks
```

với counter 32-bit.

Không hỗ trợ timeout tùy ý gần toàn bộ 32-bit range nếu dùng signed-difference model.

---

# 18. Delay tương đối

API:

```c
hr_status_t hr_task_delay(uint32_t ticks);
```

Behavior:

```text
ticks == 0
    |
    +-- yield hoặc return theo policy

ticks > 0
    |
    +-- block current tới now + ticks
```

Mã logic:

```text
current task
    |
    +-- remove ready;
    +-- wake_tick = now + ticks;
    +-- insert delayed list;
    +-- state BLOCKED;
    +-- wake reason = DELAY;
    +-- PendSV.
```

---

# 19. Delay tuyệt đối và delay-until

API:

```c
hr_status_t hr_task_delay_until(
    uint32_t *previous_wake_tick,
    uint32_t period_ticks);
```

Cách dùng:

```c
uint32_t next = hr_tick_now();

for (;;)
{
    sample_sensor();
    hr_task_delay_until(&next, 10U);
}
```

Sau mỗi chu kỳ:

```text
next = next + period
```

không phải:

```text
next = now + period
```

Điều này giảm drift.

---

# 20. Periodic task và drift

Dùng delay tương đối:

```text
work 2 ms
delay 10 ms
work 2 ms
delay 10 ms
```

Chu kỳ thực tế:

```text
12 ms
```

Dùng delay-until:

```text
release mỗi 10 ms
```

Nếu work hoàn thành trước deadline, task wake theo lịch tuyệt đối.

Nếu task quá hạn:

- có thể chạy ngay;
- update next release;
- tăng deadline miss counter;
- policy cần rõ.

---

# 21. Timeout model

Ba kiểu timeout:

```text
HR_NO_WAIT
finite ticks
HR_WAIT_FOREVER
```

Kết quả wait:

```text
SUCCESS
TIMEOUT
CANCELLED
SUSPENDED
OBJECT_DELETED
INVALID
```

HairRTOS static-first có thể chưa hỗ trợ object deletion, nhưng enum nên được thiết kế có chủ đích.

---

# 22. Wait forever và no-wait

## No-wait

```c
hr_semaphore_take(&sem, HR_NO_WAIT);
```

Nếu resource chưa có:

```text
return HR_STATUS_WOULD_BLOCK
```

Task không đổi state.

## Wait forever

```c
hr_semaphore_take(&sem, HR_WAIT_FOREVER);
```

Task vào object wait list nhưng không vào delayed list.

## Finite timeout

Task vào cả:

- object wait list;
- timeout list.

---

# 23. Task đồng thời nằm trong wait list và timeout list

TCB cần hai node:

```c
hr_list_node_t wait_node;
hr_list_node_t timeout_node;
```

Một finite wait:

```text
Semaphore wait list
    |
    +-- task.wait_node

Delayed timeout list
    |
    +-- task.timeout_node
```

Task đồng thời thuộc hai list bằng hai node khác nhau.

Khi object wake:

- remove wait node;
- remove timeout node;
- set SUCCESS.

Khi timeout:

- remove timeout node;
- remove wait node;
- set TIMEOUT.

---

# 24. Wake-up race

Race:

```text
Tick N:
Semaphore becomes available
Timeout also expires
```

Nếu cả hai path đều wake task:

- task có thể được insert ready queue hai lần;
- wait result bị ghi đè;
- list corruption;
- semaphore count sai.

Cần atomic wake protocol.

---

# 25. Single-winner wake-up rule

Chỉ một path được quyền chuyển task từ BLOCKED sang READY.

Có thể kiểm tra:

```c
if (task->state != HR_TASK_STATE_BLOCKED)
{
    return false;
}
```

Trong critical section:

1. Xác nhận task vẫn BLOCKED.
2. Remove khỏi tất cả wait structures.
3. Clear waiting object.
4. Set result.
5. Set READY.
6. Insert ready queue.

Path thứ hai thấy state không còn BLOCKED và không làm gì.

---

# 26. Synchronization là gì?

Synchronization điều phối thứ tự thực thi giữa task hoặc giữa ISR và task.

Ví dụ:

```text
Task Consumer chỉ chạy khi Producer có dữ liệu.
Task Control chỉ chạy khi ISR báo sensor ready.
Task High chờ mutex do Task Low giữ.
```

Synchronization object không chỉ bảo vệ data.

Nó có thể biểu diễn:

- event occurrence;
- resource count;
- ownership;
- completion;
- permission.

---

# 27. Race condition

Race condition xảy ra khi kết quả phụ thuộc timing không được kiểm soát.

Ví dụ:

```c
counter++;
```

không nhất thiết atomic.

Hai task:

```text
A read counter = 5
B read counter = 5
A write 6
B write 6
```

Kết quả đúng lẽ ra là 7 nhưng thực tế là 6.

Giải pháp tùy bài toán:

- critical section;
- mutex;
- atomic instruction;
- single-owner task;
- message passing.

---

# 28. Atomicity và critical section

Kernel list transitions phải atomic với ISR/kernel context có thể truy cập cùng data.

Critical section:

```c
uint32_t state = hr_port_critical_enter();

/* update list, state, bitmap */

hr_port_critical_exit(state);
```

Không được:

- block trong critical section;
- in UART;
- gọi user callback;
- giữ interrupt mask lâu;
- context switch giữa transition nửa hoàn thành.

---

# 29. Semaphore

Semaphore là counter đồng bộ.

Operation:

```text
take / wait / pend
give / post / signal
```

Semaphore không có ownership bắt buộc.

Task A có thể take, Task B có thể give tùy use case.

---

# 30. Binary semaphore

Count chỉ có:

```text
0 hoặc 1
```

Use case:

- ISR báo event cho task;
- task completion signal;
- one-shot notification.

Binary semaphore không phải mutex vì:

- không owner;
- task khác có thể give;
- priority inheritance không áp dụng;
- không bảo vệ ownership protocol.

---

# 31. Counting semaphore

Count:

```text
0 <= count <= max_count
```

Use case:

- số buffer trống;
- số packet đang chờ;
- số resource giống nhau;
- event count.

Take:

```text
count > 0 -> count--
count == 0 -> block/no-wait fail
```

Give:

```text
waiter exists -> wake waiter
else count < max -> count++
else overflow policy
```

---

# 32. Mutex

Mutex bảo vệ tài nguyên có ownership.

State:

```text
owner task
lock state
wait list
```

Lock:

- nếu free, current task thành owner;
- nếu current đã owner, xử lý theo non-recursive policy;
- nếu task khác owner, block;
- có thể priority inheritance.

Unlock:

- chỉ owner được unlock;
- nếu waiter có, chuyển ownership theo policy;
- restore priority nếu cần.

---

# 33. Semaphore và mutex khác nhau thế nào?

| Tiêu chí | Semaphore | Mutex |
|---|---|---|
| Ownership | Không bắt buộc | Bắt buộc |
| Give từ ISR | Có thể | Không |
| Unlock bởi task khác | Có thể signal | Không hợp lệ |
| Priority inheritance | Không | Có thể |
| Resource protection | Có thể nhưng không tối ưu | Mục đích chính |
| Event signaling | Phù hợp | Không phù hợp |

---

# 34. Mutex ownership

TCB có thể chứa:

```text
owned mutex list
mutex count
```

Mutex chứa:

```text
owner
wait list
```

Invariant:

- mutex locked thì owner không NULL;
- mutex free thì owner NULL;
- mutex nằm trong owned-mutex list của owner;
- non-owner unlock bị reject;
- owner không nằm trong wait list của chính mutex.

---

# 35. Recursive mutex có cần không?

Phiên bản đầu nên dùng non-recursive mutex.

Nếu owner lock lại:

```text
return HR_STATUS_DEADLOCK
```

Recursive mutex cần:

- recursion count;
- unlock đúng số lần;
- priority inheritance phức tạp hơn;
- debug ownership khó hơn.

Không cần cho Chủ đề 4 cơ bản.

---

# 36. Priority inversion

Ba task:

```text
High   priority 0
Medium priority 1
Low    priority 2
```

Kịch bản:

```text
Low lock mutex
High chạy và block vì mutex
Medium chạy CPU-bound
Low không được chạy để unlock
High bị trì hoãn bởi Medium
```

Đây là priority inversion.

---

# 37. Priority inheritance

Khi High block trên mutex do Low giữ:

```text
Low effective priority <- High priority
```

Low được chạy sớm hơn Medium, unlock mutex, rồi priority được restore.

```text
Low base = 2
Low effective = 0 while inheriting
```

Sau unlock:

```text
effective priority recomputed
```

Không luôn đơn giản gán lại base priority nếu owner còn giữ mutex khác có waiter cao.

---

# 38. Base priority và effective priority

TCB:

```c
uint8_t base_priority;
uint8_t effective_priority;
```

Scheduler dùng:

```text
effective priority
```

Application configuration dùng:

```text
base priority
```

Priority inheritance thay đổi effective priority tạm thời.

Khi effective priority đổi:

- task READY phải chuyển ready queue;
- owner đang RUNNING có thể ảnh hưởng scheduling;
- waiter ordering có thể cần cập nhật;
- ready bitmap phải nhất quán.

---

# 39. Waiter ordering

Wait list có thể:

- FIFO;
- priority ordered;
- hybrid priority + FIFO.

HairRTOS đề xuất:

```text
effective priority cao hơn đứng trước
FIFO nếu cùng priority
```

Ví dụ:

```text
Task A priority 2 arrives first
Task B priority 0 arrives later
Task C priority 2 arrives later

Order:
B -> A -> C
```

---

# 40. FIFO giữa waiter cùng priority

Mỗi task có sequence hoặc insertion order tự nhiên trong list.

Insert after các waiter cùng priority.

Điều này giúp:

- fairness;
- predictable ordering;
- tránh LIFO starvation.

---

# 41. ISR-safe synchronization API

Ví dụ:

```c
hr_status_t hr_semaphore_give_from_isr(
    hr_semaphore_t *semaphore,
    bool *higher_priority_task_woken);
```

ISR API phải:

- không block;
- không gọi task-only API;
- critical section phù hợp ISR;
- không gọi PendSV handler trực tiếp;
- chỉ request context switch.

---

# 42. Không được block trong ISR

ISR không có TCB như task.

Không được:

```c
hr_mutex_lock(&mutex,
              HR_WAIT_FOREVER);
```

từ ISR.

ISR phải:

- làm việc ngắn;
- signal task;
- return.

---

# 43. Task wake-up từ ISR

Luồng:

```text
External IRQ
    |
    v
give_from_isr()
    |
    v
wake highest-priority waiter
    |
    v
higher_priority_task_woken = true
    |
    v
Pend PendSV
    |
    v
ISR return
    |
    v
PendSV switches
```

---

# 44. Thiết kế task state trong TCB

TCB mở rộng:

```c
typedef struct hr_task
{
    uint32_t *saved_sp;

    hr_task_state_t state;

    uint8_t base_priority;
    uint8_t effective_priority;

    uint32_t wake_tick;
    hr_wait_result_t wait_result;
    void *waiting_object;

    hr_list_node_t ready_node;
    hr_list_node_t wait_node;
    hr_list_node_t timeout_node;
    hr_list_node_t all_task_node;

    ...
} hr_task_t;
```

---

# 45. Các list node cần thêm vào TCB

| Node | Mục đích |
|---|---|
| `ready_node` | Ready queue |
| `wait_node` | Semaphore/mutex/object wait list |
| `timeout_node` | Delayed/timeout list |
| `all_task_node` | Registry |
| `owned_mutex_node` | Không đủ nếu task giữ nhiều mutex; cần list ở mutex hoặc node per mutex |

Một node không được dùng đồng thời cho hai list.

---

# 46. Wait result và wake reason

Enum:

```c
typedef enum
{
    HR_WAIT_RESULT_NONE = 0,
    HR_WAIT_RESULT_SUCCESS,
    HR_WAIT_RESULT_TIMEOUT,
    HR_WAIT_RESULT_CANCELLED,
    HR_WAIT_RESULT_SUSPENDED
} hr_wait_result_t;
```

Task sau khi wake đọc kết quả.

Không dùng return value tạm nằm trên stack kernel nếu context switch xảy ra; kết quả nên lưu trong TCB hoặc protocol rõ ràng.

---

# 47. Task transition helpers

API nội bộ:

```c
bool hr_task_make_ready(
    hr_task_t *task,
    hr_wait_result_t result);

bool hr_task_block_current(
    void *object,
    hr_list_t *wait_list,
    uint32_t timeout_ticks);

bool hr_task_suspend_internal(
    hr_task_t *task);

bool hr_task_resume_internal(
    hr_task_t *task);
```

Chỉ helper được sửa:

- state;
- list membership;
- waiting object;
- timeout;
- ready bitmap.

---

# 48. Block current task

Pseudo-code:

```c
bool hr_task_block_current(
    void *object,
    hr_list_t *wait_list,
    uint32_t timeout_ticks)
{
    hr_task_t *task = hr_scheduler_current();

    HR_ASSERT(task != NULL);
    HR_ASSERT(task->state == HR_TASK_RUNNING);

    if (!hr_ready_remove(task))
    {
        return false;
    }

    task->waiting_object = object;
    task->wait_result = HR_WAIT_RESULT_NONE;

    if (!hr_wait_list_insert(wait_list, task))
    {
        hr_ready_insert(task);
        return false;
    }

    if (timeout_ticks != HR_WAIT_FOREVER)
    {
        task->wake_tick =
            hr_tick_now() + timeout_ticks;

        if (!hr_timeout_insert(task))
        {
            hr_wait_list_remove(wait_list, task);
            hr_ready_insert(task);
            return false;
        }
    }

    task->state = HR_TASK_STATE_BLOCKED;
    hr_port_request_context_switch();

    return true;
}
```

Toàn bộ transition phải ở critical section.

---

# 49. Wake blocked task

```c
bool hr_task_wake(
    hr_task_t *task,
    hr_wait_result_t result)
{
    if ((task == NULL) ||
        (task->state != HR_TASK_STATE_BLOCKED))
    {
        return false;
    }

    hr_wait_remove_if_linked(task);
    hr_timeout_remove_if_linked(task);

    task->waiting_object = NULL;
    task->wait_result = result;
    task->state = HR_TASK_STATE_READY;

    return hr_ready_insert(task);
}
```

---

# 50. Suspend và resume

Suspend:

```c
hr_status_t hr_task_suspend(hr_task_t *task);
```

Resume:

```c
hr_status_t hr_task_resume(hr_task_t *task);
```

Suspend current task:

- remove ready;
- state SUSPENDED;
- PendSV.

Suspend READY task:

- remove ready;
- state SUSPENDED.

Suspend BLOCKED task cần policy rõ.

---

# 51. Suspend khác block ở đâu?

| Đặc điểm | BLOCKED | SUSPENDED |
|---|---|---|
| Chờ object | Có thể | Không |
| Chờ timeout | Có thể | Không |
| Auto wake | Có | Không |
| Resume explicit | Không bắt buộc | Bắt buộc |
| Wait result | Có | Không hoặc SUSPENDED |
| Wait list | Có thể | Không |

---

# 52. Suspend task đang BLOCKED

Hai policy:

## Policy A — Reject

```text
Không cho suspend BLOCKED task.
```

Đơn giản hơn.

## Policy B — Cancel wait rồi suspend

- remove wait list;
- remove timeout list;
- set wait result SUSPENDED;
- clear object;
- state SUSPENDED.

HairRTOS nên chọn và document rõ. Không để task vẫn nằm trong wait list khi state SUSPENDED.

---

# 53. Kernel invariants

- RUNNING task là current task.
- READY task nằm đúng một ready queue.
- BLOCKED task không nằm ready queue.
- BLOCKED finite timeout nằm timeout list.
- BLOCKED wait object nằm object wait list.
- SUSPENDED task không nằm ready/wait/timeout list.
- `waiting_object == NULL` nếu task không BLOCKED.
- Timeout node owner đúng delayed list.
- Ready bitmap khớp queues.
- Current task không NULL khi kernel RUNNING.
- Idle task không block hoặc suspend.
- Task wake chỉ xảy ra một lần.

---

# 54. Synchronization object invariants

## Semaphore

- `count <= max_count`.
- Waiter tồn tại thì policy count phải nhất quán.
- Wait list ordered đúng.
- Waiter state BLOCKED.
- Waiter waiting object trỏ semaphore.

## Mutex

- free -> owner NULL.
- locked -> owner hợp lệ.
- owner không nằm wait list.
- non-owner unlock fail.
- waiters BLOCKED.
- inherited priority phản ánh waiter cao nhất theo policy.

---

# 55. Lock ordering và deadlock

Deadlock cổ điển:

```text
Task A lock M1
Task B lock M2
Task A waits M2
Task B waits M1
```

Giải pháp:

- global lock order;
- timeout;
- try-lock;
- tránh nested locks;
- ownership design;
- diagnostics.

Ví dụ order:

```text
M1 < M2 < M3
```

Task chỉ lock theo thứ tự tăng.

---

# 56. Starvation

Starvation có thể xảy ra khi:

- priority cao luôn chạy;
- waiter priority thấp luôn bị vượt;
- unfair wake policy;
- mutex liên tục được reacquire bởi task cao.

Mitigation:

- FIFO cùng priority;
- application blocking đúng;
- priority design;
- bounded critical sections;
- không spin.

---

# 57. Diagnostic và trace

Task diagnostic:

```text
name
state
base priority
effective priority
waiting object
wake tick
wait result
stack guard
```

Object diagnostic:

```text
semaphore count
max count
waiter names
mutex owner
mutex waiters
inheritance state
```

Trace events:

```text
TASK_BLOCK
TASK_WAKE
TASK_TIMEOUT
TASK_SUSPEND
TASK_RESUME
SEM_TAKE
SEM_GIVE
MUTEX_LOCK
MUTEX_UNLOCK
PRIORITY_INHERIT
PRIORITY_RESTORE
```

Không in trực tiếp trong critical path. Lưu ring buffer rồi snapshot.

---

# 58. Mã khung task state

```c
typedef enum
{
    HR_TASK_STATE_UNUSED = 0,
    HR_TASK_STATE_CREATED,
    HR_TASK_STATE_READY,
    HR_TASK_STATE_RUNNING,
    HR_TASK_STATE_BLOCKED,
    HR_TASK_STATE_SUSPENDED
} hr_task_state_t;
```

Transition validator:

```c
bool hr_task_transition_allowed(
    hr_task_state_t from,
    hr_task_state_t to)
{
    switch (from)
    {
        case HR_TASK_STATE_CREATED:
            return to == HR_TASK_STATE_READY;

        case HR_TASK_STATE_READY:
            return (to == HR_TASK_STATE_RUNNING) ||
                   (to == HR_TASK_STATE_SUSPENDED);

        case HR_TASK_STATE_RUNNING:
            return (to == HR_TASK_STATE_READY) ||
                   (to == HR_TASK_STATE_BLOCKED) ||
                   (to == HR_TASK_STATE_SUSPENDED);

        case HR_TASK_STATE_BLOCKED:
            return (to == HR_TASK_STATE_READY) ||
                   (to == HR_TASK_STATE_SUSPENDED);

        case HR_TASK_STATE_SUSPENDED:
            return to == HR_TASK_STATE_READY;

        default:
            return false;
    }
}
```

---

# 59. Mã khung tick comparison

```c
static inline bool hr_tick_reached(
    uint32_t now,
    uint32_t deadline)
{
    return (int32_t)(now - deadline) >= 0;
}

static inline bool hr_tick_before(
    uint32_t a,
    uint32_t b)
{
    return (int32_t)(a - b) < 0;
}
```

Contract:

```c
#define HR_MAX_FINITE_TIMEOUT_TICKS \
    ((uint32_t)INT32_MAX)
```

---

# 60. Mã khung delayed list

```c
static hr_list_t g_delayed_tasks;

bool hr_timeout_insert(hr_task_t *task)
{
    hr_list_node_t *node;

    node = hr_list_front(&g_delayed_tasks);

    while (node != NULL)
    {
        hr_task_t *other =
            HR_CONTAINER_OF(
                node,
                hr_task_t,
                timeout_node);

        if (hr_tick_before(
                task->wake_tick,
                other->wake_tick))
        {
            return hr_list_insert_before(
                &g_delayed_tasks,
                node,
                &task->timeout_node);
        }

        node = node->next;
    }

    return hr_list_push_back(
        &g_delayed_tasks,
        &task->timeout_node);
}
```

Lưu ý wrap-around ordering cần contract rõ về maximum horizon. Một sorted list duy nhất qua wrap boundary cần kiểm thử kỹ.

---

# 61. Mã khung task delay

```c
hr_status_t hr_task_delay(uint32_t ticks)
{
    uint32_t state;
    hr_task_t *task;

    if (ticks == 0U)
    {
        hr_task_yield();
        return HR_STATUS_OK;
    }

    state = hr_port_critical_enter();

    task = hr_scheduler_current();

    if ((task == NULL) ||
        (task->state != HR_TASK_STATE_RUNNING))
    {
        hr_port_critical_exit(state);
        return HR_STATUS_INVALID_STATE;
    }

    task->wake_tick = hr_tick_now() + ticks;
    task->waiting_object = NULL;
    task->wait_result = HR_WAIT_RESULT_NONE;

    hr_ready_remove(task);
    hr_timeout_insert(task);

    task->state = HR_TASK_STATE_BLOCKED;

    hr_port_request_context_switch();
    hr_port_critical_exit(state);

    return HR_STATUS_OK;
}
```

---

# 62. Mã khung binary semaphore

```c
typedef struct
{
    bool available;
    hr_list_t waiters;
} hr_binary_semaphore_t;
```

Take:

```c
hr_status_t hr_binary_semaphore_take(
    hr_binary_semaphore_t *sem,
    uint32_t timeout)
{
    uint32_t state;

    if (sem == NULL)
    {
        return HR_STATUS_INVALID_ARGUMENT;
    }

    state = hr_port_critical_enter();

    if (sem->available)
    {
        sem->available = false;
        hr_port_critical_exit(state);
        return HR_STATUS_OK;
    }

    if (timeout == HR_NO_WAIT)
    {
        hr_port_critical_exit(state);
        return HR_STATUS_WOULD_BLOCK;
    }

    hr_block_current_on_object(
        sem,
        &sem->waiters,
        timeout);

    hr_port_critical_exit(state);

    return hr_current_wait_result();
}
```

Thực tế return sau block cần API design phù hợp call stack khi task resume.

---

# 63. Mã khung counting semaphore

```c
typedef struct
{
    uint32_t count;
    uint32_t max_count;
    hr_list_t waiters;
} hr_counting_semaphore_t;
```

Give:

```c
hr_status_t hr_counting_semaphore_give(
    hr_counting_semaphore_t *sem)
{
    uint32_t state;
    hr_task_t *waiter;

    if (sem == NULL)
    {
        return HR_STATUS_INVALID_ARGUMENT;
    }

    state = hr_port_critical_enter();

    waiter = hr_wait_list_take_first(
        &sem->waiters);

    if (waiter != NULL)
    {
        hr_task_wake(
            waiter,
            HR_WAIT_RESULT_SUCCESS);

        hr_scheduler_request_preemption_if_needed(
            waiter);

        hr_port_critical_exit(state);
        return HR_STATUS_OK;
    }

    if (sem->count >= sem->max_count)
    {
        hr_port_critical_exit(state);
        return HR_STATUS_OVERFLOW;
    }

    sem->count++;

    hr_port_critical_exit(state);
    return HR_STATUS_OK;
}
```

---

# 64. Mã khung mutex

```c
typedef struct
{
    hr_task_t *owner;
    hr_list_t waiters;
} hr_mutex_t;
```

Lock:

```c
hr_status_t hr_mutex_lock(
    hr_mutex_t *mutex,
    uint32_t timeout)
{
    uint32_t state;
    hr_task_t *current;

    if (mutex == NULL)
    {
        return HR_STATUS_INVALID_ARGUMENT;
    }

    state = hr_port_critical_enter();
    current = hr_scheduler_current();

    if (mutex->owner == NULL)
    {
        mutex->owner = current;
        hr_task_owned_mutex_add(
            current,
            mutex);

        hr_port_critical_exit(state);
        return HR_STATUS_OK;
    }

    if (mutex->owner == current)
    {
        hr_port_critical_exit(state);
        return HR_STATUS_DEADLOCK;
    }

    if (timeout == HR_NO_WAIT)
    {
        hr_port_critical_exit(state);
        return HR_STATUS_WOULD_BLOCK;
    }

    hr_priority_inherit(
        mutex->owner,
        current->effective_priority);

    hr_block_current_on_object(
        mutex,
        &mutex->waiters,
        timeout);

    hr_port_critical_exit(state);

    return hr_current_wait_result();
}
```

---

# 65. Mã khung priority inheritance

```c
void hr_priority_inherit(
    hr_task_t *owner,
    uint8_t waiter_priority)
{
    if ((owner == NULL) ||
        (waiter_priority >=
         owner->effective_priority))
    {
        return;
    }

    hr_scheduler_change_effective_priority(
        owner,
        waiter_priority);
}
```

Restore:

```c
void hr_priority_recompute(
    hr_task_t *task)
{
    uint8_t effective =
        task->base_priority;

    hr_mutex_t *mutex =
        hr_task_first_owned_mutex(task);

    while (mutex != NULL)
    {
        hr_task_t *waiter =
            hr_mutex_highest_waiter(mutex);

        if ((waiter != NULL) &&
            (waiter->effective_priority <
             effective))
        {
            effective =
                waiter->effective_priority;
        }

        mutex =
            hr_task_next_owned_mutex(
                task,
                mutex);
    }

    hr_scheduler_change_effective_priority(
        task,
        effective);
}
```

Đây là phần mở rộng HairRTOS, không phải chi tiết được nêu trong bảng AKOS.

---

# 66. Chiến lược kiểm thử

## Host tests

- state transition matrix;
- ready/block/suspend list membership;
- delayed list ordering;
- tick wrap-around;
- delay-until;
- binary semaphore;
- counting semaphore;
- mutex ownership;
- non-owner unlock;
- priority inversion scenario;
- priority inheritance;
- timeout/object race;
- duplicate wake;
- waiter ordering.

## Target tests

- real SysTick;
- PendSV after wake;
- ISR give semaphore;
- high task preempts low;
- mutex priority inheritance;
- stack guards;
- trace.

## Randomized tests

Random operations:

```text
delay
tick
sem take
sem give
mutex lock
mutex unlock
suspend
resume
timeout
```

Sau mỗi operation:

```c
assert(hr_kernel_validate());
```

---

# 67. Lỗi thường gặp

## 67.1 Chỉ đổi state enum

Không cập nhật list.

## 67.2 BLOCKED task vẫn nằm ready queue

Scheduler vẫn chọn task đang chờ.

## 67.3 Finite waiter chỉ nằm object list

Timeout không bao giờ xảy ra.

## 67.4 Dùng cùng node cho wait list và timeout list

Corruption.

## 67.5 Timeout và object cùng wake task

Duplicate ready insertion.

## 67.6 `now >= deadline` khi wrap

Timeout sai.

## 67.7 Suspend nhưng không remove wait list

Object give sau đó wake task SUSPENDED.

## 67.8 Binary semaphore dùng như mutex

Không ownership, không PI.

## 67.9 Mutex unlock bởi non-owner

Resource protocol bị phá.

## 67.10 ISR gọi blocking API

Không có task context để block.

## 67.11 Give semaphore tăng count dù đã wake waiter

Resource được tính hai lần.

## 67.12 Priority inheritance không di chuyển READY owner

Owner vẫn nằm queue cũ.

## 67.13 Restore priority trực tiếp về base

Sai nếu owner còn mutex khác có waiter cao.

## 67.14 In UART trong critical section

Interrupt latency tăng.

## 67.15 Idle task block

Ready bitmap có thể rỗng.

---

# 68. Bài thực hành

## Bài 01 — Task state machine on host

### Mục tiêu

- Cài state enum.
- Validate transition.
- Kiểm tra READY/RUNNING/BLOCKED/SUSPENDED.

### Tests

- valid transitions;
- invalid transitions;
- current task rule;
- suspended auto-wake reject.

### Fault injection

Cho phép:

```text
SUSPENDED -> RUNNING
```

trực tiếp và giải thích invariant bị phá.

### Tiêu chí hoàn thành

- Transition matrix đầy đủ.
- ASan/UBSan pass.
- State không đổi nếu transition fail.

---

## Bài 02 — Efficient blocking and ready-list removal

### Mục tiêu

- Remove current khỏi ready queue khi block.
- Insert lại khi wake.
- Không busy-wait.

### Tests

- block RUNNING;
- block invalid state;
- wake BLOCKED;
- duplicate wake;
- priority preemption decision.

### Tiêu chí hoàn thành

- BLOCKED không còn trong ready queue.
- Wake insert đúng priority.
- Ready bitmap đúng.

---

## Bài 03 — Delayed task list and tick wrap

### Mục tiêu

- Sorted delayed list.
- Wake due tasks.
- Tick wrap-safe comparison.

### Tests

- increasing deadlines;
- same deadline;
- multiple wake cùng tick;
- wrap `0xFFFFFFFF -> 0`;
- maximum finite timeout contract.

### Tiêu chí hoàn thành

- Head là deadline gần nhất.
- Không quét task chưa tới hạn.
- Wrap tests pass.

---

## Bài 04 — Delay and delay-until

### Mục tiêu

- Relative delay.
- Absolute periodic delay.
- Quan sát drift.

### Demo

So sánh:

```text
delay(period)
delay_until(next, period)
```

### Tiêu chí hoàn thành

- Delay 0 policy rõ.
- Periodic release đúng.
- Deadline miss được ghi lại.

---

## Bài 05 — Binary and counting semaphore

### Mục tiêu

- Binary event signaling.
- Counting resource.
- Priority-ordered waiters.

### Tests

- immediate take;
- no-wait fail;
- finite timeout;
- wait forever;
- give wakes waiter;
- count overflow;
- FIFO same priority.

### Tiêu chí hoàn thành

- Count invariant.
- Không double-count khi wake waiter.
- Host tests pass.

---

## Bài 06 — ISR-to-task semaphore wake

### Mục tiêu

- `give_from_isr`.
- `higher_priority_task_woken`.
- Deferred PendSV.

### Target

Button EXTI hoặc timer ISR signal task high.

### Tiêu chí hoàn thành

- ISR không block.
- High task chạy sau ISR return.
- Trace chứng minh deferred switch.

---

## Bài 07 — Mutex ownership

### Mục tiêu

- Lock/unlock.
- Owner validation.
- Wait list.

### Tests

- free lock;
- recursive lock reject;
- non-owner unlock reject;
- transfer ownership;
- timeout.

### Tiêu chí hoàn thành

- Owner invariant.
- Semaphore không được dùng thay mutex trong test.

---

## Bài 08 — Priority inversion and inheritance

### Mục tiêu

- Tạo High/Medium/Low scenario.
- Quan sát inversion.
- Bật priority inheritance.

### Tests

- Low owns mutex;
- High blocks;
- Medium ready;
- Low effective priority boosted;
- restore after unlock.

### Tiêu chí hoàn thành

- Trace trước/sau PI.
- Effective priority đúng.
- Owner chuyển ready queue khi priority đổi.

---

## Bài 09 — Suspend and resume

### Mục tiêu

- Suspend READY/RUNNING.
- Resume.
- Policy cho BLOCKED task.

### Tests

- suspend current;
- suspend ready;
- double suspend;
- resume non-suspended;
- timeout không wake suspended task.

### Tiêu chí hoàn thành

- SUSPENDED không nằm ready/wait/timeout list.
- Chỉ resume explicit mới wake.

---

## Bài 10 — Target Synchronization Playground

### Mục tiêu

Chạy trên STM32F103:

- task states;
- delays;
- semaphores;
- mutex;
- ISR wake-up;
- priority inheritance;
- diagnostics.

### UART commands

```text
help
tasks
delayed
semaphores
mutexes
trace
validate
pi-demo
suspend-demo
```

### Tiêu chí hoàn thành

- UART report.
- ISR semaphore demo.
- PI demo.
- Task state validation.
- Stack guards.
- Target report.

---

# 69. Project tổng kết chủ đề

## Tên project

```text
Synchronization Playground
```

## 69.1 Mục tiêu

Firmware độc lập minh họa:

- task state machine;
- efficient blocking;
- delayed list;
- finite timeout;
- binary semaphore;
- counting semaphore;
- mutex ownership;
- priority inversion;
- priority inheritance;
- ISR-to-task wake-up;
- suspend/resume;
- diagnostics.

## 69.2 Kiến trúc

```text
+---------------------------------------------------+
| Application Tasks                                 |
|                                                   |
| High Control Task                                 |
| Medium CPU Task                                   |
| Low Resource Task                                 |
| Producer Task                                     |
| Consumer Task                                     |
| Monitor Task                                      |
| Idle Task                                         |
+-------------------------+-------------------------+
                          |
                          v
+---------------------------------------------------+
| Synchronization                                   |
|                                                   |
| Binary Semaphore                                  |
| Counting Semaphore                                |
| Mutex + Priority Inheritance                      |
+-------------------------+-------------------------+
                          |
                          v
+---------------------------------------------------+
| Task State and Timeout                            |
|                                                   |
| Ready queues                                      |
| Delayed list                                      |
| Object wait lists                                 |
| Suspend/resume                                    |
+-------------------------+-------------------------+
                          |
                          v
+---------------------------------------------------+
| Cortex-M3 Port                                    |
|                                                   |
| SVC  PendSV  SysTick  Critical Section            |
+-------------------------+-------------------------+
                          |
                          v
+---------------------------------------------------+
| STM32F103 Platform                                |
+---------------------------------------------------+
```

## 69.3 Demo scenarios

### Semaphore demo

```text
Timer/Button ISR
    |
    v
Binary semaphore give_from_isr
    |
    v
High task wake
```

### Producer/consumer count demo

```text
Producer gives counting semaphore
Consumer takes semaphore
```

### Priority inversion demo

```text
Low locks mutex
High blocks
Medium runs
```

Sau khi bật PI:

```text
Low inherits High priority
Low unlocks
High continues
```

### Suspend demo

```text
Monitor suspends worker
Timeout/event không wake
Monitor resumes worker
```

## 69.4 UART commands

```text
h  help
t  tasks
d  delayed list
s  semaphores
m  mutexes
p  priority-inheritance demo
u  suspend/resume demo
r  trace
v  validate
```

## 69.5 Requirements

- Static synchronization objects.
- No heap in kernel.
- Priority-ordered waiters.
- FIFO among equals.
- Finite timeout.
- Wait forever.
- No-wait.
- Timeout wrap tests.
- Single-winner wake.
- ISR-safe give.
- Mutex ownership.
- Priority inheritance.
- Stack guards.
- Host tests.
- Target validation.

---

# 70. Cấu trúc repository đề xuất

```text
04-task-state-synchronization/
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
│   ├── wait_list.h
│   ├── semaphore.h
│   ├── mutex.h
│   ├── synchronization_trace.h
│   └── synchronization_playground.h
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
│   ├── wait_list.c
│   ├── semaphore.c
│   ├── mutex.c
│   ├── synchronization_trace.c
│   ├── synchronization_playground.c
│   ├── cortex_m3_port.c
│   └── cortex_m3_portasm.S
│
├── labs/
│   ├── README.md
│   ├── 01-task-state-machine/
│   ├── 02-efficient-blocking/
│   ├── 03-delayed-list-tick-wrap/
│   ├── 04-delay-until/
│   ├── 05-semaphore-host/
│   ├── 06-isr-semaphore-wake/
│   ├── 07-mutex-ownership/
│   ├── 08-priority-inheritance/
│   ├── 09-suspend-resume/
│   └── 10-target-synchronization-playground/
│
├── docs/
│   ├── task-state-machine.md
│   ├── efficient-blocking.md
│   ├── delayed-list.md
│   ├── timeout-model.md
│   ├── semaphore.md
│   ├── mutex.md
│   ├── priority-inversion.md
│   ├── priority-inheritance.md
│   ├── isr-safe-api.md
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

# 72. Liên hệ với roadmap HairRTOS

Chủ đề này bao phủ các phase:

## Phase 7 — SysTick, delay and timeout

- kernel tick;
- delay;
- delay-until;
- delayed list;
- timeout;
- wrap-safe comparison.

## Phase 9 — Queue foundation

Queue chưa phải trọng tâm, nhưng blocking model và wait lists được tái sử dụng.

## Phase 10 — Semaphore and mutex

- binary semaphore;
- counting semaphore;
- mutex ownership;
- priority inheritance.

## Phase 11 — Suspend/resume

- SUSPENDED state;
- suspend/resume API;
- ISR resume policy nếu có.

---

# 74. Tiêu chí hoàn thành

## Task state

- READY.
- RUNNING.
- BLOCKED.
- SUSPENDED.
- Transition validation.
- List membership consistent.
- Current task invariant.
- Idle task invariant.

## Blocking and timeout

- Efficient blocking.
- Delayed list.
- Sorted wake order.
- Tick wrap test.
- Delay.
- Delay-until.
- No-wait.
- Finite timeout.
- Wait forever.
- Single-winner wake.
- Timeout/object race test.

## Semaphore

- Binary semaphore.
- Counting semaphore.
- Count invariant.
- Priority waiters.
- FIFO equals.
- ISR-safe give.
- No blocking in ISR.

## Mutex

- Ownership.
- Non-owner unlock rejection.
- Recursive lock policy.
- Wait list.
- Priority inversion demo.
- Priority inheritance.
- Effective priority update.
- Priority restore.

## Suspend/resume

- Suspend READY.
- Suspend RUNNING.
- BLOCKED policy.
- Resume.
- No auto-wake while suspended.
- Double suspend handling.

## Tests and target

- Host tests.
- ASan.
- UBSan.
- Randomized state tests.
- STM32 build.
- UART diagnostics.
- ISR wake demo.
- PI demo.
- Stack guards.
- Validation report.

---

# 75. Tổng kết

Chủ đề 4 trả lời:

```text
Task làm gì khi chưa thể tiếp tục chạy?
```

và:

```text
Nhiều task phối hợp và bảo vệ tài nguyên dùng chung như thế nào?
```

Luồng kiến thức:

```text
Task states
      |
      v
Efficient blocking
      |
      v
Delayed list
      |
      v
Timeout
      |
      v
Semaphore
      |
      v
Mutex ownership
      |
      v
Priority inversion
      |
      v
Priority inheritance
      |
      v
ISR-safe wake-up
      |
      v
Synchronization Playground
```

Các nguyên tắc cần nhớ:

1. BLOCKED task không nằm trong ready queue.
2. SUSPENDED task không tự wake.
3. Blocking phải thay busy-wait khi thời gian chờ không ngắn và có scheduler.
4. Finite waiter cần cả wait node và timeout node.
5. Một node chỉ thuộc một list.
6. Timeout và object wake phải có single winner.
7. Tick comparison phải an toàn khi wrap.
8. Delay-until giảm drift.
9. Semaphore không có ownership.
10. Mutex có ownership.
11. ISR không được block.
12. ISR chỉ request deferred switch.
13. Priority inversion cần được hiểu trước khi dùng mutex.
14. Priority inheritance thay đổi effective priority.
15. Priority restore phải xét các mutex còn sở hữu.
16. Waiter priority cao được wake trước.
17. FIFO áp dụng giữa waiter cùng priority.
18. Critical section phải ngắn.
19. Không UART trong PendSV hoặc object transition.
20. Host test state machine trước khi chạy target.

Sau chủ đề này, kết quả cần đạt:

```text
Task state machine đã được kiểm thử
+
Delayed list và timeout model
+
Semaphore
+
Mutex ownership
+
Priority inheritance cơ bản
+
Suspend/resume
+
Synchronization Playground chạy trên STM32F103
```

Chủ đề tiếp theo:

```text
Communication + Timer + Benchmark
```

sẽ tái sử dụng:

- blocking model;
- timeout;
- wait list;
- ISR wake-up;
- scheduler;
- task states;
- synchronization object.

---

## Nguồn chương trình

Phạm vi gốc của README được lấy từ Chủ đề 4 trong tài liệu:

```text
self-develop-rtos.pdf

Task state + Synchronization
    - Efficient blocking of threads
    - Task state: Ready, Running, Blocked, Suspended
    - Ready list và priority scheduling
    - Delayed task list
    - Synchronization and mutual exclusion mechanisms
```

Những phần như timeout race handling, semaphore API, mutex ownership, priority inversion, priority inheritance, ISR-safe API, hệ thống lab và project tổng kết là phần mở rộng thực hành dành cho HairRTOS.
