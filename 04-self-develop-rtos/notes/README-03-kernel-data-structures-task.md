# Chủ đề 3 - Cấu trúc dữ liệu trong Kernel và Task
## Kernel Data Structures, Intrusive Lists và Task Control Block

> README này được xây dựng theo phạm vi **Chủ đề 3 — “Cấu trúc dữ liệu trong kernel + Task”** của chương trình tự phát triển AKOS.

---

## Mục lục

1. [Mục tiêu học tập](#1-mục-tiêu-học-tập)
2. [Phạm vi của chủ đề](#2-phạm-vi-của-chủ-đề)
3. [Kiến thức cần chuẩn bị](#3-kiến-thức-cần-chuẩn-bị)
4. [Tại sao kernel cần cấu trúc dữ liệu riêng?](#4-tại-sao-kernel-cần-cấu-trúc-dữ-liệu-riêng)
5. [Những yêu cầu đặc biệt của cấu trúc dữ liệu trong RTOS](#5-những-yêu-cầu-đặc-biệt-của-cấu-trúc-dữ-liệu-trong-rtos)
6. [Các loại linked list cơ bản](#6-các-loại-linked-list-cơ-bản)
7. [Singly linked list](#7-singly-linked-list)
8. [Doubly linked list](#8-doubly-linked-list)
9. [Circular linked list](#9-circular-linked-list)
10. [Sentinel node](#10-sentinel-node)
11. [Intrusive linked list](#11-intrusive-linked-list)
12. [Intrusive list và non-intrusive list](#12-intrusive-list-và-non-intrusive-list)
13. [List node ownership](#13-list-node-ownership)
14. [List membership và duplicate insertion](#14-list-membership-và-duplicate-insertion)
15. [List invariants](#15-list-invariants)
16. [API cho intrusive doubly linked list](#16-api-cho-intrusive-doubly-linked-list)
17. [Mã khung intrusive list](#17-mã-khung-intrusive-list)
18. [Container-of](#18-container-of)
19. [Kiểm thử linked list trên host](#19-kiểm-thử-linked-list-trên-host)
20. [Task trong RTOS là gì?](#20-task-trong-rtos-là-gì)
21. [Task Control Block là gì?](#21-task-control-block-là-gì)
22. [TCB lưu những gì?](#22-tcb-lưu-những-gì)
23. [Saved stack pointer và contract với assembly](#23-saved-stack-pointer-và-contract-với-assembly)
24. [Thông tin stack trong TCB](#24-thông-tin-stack-trong-tcb)
25. [Priority trong TCB](#25-priority-trong-tcb)
26. [Task state tối thiểu trong Chủ đề 3](#26-task-state-tối-thiểu-trong-chủ-đề-3)
27. [Task entry và argument](#27-task-entry-và-argument)
28. [Static task creation](#28-static-task-creation)
29. [Task stack initialization](#29-task-stack-initialization)
30. [Task registry và all-task list](#30-task-registry-và-all-task-list)
31. [Ready lists trong kernel](#31-ready-lists-trong-kernel)
32. [Một TCB có thể nằm trong nhiều list không?](#32-một-tcb-có-thể-nằm-trong-nhiều-list-không)
33. [Tổ chức nhiều list node trong một TCB](#33-tổ-chức-nhiều-list-node-trong-một-tcb)
34. [Current task và next task](#34-current-task-và-next-task)
35. [Task lifecycle ở mức dữ liệu](#35-task-lifecycle-ở-mức-dữ-liệu)
36. [Task naming và diagnostics](#36-task-naming-và-diagnostics)
37. [Stack guard và high-water mark](#37-stack-guard-và-high-water-mark)
38. [Kernel assertions](#38-kernel-assertions)
39. [Error handling trong task creation](#39-error-handling-trong-task-creation)
40. [Mã khung TCB và task API](#40-mã-khung-tcb-và-task-api)
41. [Mã khung static task creation](#41-mã-khung-static-task-creation)
42. [Mã khung task registry](#42-mã-khung-task-registry)
43. [Mã khung ready queue](#43-mã-khung-ready-queue)
44. [Chiến lược kiểm thử](#44-chiến-lược-kiểm-thử)
45. [Lỗi thường gặp](#45-lỗi-thường-gặp)
46. [Bài thực hành](#46-bài-thực-hành)
47. [Project tổng kết chủ đề](#47-project-tổng-kết-chủ-đề)
48. [Cấu trúc repository đề xuất](#48-cấu-trúc-repository-đề-xuất)
49. [Liên hệ với roadmap HairRTOS](#49-liên-hệ-với-roadmap-hairtos)
50. [Tiêu chí hoàn thành](#50-tiêu-chí-hoàn-thành)
51. [Tổng kết](#51-tổng-kết)

---

# 1. Mục tiêu học tập

Sau khi hoàn thành chủ đề này, người học cần có khả năng:

- Giải thích được vì sao kernel RTOS cần linked list.
- Phân biệt được:
  - singly linked list;
  - doubly linked list;
  - circular linked list;
  - sentinel-based list;
  - intrusive list;
  - non-intrusive list.
- Giải thích được ưu và nhược điểm của intrusive list.
- Thiết kế được API linked list không phụ thuộc phần cứng.
- Xác định được list ownership và node ownership.
- Phát hiện được:
  - duplicate insertion;
  - remove node không thuộc list;
  - list corruption;
  - count mismatch;
  - broken forward/backward link.
- Viết được host tests cho list bằng ASan và UBSan.
- Giải thích được task trong RTOS là gì.
- Giải thích được vai trò của Task Control Block.
- Thiết kế được một TCB tối thiểu cho Cortex-M3.
- Giải thích được vì sao `saved_sp` thường phải nằm ở offset 0.
- Thiết kế được static task creation.
- Tạo được task stack tĩnh.
- Build được initial task frame hoặc gọi architecture port để tạo frame.
- Tổ chức được:
  - all-task list;
  - per-priority ready list;
  - current task;
  - idle task.
- Phân biệt được một task object với một list node.
- Giải thích được tại sao một TCB cần nhiều intrusive node khác nhau.
- Theo dõi được:
  - task name;
  - priority;
  - state;
  - stack bounds;
  - stack high-water;
  - runtime counter.
- Tạo được project tổng kết giúp quan sát TCB và các kernel list.

---

# 2. Phạm vi của chủ đề

Theo chương trình AKOS, Chủ đề 3 bao gồm:

```text
Cấu trúc dữ liệu trong kernel + Task
    |
    +-- Tasks trong AK-mOS
    +-- Task Control Block
    +-- Các loại linked list
    +-- Case study linked list trong AK-mOS
```

Trong README này, phạm vi được mở rộng theo hướng HairRTOS:

```text
Linked-list theory
        |
        v
Intrusive doubly linked list
        |
        v
List invariant và host tests
        |
        v
TCB layout
        |
        v
Static task creation
        |
        v
All-task list
        |
        v
Per-priority ready queues
        |
        v
Task diagnostics
        |
        v
Task Inspector project
```

## Ranh giới của chủ đề

Chủ đề này tập trung vào:

- linked list;
- intrusive node;
- list container;
- TCB;
- task stack metadata;
- static task creation;
- task registry;
- ready list;
- task diagnostic.

Chủ đề này **không đi sâu** vào:

- task blocking;
- delayed task list;
- timeout;
- queue;
- semaphore;
- mutex;
- priority inheritance;
- software timer;
- event framework.

Những nội dung đó thuộc các chủ đề sau.

---

# 3. Kiến thức cần chuẩn bị

## 3.1 Kiến thức từ Chủ đề 1

- MCU memory layout.
- Static allocation.
- Stack.
- Alignment.
- Linker script.
- Startup.
- Assertions.
- Host testing.

## 3.2 Kiến thức từ Chủ đề 2

- Scheduler là gì.
- Current task.
- Next task.
- Fixed-priority scheduling.
- Ready queue.
- Initial task stack frame.
- SVC.
- PendSV.
- Saved stack pointer.
- Task chạy bằng PSP.

## 3.3 Ngôn ngữ C

Người học cần nắm:

- `struct`;
- nested `struct`;
- pointer;
- pointer arithmetic;
- `offsetof`;
- function pointer;
- `enum`;
- `size_t`;
- `uintptr_t`;
- macro;
- `_Static_assert`;
- `const`;
- `volatile`;
- compiler attribute ở mức cơ bản.

## 3.4 Công cụ

- GCC hoặc Clang host compiler.
- AddressSanitizer.
- UndefinedBehaviorSanitizer.
- GNU Arm Embedded Toolchain hoặc Clang cross-compiler.
- Make.
- GDB.
- `nm`.
- `objdump`.
- map file.

---

# 4. Tại sao kernel cần cấu trúc dữ liệu riêng?

Kernel phải quản lý nhiều object:

```text
Tasks
Ready tasks
Blocked tasks
Delayed tasks
Semaphore waiters
Mutex waiters
Software timers
Event subscribers
All created objects
```

Các object này thay đổi trạng thái theo thời gian.

Ví dụ:

```text
Task A
    |
    +-- đang nằm trong all-task list;
    +-- đang nằm trong ready queue priority 1;
    +-- có thể sau đó chuyển sang delayed list;
    +-- có thể chờ semaphore;
    +-- có thể quay lại ready queue.
```

Kernel cần thao tác:

- insert;
- remove;
- move;
- select;
- iterate;
- validate.

Linked list phù hợp vì:

- không cần dịch chuyển toàn bộ phần tử;
- node có thể được nhúng trong object;
- insert/remove có thể thực hiện nhanh khi đã có node;
- capacity không cần mảng pointer cố định;
- phù hợp static allocation.

Tuy nhiên, linked list cũng có rủi ro:

- pointer corruption;
- duplicate insertion;
- remove sai list;
- list cycle ngoài ý muốn;
- count sai;
- concurrency race.

---

# 5. Những yêu cầu đặc biệt của cấu trúc dữ liệu trong RTOS

Một linked list dùng trong kernel không chỉ cần “chạy đúng” ở trường hợp bình thường.

Nó cần:

- thời gian thao tác dễ dự đoán;
- không cấp phát động ngầm;
- không gọi callback application;
- không dùng recursion;
- không ẩn ownership;
- có invariant rõ ràng;
- có assert trong debug build;
- có host unit test;
- có API nhỏ;
- có hành vi rõ khi input sai;
- có quy tắc dùng trong critical section.

## 5.1 Determinism

Kernel cần biết chi phí:

```text
push_back: O(1)
remove known node: O(1)
pop_front: O(1)
find arbitrary node: O(n)
```

## 5.2 Không dynamic allocation ngầm

Không nên:

```c
list_push_back(list, task);
```

rồi bên trong tự gọi `malloc()` để tạo node.

HairRTOS theo static-first, vì vậy node được nhúng trực tiếp trong object.

## 5.3 Không callback khi list đang bị khóa

List operation không nên gọi user callback trong critical section.

## 5.4 Debug support

Debug build nên kiểm tra:

- node đã nằm trong list chưa;
- list owner có đúng không;
- forward/backward link có nhất quán không;
- count có đúng không;
- head/tail có hợp lệ không.

---

# 6. Các loại linked list cơ bản

Các dạng phổ biến:

```text
Singly linked list
Doubly linked list
Circular linked list
Sentinel-based list
Intrusive list
```

Không có một loại list tốt nhất cho mọi trường hợp.

Lựa chọn phụ thuộc:

- thao tác chính;
- yêu cầu memory;
- cần remove từ giữa hay không;
- có cần duyệt hai chiều hay không;
- có cần biết owner object hay không;
- có cần nhiều membership đồng thời hay không.

---

# 7. Singly linked list

Node:

```c
typedef struct singly_node
{
    struct singly_node *next;
} singly_node_t;
```

Cấu trúc:

```text
HEAD
 |
 v
[A] -> [B] -> [C] -> NULL
```

## Ưu điểm

- mỗi node chỉ cần một pointer;
- đơn giản;
- push front O(1);
- phù hợp free list hoặc stack.

## Nhược điểm

- remove node cần biết previous;
- không duyệt ngược;
- remove tail thường O(n) nếu không có metadata bổ sung;
- khó thao tác middle node hơn doubly list.

## Trường hợp phù hợp

- fixed-size free block pool;
- LIFO list;
- simple pending command list;
- single producer list được quản lý chặt.

---

# 8. Doubly linked list

Node:

```c
typedef struct doubly_node
{
    struct doubly_node *previous;
    struct doubly_node *next;
} doubly_node_t;
```

Cấu trúc:

```text
NULL <- [A] <-> [B] <-> [C] -> NULL
```

## Ưu điểm

- remove known node O(1);
- insert trước hoặc sau known node O(1);
- duyệt hai chiều;
- phù hợp task chuyển giữa nhiều container.

## Nhược điểm

- mỗi node tốn hai pointer;
- nhiều pointer hơn nên nhiều invariant hơn;
- corruption khó debug hơn nếu không có validator.

## Trường hợp phù hợp

- ready queue;
- delayed list;
- wait list;
- all-task list;
- software timer list.

---

# 9. Circular linked list

Circular list:

```text
[A] <-> [B] <-> [C]
 ^                 |
 |_________________|
```

Không có `NULL` ở cuối.

## Ưu điểm

- round-robin thuận tiện;
- từ tail có thể về head;
- có thể dùng sentinel duy nhất.

## Nhược điểm

- loop condition dễ sai;
- corruption có thể tạo infinite loop;
- cần convention rõ cho empty list.

## Trường hợp phù hợp

- round-robin ready queue;
- cyclic object iteration.

HairRTOS có thể dùng doubly list tuyến tính hoặc circular list. Điều quan trọng là API và invariant phải nhất quán.

---

# 10. Sentinel node

Sentinel là node đặc biệt không đại diện object thật.

```text
[SENTINEL] <-> [A] <-> [B] <-> [SENTINEL]
```

## Ưu điểm

- giảm số trường hợp riêng cho head/tail;
- empty list có thể biểu diễn bằng sentinel tự trỏ;
- insert/remove đồng nhất hơn.

## Nhược điểm

- container-of không được áp dụng lên sentinel như object thật;
- người học dễ nhầm sentinel với phần tử;
- debug cần nhận biết sentinel.

## Empty list

```c
sentinel.next = &sentinel;
sentinel.previous = &sentinel;
```

List empty khi:

```c
sentinel.next == &sentinel
```

---

# 11. Intrusive linked list

Intrusive list không cấp node riêng.

Node nằm trực tiếp trong object.

```c
typedef struct
{
    list_node_t ready_node;
    list_node_t all_task_node;

    uint8_t priority;
    const char *name;
} task_t;
```

Cấu trúc:

```text
task object
+----------------------+
| ready_node           |
| all_task_node        |
| priority             |
| name                 |
+----------------------+
```

List chỉ liên kết các node:

```text
ready list
    |
    v
task_a.ready_node <-> task_b.ready_node
```

## Ưu điểm

- không cần malloc;
- không cần node wrapper;
- ownership rõ;
- remove known node O(1);
- phù hợp static kernel object;
- không có allocation failure khi insert.

## Nhược điểm

- object phải chứa node;
- một node chỉ có thể thuộc một list tại một thời điểm;
- cần `container_of`;
- dễ dùng nhầm node;
- TCB lớn hơn nếu cần nhiều list membership.

---

# 12. Intrusive list và non-intrusive list

## Non-intrusive

```c
typedef struct list_entry
{
    void *object;
    struct list_entry *next;
} list_entry_t;
```

Cần một entry riêng cho mỗi membership.

```text
list entry -> task object
```

## Intrusive

```text
task object chứa list node
```

So sánh:

| Tiêu chí | Intrusive | Non-intrusive |
|---|---|---|
| Dynamic allocation | Không bắt buộc | Thường cần wrapper |
| Remove known object | O(1) | Phụ thuộc wrapper |
| Memory overhead | Node trong object | Node + object pointer |
| Type safety | Cần macro/container-of | Object là `void *` hoặc generic |
| Nhiều list | Cần nhiều node | Tạo nhiều wrapper |
| Phù hợp RTOS static-first | Rất phù hợp | Ít phù hợp hơn |

---

# 13. List node ownership

Mỗi node cần có ownership rõ.

Ví dụ:

```text
ready_node
    |
    +-- chỉ scheduler được insert/remove

all_task_node
    |
    +-- task registry được quản lý

wait_node
    |
    +-- synchronization subsystem được quản lý
```

Không nên để module bất kỳ tự ý thao tác node.

## Ownership rule

```text
Subsystem sở hữu list
    |
    +-- quyết định insert;
    +-- quyết định remove;
    +-- bảo vệ critical section;
    +-- duy trì invariant.
```

Ví dụ:

```text
Scheduler owns ready queues.
Task registry owns all-task list.
Semaphore owns its wait list.
Timer service owns timer list.
```

---

# 14. List membership và duplicate insertion

Một node intrusive chỉ được thuộc một list tại một thời điểm.

Sai:

```c
list_push_back(&ready_queue_1, &task->ready_node);
list_push_back(&ready_queue_2, &task->ready_node);
```

Hậu quả:

- pointer của node bị ghi đè;
- list 1 bị hỏng;
- task xuất hiện ở hai priority;
- scheduler có thể chọn sai task.

## Debug membership

Có thể thêm:

```c
typedef struct list_node
{
    struct list_node *previous;
    struct list_node *next;

#if HR_CONFIG_LIST_DEBUG
    const void *owner;
#endif
} list_node_t;
```

Khi insert:

```c
HR_ASSERT(node->owner == NULL);
node->owner = list;
```

Khi remove:

```c
HR_ASSERT(node->owner == list);
node->owner = NULL;
```

Release build có thể bỏ field này nếu cần tiết kiệm RAM.

---

# 15. List invariants

Một doubly linked list tuyến tính cần:

- empty list:
  - `head == NULL`;
  - `tail == NULL`;
  - `count == 0`.
- non-empty list:
  - `head != NULL`;
  - `tail != NULL`;
  - `head->previous == NULL`;
  - `tail->next == NULL`.
- mỗi node:
  - `node->next->previous == node` nếu `next != NULL`;
  - `node->previous->next == node` nếu `previous != NULL`.
- số node duyệt được bằng `count`.
- không có cycle ngoài dự kiến.
- node owner đúng list.
- một node không xuất hiện hai lần.

## Validator

Validator nên chỉ dùng trong:

- host tests;
- debug build;
- diagnostic command.

Không nên chạy full O(n) validator trong mọi scheduler operation của release build.

---

# 16. API cho intrusive doubly linked list

Public API gợi ý:

```c
typedef struct hr_list_node
{
    struct hr_list_node *previous;
    struct hr_list_node *next;

#if HR_CONFIG_LIST_DEBUG
    const void *owner;
#endif
} hr_list_node_t;

typedef struct
{
    hr_list_node_t *head;
    hr_list_node_t *tail;
    size_t count;
} hr_list_t;

void hr_list_init(hr_list_t *list);
void hr_list_node_init(hr_list_node_t *node);

bool hr_list_is_empty(const hr_list_t *list);
size_t hr_list_count(const hr_list_t *list);

hr_list_node_t *hr_list_front(const hr_list_t *list);
hr_list_node_t *hr_list_back(const hr_list_t *list);

bool hr_list_push_front(hr_list_t *list,
                        hr_list_node_t *node);

bool hr_list_push_back(hr_list_t *list,
                       hr_list_node_t *node);

hr_list_node_t *hr_list_pop_front(hr_list_t *list);
hr_list_node_t *hr_list_pop_back(hr_list_t *list);

bool hr_list_insert_before(hr_list_t *list,
                           hr_list_node_t *position,
                           hr_list_node_t *node);

bool hr_list_insert_after(hr_list_t *list,
                          hr_list_node_t *position,
                          hr_list_node_t *node);

bool hr_list_remove(hr_list_t *list,
                    hr_list_node_t *node);

bool hr_list_validate(const hr_list_t *list);
```

## API design principles

- Không cấp phát động.
- Không dùng `void *object` trong list core.
- Không gọi callback.
- Return status rõ ràng.
- Node init riêng.
- Không âm thầm remove node từ list khác.
- Không âm thầm insert duplicate node.

---

# 17. Mã khung intrusive list

## 17.1 Init list

```c
void hr_list_init(hr_list_t *list)
{
    HR_ASSERT(list != NULL);

    list->head = NULL;
    list->tail = NULL;
    list->count = 0U;
}
```

## 17.2 Init node

```c
void hr_list_node_init(hr_list_node_t *node)
{
    HR_ASSERT(node != NULL);

    node->previous = NULL;
    node->next = NULL;

#if HR_CONFIG_LIST_DEBUG
    node->owner = NULL;
#endif
}
```

## 17.3 Push back

```c
bool hr_list_push_back(hr_list_t *list,
                       hr_list_node_t *node)
{
    if ((list == NULL) || (node == NULL))
    {
        return false;
    }

#if HR_CONFIG_LIST_DEBUG
    if (node->owner != NULL)
    {
        return false;
    }
#endif

    node->next = NULL;
    node->previous = list->tail;

    if (list->tail != NULL)
    {
        list->tail->next = node;
    }
    else
    {
        list->head = node;
    }

    list->tail = node;
    list->count++;

#if HR_CONFIG_LIST_DEBUG
    node->owner = list;
#endif

    return true;
}
```

## 17.4 Remove

```c
bool hr_list_remove(hr_list_t *list,
                    hr_list_node_t *node)
{
    if ((list == NULL) || (node == NULL))
    {
        return false;
    }

#if HR_CONFIG_LIST_DEBUG
    if (node->owner != list)
    {
        return false;
    }
#endif

    if (node->previous != NULL)
    {
        node->previous->next = node->next;
    }
    else
    {
        list->head = node->next;
    }

    if (node->next != NULL)
    {
        node->next->previous = node->previous;
    }
    else
    {
        list->tail = node->previous;
    }

    node->previous = NULL;
    node->next = NULL;

#if HR_CONFIG_LIST_DEBUG
    node->owner = NULL;
#endif

    HR_ASSERT(list->count > 0U);
    list->count--;

    return true;
}
```

## 17.5 Pop front

```c
hr_list_node_t *hr_list_pop_front(hr_list_t *list)
{
    hr_list_node_t *node;

    if ((list == NULL) || (list->head == NULL))
    {
        return NULL;
    }

    node = list->head;

    if (!hr_list_remove(list, node))
    {
        return NULL;
    }

    return node;
}
```

---

# 18. Container-of

Intrusive list trả về node.

Kernel cần lấy object chứa node.

Macro:

```c
#define HR_CONTAINER_OF(pointer, type, member) \
    ((type *)((uint8_t *)(pointer) - offsetof(type, member)))
```

Ví dụ:

```c
hr_list_node_t *node = hr_list_front(&all_tasks);

hr_task_t *task =
    HR_CONTAINER_OF(node,
                    hr_task_t,
                    all_task_node);
```

## Điều kiện sử dụng đúng

- `pointer` thật sự trỏ đúng member.
- `type` đúng object type.
- `member` đúng field.
- object vẫn còn lifetime hợp lệ.
- không áp dụng lên sentinel node không thuộc object.

## Static assertion

Có thể kiểm tra layout quan trọng:

```c
_Static_assert(offsetof(hr_task_t, saved_sp) == 0U,
               "saved_sp must be at offset zero");
```

---

# 19. Kiểm thử linked list trên host

Host test giúp tách lỗi list khỏi:

- interrupt;
- scheduler;
- assembly;
- target memory;
- UART.

Compiler flags:

```bash
-std=c11
-Wall
-Wextra
-Werror
-pedantic
-fsanitize=address,undefined
-fno-omit-frame-pointer
```

## Test tối thiểu

### Init

- init list;
- init node;
- empty state;
- count zero.

### Push front/back

- insert một node;
- insert nhiều node;
- head/tail đúng;
- order đúng.

### Pop front/back

- empty list;
- one-node list;
- multiple nodes.

### Remove

- remove head;
- remove tail;
- remove middle;
- remove only node;
- remove node không thuộc list.

### Duplicate insertion

- insert cùng node hai lần;
- insert node từ list A vào list B khi chưa remove.

### Validator

- sửa `next`;
- sửa `previous`;
- sửa count;
- tạo cycle;
- owner sai.

### Randomized test

Random operation:

```text
push front
push back
remove
pop front
pop back
```

Sau mỗi operation:

```c
assert(hr_list_validate(&list));
```

---

# 20. Task trong RTOS là gì?

Task là một luồng thực thi độc lập về mặt logic.

Task gồm:

```text
Task code
+
Task argument
+
Task stack
+
Task Control Block
```

Ví dụ:

```c
static void sensor_task(void *argument)
{
    sensor_config_t *config = argument;

    for (;;)
    {
        sensor_sample(config);
        hr_task_delay(10U);
    }
}
```

Trong Chủ đề 3, trọng tâm không phải delay mà là:

- task object được lưu thế nào;
- task stack được quản lý thế nào;
- task nằm trong list nào;
- scheduler tìm task thế nào;
- diagnostic đọc task thế nào.

---

# 21. Task Control Block là gì?

TCB là cấu trúc dữ liệu đại diện cho một task trong kernel.

```text
Application task code
        |
        v
TCB stores kernel-visible metadata
        |
        v
Scheduler and architecture port use TCB
```

TCB không chứa toàn bộ stack data.

TCB chứa pointer và metadata liên quan stack.

Ví dụ:

```text
TCB
+-- saved_sp
+-- stack_low
+-- stack_high
+-- entry
+-- argument
+-- state
+-- priority
+-- list nodes
+-- name
+-- runtime counters
```

---

# 22. TCB lưu những gì?

Một TCB HairRTOS có thể chứa:

```c
typedef struct hr_task
{
    uint32_t *saved_sp;

    uint32_t *stack_low;
    uint32_t *stack_high;
    size_t stack_word_count;

    hr_task_entry_t entry;
    void *argument;

    const char *name;

    hr_task_state_t state;
    uint8_t base_priority;
    uint8_t effective_priority;

    uint32_t wake_tick;
    uint32_t time_slice_remaining;

    hr_list_node_t ready_node;
    hr_list_node_t wait_node;
    hr_list_node_t timeout_node;
    hr_list_node_t all_task_node;

    uint32_t runtime_counter;
    size_t stack_high_water_words;

    uint32_t magic;
} hr_task_t;
```

Không phải field nào cũng phải được dùng ngay ở Chủ đề 3.

## Phân nhóm

### Architecture context

```text
saved_sp
stack bounds
```

### Scheduler

```text
state
priority
ready node
time slice
```

### Blocking and timeout

```text
wait node
timeout node
wake tick
```

### Registry and diagnostics

```text
all-task node
name
runtime counter
stack high-water
magic
```

---

# 23. Saved stack pointer và contract với assembly

Trong Cortex-M3 port:

```text
PendSV
    |
    +-- đọc current TCB;
    +-- lưu PSP vào current->saved_sp;
    +-- đọc next->saved_sp;
    +-- ghi PSP.
```

Assembly thường giả định:

```text
saved_sp ở offset 0
```

Ví dụ:

```asm
ldr r2, [r1]
str r0, [r2]
```

Nếu TCB layout đổi:

```c
typedef struct
{
    const char *name;
    uint32_t *saved_sp;
} hr_task_t;
```

assembly cũ sẽ ghi PSP vào `name`.

Hậu quả:

- task name bị hỏng;
- saved_sp không cập nhật;
- next switch HardFault;
- stack corruption.

## Contract bắt buộc

```c
_Static_assert(offsetof(hr_task_t, saved_sp) == 0U,
               "TCB saved_sp offset changed");
```

Có thể export offset sang assembly qua generated header hoặc dùng constant được kiểm tra.

---

# 24. Thông tin stack trong TCB

TCB cần biết:

```text
stack_low
stack_high
saved_sp
stack size
guard location
```

Ví dụ stack tăng xuống:

```text
High address
+-----------------------+
| stack_high            |
| initial frame         |
| free pattern          |
| used stack            |
| guard                 |
+-----------------------+
Low address = stack_low
```

## Kiểm tra range

```c
bool hr_task_saved_sp_is_valid(const hr_task_t *task)
{
    uintptr_t sp;
    uintptr_t low;
    uintptr_t high;

    if (task == NULL)
    {
        return false;
    }

    sp = (uintptr_t)task->saved_sp;
    low = (uintptr_t)task->stack_low;
    high = (uintptr_t)task->stack_high;

    return (sp >= low) &&
           (sp < high) &&
           ((sp & 0x7U) == 0U);
}
```

---

# 25. Priority trong TCB

HairRTOS dùng:

```text
Số nhỏ hơn = priority cao hơn
```

TCB có thể lưu:

```c
uint8_t base_priority;
uint8_t effective_priority;
```

## Base priority

Priority do application cấu hình.

## Effective priority

Priority kernel đang dùng để scheduling.

Ở Chủ đề 3:

```text
effective_priority = base_priority
```

Sau này mutex priority inheritance có thể thay đổi effective priority tạm thời.

## Validation

```c
if (priority >= HR_PRIORITY_COUNT)
{
    return HR_STATUS_INVALID_PRIORITY;
}
```

---

# 26. Task state tối thiểu trong Chủ đề 3

Chủ đề task state đầy đủ thuộc Chủ đề 4.

Tuy nhiên, Chủ đề 3 cần enum tối thiểu để tổ chức object:

```c
typedef enum
{
    HR_TASK_STATE_UNUSED = 0,
    HR_TASK_STATE_CREATED,
    HR_TASK_STATE_READY,
    HR_TASK_STATE_RUNNING
} hr_task_state_t;
```

Có thể khai báo đầy đủ enum ngay từ đầu:

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

Nhưng trong Chủ đề 3 chỉ kiểm thử:

```text
UNUSED -> CREATED -> READY
```

và organization data.

Không triển khai đầy đủ blocking/suspend transition tại đây.

---

# 27. Task entry và argument

Task entry type:

```c
typedef void (*hr_task_entry_t)(void *argument);
```

Task creation lưu:

```c
task->entry = entry;
task->argument = argument;
```

Architecture port đặt:

```text
PC = entry
R0 = argument
LR = task-return handler
```

## Task entry không được NULL

```c
if (entry == NULL)
{
    return HR_STATUS_INVALID_ARGUMENT;
}
```

## Task không nên return

```c
static void worker_task(void *argument)
{
    for (;;)
    {
    }
}
```

Nếu return:

```text
task-return handler -> panic
```

---

# 28. Static task creation

HairRTOS static-first.

Application cung cấp:

```text
TCB storage
Task stack storage
Task entry
Task argument
Priority
Name
```

Ví dụ:

```c
static hr_task_t g_worker_task;
static uint32_t g_worker_stack[128];
```

Creation API:

```c
hr_status_t hr_task_create_static(
    hr_task_t *task,
    const char *name,
    hr_task_entry_t entry,
    void *argument,
    uint8_t priority,
    uint32_t *stack_memory,
    size_t stack_word_count);
```

## Không allocation ngầm

Không nên:

```c
hr_task_t *task = hr_task_create(...);
```

nếu bên trong gọi heap mà policy không nói rõ.

---

# 29. Task stack initialization

Creation sequence:

```text
Validate arguments
        |
        v
Initialize TCB
        |
        v
Fill stack pattern
        |
        v
Set guard word
        |
        v
Build initial architecture frame
        |
        v
Save returned SP in TCB
        |
        v
Initialize list nodes
        |
        v
Set state CREATED
        |
        v
Register in all-task list
```

Architecture-specific frame nên nằm trong port:

```c
task->saved_sp =
    hr_port_build_initial_stack(
        stack_memory,
        stack_word_count,
        entry,
        argument);
```

Task module không nên tự hard-code toàn bộ Cortex-M register frame nếu muốn portability.

---

# 30. Task registry và all-task list

All-task list chứa mọi task đã tạo hợp lệ.

```text
all-task list
    |
    +-- Idle task
    +-- Sensor task
    +-- Control task
    +-- Logger task
```

Mục đích:

- diagnostics;
- task lookup theo name;
- stack checking;
- runtime statistics;
- debug;
- iteration khi shutdown hoặc validation.

## Không dùng all-task list để scheduling

Scheduler nên dùng ready queues.

All-task list có thể chứa:

- READY task;
- RUNNING task;
- BLOCKED task;
- SUSPENDED task.

---

# 31. Ready lists trong kernel

Fixed-priority scheduler có một ready queue cho mỗi priority.

```text
ready[0] -> High task
ready[1] -> Worker A <-> Worker B
ready[2] -> Logger
ready[3] -> Idle
```

Mỗi task READY nằm đúng một ready queue.

## Queue policy

- insert tail;
- select head;
- rotate head về tail khi yield/time slice;
- clear bitmap khi queue empty;
- set bitmap khi queue từ empty thành non-empty.

## Ready node

TCB chứa:

```c
hr_list_node_t ready_node;
```

Không dùng `all_task_node` thay cho `ready_node`.

---

# 32. Một TCB có thể nằm trong nhiều list không?

Có, nhưng mỗi membership cần một node riêng.

Ví dụ task READY:

```text
all-task list
    |
    +-- task.all_task_node

ready queue
    |
    +-- task.ready_node
```

Cùng một TCB nằm đồng thời trong hai list thông qua hai node khác nhau.

Không được dùng cùng node cho cả hai list.

Sai:

```c
hr_list_push_back(&all_tasks,
                  &task->ready_node);

hr_list_push_back(&ready_queue,
                  &task->ready_node);
```

Đúng:

```c
hr_list_push_back(&all_tasks,
                  &task->all_task_node);

hr_list_push_back(&ready_queue,
                  &task->ready_node);
```

---

# 33. Tổ chức nhiều list node trong một TCB

TCB mở rộng:

```c
typedef struct hr_task
{
    uint32_t *saved_sp;

    hr_list_node_t ready_node;
    hr_list_node_t all_task_node;
    hr_list_node_t wait_node;
    hr_list_node_t timeout_node;

    ...
} hr_task_t;
```

## Node responsibility

| Node | List |
|---|---|
| `ready_node` | Ready queue |
| `all_task_node` | All-task registry |
| `wait_node` | Semaphore/mutex/queue wait list |
| `timeout_node` | Delayed/timeout list |

Trong Chủ đề 3 chỉ cần triển khai:

- `ready_node`;
- `all_task_node`.

Các node khác có thể khai báo sau khi bắt đầu Chủ đề 4.

---

# 34. Current task và next task

Global scheduler state:

```c
static hr_task_t *g_current_task;
```

Scheduler chọn:

```c
hr_task_t *next =
    hr_scheduler_select_next();
```

Context switch:

```text
current task saved_sp <- PSP
g_current_task        <- next
PSP                   <- next saved_sp
```

## Current task invariant

Khi kernel RUNNING:

- `g_current_task != NULL`;
- current task state là RUNNING;
- current task là head của ready queue priority tương ứng;
- current task thuộc all-task list;
- saved_sp nằm trong stack range.

---

# 35. Task lifecycle ở mức dữ liệu

Chủ đề 3 chỉ xử lý organization cơ bản:

```text
UNUSED
  |
  | create
  v
CREATED
  |
  | make ready
  v
READY
  |
  | scheduler selects
  v
RUNNING
```

Data operation:

## Create

- init TCB;
- init stack;
- init nodes;
- insert all-task list.

## Make ready

- insert ready queue;
- state READY;
- update bitmap.

## Run

- scheduler chọn;
- state RUNNING;
- current pointer update.

Blocking và suspended transition để Chủ đề 4 xử lý.

---

# 36. Task naming và diagnostics

Task name giúp:

- GDB;
- UART report;
- panic log;
- stack report;
- trace.

API:

```c
const char *hr_task_name(const hr_task_t *task);
```

## Name lifetime

Nếu TCB chỉ lưu pointer:

```c
task->name = name;
```

thì name phải tồn tại suốt lifetime task.

Tốt:

```c
static const char worker_name[] = "worker";
```

Không tốt:

```c
void create(void)
{
    char name[16] = "worker";
    hr_task_create_static(..., name, ...);
}
```

Sau khi `create()` return, pointer không còn hợp lệ.

Có thể chọn:

- lưu pointer tới string static;
- copy vào fixed-size field trong TCB.

---

# 37. Stack guard và high-water mark

## Stack fill pattern

Khi create:

```c
for (size_t i = 0U; i < stack_word_count; ++i)
{
    stack_memory[i] = 0xA5A5A5A5UL;
}
```

Sau đó architecture frame ghi ở đầu stack theo growth direction.

## Guard word

```c
stack_memory[0] = 0xDEADBEEFUL;
```

Guard được kiểm tra:

```c
bool hr_task_stack_guard_ok(const hr_task_t *task);
```

## High-water mark

Quét pattern chưa bị ghi.

```text
stack size
-
unused pattern words
=
maximum observed usage
```

Không nên quét trong PendSV.

Có thể quét:

- diagnostic command;
- monitor task;
- debug build;
- stress-test checkpoint.

---

# 38. Kernel assertions

Assertions cho TCB và list:

```c
HR_ASSERT(task != NULL);
HR_ASSERT(task->magic == HR_TASK_MAGIC);
HR_ASSERT(task->state != HR_TASK_STATE_UNUSED);
HR_ASSERT(task->effective_priority < HR_PRIORITY_COUNT);
HR_ASSERT(hr_task_saved_sp_is_valid(task));
HR_ASSERT(hr_list_validate(&g_all_tasks));
```

Assertions nên:

- không gọi heap;
- không giữ critical section lâu;
- đi tới panic handler;
- có file/line nếu debug build;
- có cách tắt hoặc giảm trong release build.

---

# 39. Error handling trong task creation

Status enum:

```c
typedef enum
{
    HR_STATUS_OK = 0,
    HR_STATUS_INVALID_ARGUMENT,
    HR_STATUS_INVALID_PRIORITY,
    HR_STATUS_STACK_TOO_SMALL,
    HR_STATUS_STACK_ALIGNMENT_ERROR,
    HR_STATUS_TASK_ALREADY_CREATED,
    HR_STATUS_LIST_ERROR,
    HR_STATUS_PORT_ERROR
} hr_status_t;
```

## Input validation

- task pointer không NULL;
- stack pointer không NULL;
- entry không NULL;
- stack size tối thiểu;
- stack alignment;
- priority hợp lệ;
- TCB chưa được tạo;
- name hợp lệ theo policy;
- initial frame build thành công.

## Rollback

Nếu creation fail sau khi đã init một phần:

- không để task trong all-task list;
- không để node owner trỏ list;
- không để state READY;
- trả status rõ ràng;
- giữ TCB ở trạng thái có thể init lại.

---

# 40. Mã khung TCB và task API

```c
#ifndef HR_TASK_H
#define HR_TASK_H

#include <stddef.h>
#include <stdint.h>

#include "hr_list.h"

#define HR_TASK_MAGIC (0x5441534BUL)

typedef void (*hr_task_entry_t)(void *argument);

typedef enum
{
    HR_TASK_STATE_UNUSED = 0,
    HR_TASK_STATE_CREATED,
    HR_TASK_STATE_READY,
    HR_TASK_STATE_RUNNING,
    HR_TASK_STATE_BLOCKED,
    HR_TASK_STATE_SUSPENDED
} hr_task_state_t;

typedef struct hr_task
{
    uint32_t *saved_sp;

    uint32_t *stack_low;
    uint32_t *stack_high;
    size_t stack_word_count;

    hr_task_entry_t entry;
    void *argument;
    const char *name;

    hr_task_state_t state;
    uint8_t base_priority;
    uint8_t effective_priority;

    hr_list_node_t ready_node;
    hr_list_node_t all_task_node;

    uint32_t runtime_counter;
    uint32_t magic;
} hr_task_t;

_Static_assert(offsetof(hr_task_t, saved_sp) == 0U,
               "saved_sp must remain at offset zero");

hr_status_t hr_task_create_static(
    hr_task_t *task,
    const char *name,
    hr_task_entry_t entry,
    void *argument,
    uint8_t priority,
    uint32_t *stack_memory,
    size_t stack_word_count);

const char *hr_task_name(const hr_task_t *task);

bool hr_task_is_valid(const hr_task_t *task);
bool hr_task_stack_guard_ok(const hr_task_t *task);
size_t hr_task_stack_high_water_words(
    const hr_task_t *task);

#endif
```

---

# 41. Mã khung static task creation

```c
hr_status_t hr_task_create_static(
    hr_task_t *task,
    const char *name,
    hr_task_entry_t entry,
    void *argument,
    uint8_t priority,
    uint32_t *stack_memory,
    size_t stack_word_count)
{
    uint32_t *initial_sp;

    if ((task == NULL) ||
        (name == NULL) ||
        (entry == NULL) ||
        (stack_memory == NULL))
    {
        return HR_STATUS_INVALID_ARGUMENT;
    }

    if (priority >= HR_PRIORITY_COUNT)
    {
        return HR_STATUS_INVALID_PRIORITY;
    }

    if (stack_word_count < HR_MIN_STACK_WORDS)
    {
        return HR_STATUS_STACK_TOO_SMALL;
    }

    if (((uintptr_t)stack_memory & 0x7U) != 0U)
    {
        return HR_STATUS_STACK_ALIGNMENT_ERROR;
    }

    if (task->magic == HR_TASK_MAGIC)
    {
        return HR_STATUS_TASK_ALREADY_CREATED;
    }

    memset(task, 0, sizeof(*task));

    hr_list_node_init(&task->ready_node);
    hr_list_node_init(&task->all_task_node);

    hr_stack_fill_pattern(stack_memory,
                          stack_word_count);

    hr_stack_write_guard(stack_memory,
                         stack_word_count);

    initial_sp =
        hr_port_build_initial_stack(
            stack_memory,
            stack_word_count,
            entry,
            argument);

    if (initial_sp == NULL)
    {
        return HR_STATUS_PORT_ERROR;
    }

    task->saved_sp = initial_sp;
    task->stack_low = stack_memory;
    task->stack_high =
        stack_memory + stack_word_count;
    task->stack_word_count = stack_word_count;

    task->entry = entry;
    task->argument = argument;
    task->name = name;

    task->state = HR_TASK_STATE_CREATED;
    task->base_priority = priority;
    task->effective_priority = priority;

    task->runtime_counter = 0U;
    task->magic = HR_TASK_MAGIC;

    if (!hr_task_registry_add(task))
    {
        task->magic = 0U;
        task->state = HR_TASK_STATE_UNUSED;
        return HR_STATUS_LIST_ERROR;
    }

    return HR_STATUS_OK;
}
```

Ghi chú:

- `memset()` cần runtime implementation phù hợp bare-metal.
- Stack fill không được ghi đè initial frame sau khi build.
- Trình tự guard và frame phải nhất quán với stack growth.
- Rollback cần hoàn thiện trong code production.

---

# 42. Mã khung task registry

```c
static hr_list_t g_all_tasks;

void hr_task_registry_init(void)
{
    hr_list_init(&g_all_tasks);
}

bool hr_task_registry_add(hr_task_t *task)
{
    if (!hr_task_is_valid(task))
    {
        return false;
    }

    return hr_list_push_back(
        &g_all_tasks,
        &task->all_task_node);
}

size_t hr_task_registry_count(void)
{
    return hr_list_count(&g_all_tasks);
}
```

## Iterate

```c
void hr_task_registry_dump(void)
{
    hr_list_node_t *node;

    node = hr_list_front(&g_all_tasks);

    while (node != NULL)
    {
        hr_task_t *task =
            HR_CONTAINER_OF(node,
                            hr_task_t,
                            all_task_node);

        hr_task_report(task);

        node = node->next;
    }
}
```

Không nên in UART khi đang giữ scheduler critical section.

---

# 43. Mã khung ready queue

```c
typedef struct
{
    hr_list_t tasks;
} hr_ready_queue_t;

static hr_ready_queue_t
    g_ready_queues[HR_PRIORITY_COUNT];

static uint32_t g_ready_bitmap;
```

## Init

```c
void hr_ready_queues_init(void)
{
    for (size_t i = 0U;
         i < HR_PRIORITY_COUNT;
         ++i)
    {
        hr_list_init(&g_ready_queues[i].tasks);
    }

    g_ready_bitmap = 0U;
}
```

## Insert ready

```c
bool hr_ready_insert(hr_task_t *task)
{
    uint8_t priority;

    if (!hr_task_is_valid(task))
    {
        return false;
    }

    priority = task->effective_priority;

    if (priority >= HR_PRIORITY_COUNT)
    {
        return false;
    }

    if (!hr_list_push_back(
            &g_ready_queues[priority].tasks,
            &task->ready_node))
    {
        return false;
    }

    g_ready_bitmap |= (1UL << priority);
    task->state = HR_TASK_STATE_READY;

    return true;
}
```

## Remove ready

```c
bool hr_ready_remove(hr_task_t *task)
{
    uint8_t priority;
    hr_list_t *list;

    if (!hr_task_is_valid(task))
    {
        return false;
    }

    priority = task->effective_priority;
    list = &g_ready_queues[priority].tasks;

    if (!hr_list_remove(list,
                        &task->ready_node))
    {
        return false;
    }

    if (hr_list_is_empty(list))
    {
        g_ready_bitmap &=
            ~(1UL << priority);
    }

    return true;
}
```

---

# 44. Chiến lược kiểm thử

## 44.1 Host tests

Nên test trên host:

- intrusive list;
- container-of;
- list ownership;
- task input validation;
- TCB initialization;
- task registry;
- ready queue;
- ready bitmap;
- task lookup;
- stack pattern;
- stack high-water calculation.

## 44.2 Target tests

Nên test trên STM32:

- initial frame thật;
- saved_sp range;
- TCB offset contract;
- scheduler sử dụng ready queue;
- task context switch;
- stack guard;
- UART diagnostic.

## 44.3 Separation

```text
Host tests
    |
    +-- list logic
    +-- TCB metadata
    +-- registry
    +-- queue policy

Target tests
    |
    +-- PSP
    +-- SVC
    +-- PendSV
    +-- real stack
```

## 44.4 Sanitizers

Host tests:

```bash
-fsanitize=address,undefined
```

Phát hiện:

- out-of-bounds;
- invalid pointer;
- use-after-scope;
- undefined behavior;
- alignment issues.

## 44.5 Randomized tests

Random task/list operations:

```text
create task
insert ready
remove ready
rotate
iterate registry
validate
```

Sau mỗi operation:

```c
assert(hr_list_validate(&g_all_tasks));
assert(hr_ready_validate_all());
```

---

# 45. Lỗi thường gặp

## 45.1 Dùng cùng một node cho hai list

Làm hỏng cả hai list.

## 45.2 Không init node trước khi insert

Node chứa pointer rác.

## 45.3 Remove node không thuộc list

Head/tail bị sửa sai.

## 45.4 Quên clear owner khi remove

Node không thể insert lại.

## 45.5 Count không cập nhật

Diagnostic và invariant sai.

## 45.6 Duyệt list trong khi remove current node sai cách

Sai:

```c
for (node = list->head;
     node != NULL;
     node = node->next)
{
    hr_list_remove(list, node);
}
```

Sau remove, `node->next` có thể đã bị clear.

Đúng:

```c
next = node->next;
hr_list_remove(list, node);
node = next;
```

## 45.7 TCB `saved_sp` không ở offset 0

PendSV ghi sai field.

## 45.8 Stack pointer không nằm trong stack range

Task restore từ vùng RAM sai.

## 45.9 Task name trỏ biến local

Sau function return, name pointer invalid.

## 45.10 Task create fail nhưng vẫn insert registry

All-task list chứa object không hợp lệ.

## 45.11 Task nằm trong ready queue nhưng state không READY

Scheduler data và state không đồng bộ.

## 45.12 Bitmap và queue không nhất quán

`ctz()` chọn queue rỗng hoặc bỏ qua task READY.

## 45.13 Dùng full list validation trong PendSV

Làm context-switch latency tăng O(n).

## 45.14 In UART trong critical section

Giữ interrupt mask quá lâu.

## 45.15 Không kiểm tra duplicate task creation

Cùng TCB được init lại trong lúc đang nằm trong list.

---

# 46. Bài thực hành

## Bài 01 — Singly linked list

### Mục tiêu

- Cài singly linked list.
- Hiểu head, next và traversal.
- So sánh push front và push back.

### Yêu cầu

- init;
- push front;
- push back;
- pop front;
- find;
- count;
- validator cơ bản.

### Test

- empty;
- one node;
- multiple nodes;
- order;
- duplicate insertion policy.

### Fault injection

Tạo cycle:

```text
C.next = B
```

Quan sát validator hoặc traversal limit phát hiện lỗi.

### Tiêu chí hoàn thành

- Host tests pass.
- ASan pass.
- Không malloc.
- Giải thích được O(1) và O(n) operations.

---

## Bài 02 — Intrusive doubly linked list

### Mục tiêu

- Cài intrusive doubly list.
- Remove known node O(1).
- Duyệt hai chiều.

### Yêu cầu

- init list/node;
- push front/back;
- pop front/back;
- insert before/after;
- remove;
- count;
- front/back.

### Fault injection

Bỏ cập nhật:

```c
node->next->previous
```

Chạy backward traversal và validator.

### Tiêu chí hoàn thành

- Forward traversal đúng.
- Backward traversal đúng.
- Head/tail đúng.
- Middle remove đúng.
- Sanitizer pass.

---

## Bài 03 — List invariants and ownership

### Mục tiêu

- Thêm debug owner.
- Detect duplicate insertion.
- Detect remove wrong list.
- Validate list.

### Yêu cầu

- owner field trong debug build;
- list validator;
- cycle detection hoặc traversal bound;
- corruption tests.

### Test

- node list A insert list B;
- wrong count;
- wrong previous;
- wrong tail;
- self-link;
- cycle.

### Tiêu chí hoàn thành

- Mỗi corruption có test.
- Validator không sửa list.
- Release mode có thể bỏ owner field.

---

## Bài 04 — TCB layout and static assertions

### Mục tiêu

- Thiết kế TCB.
- Đảm bảo `saved_sp` offset 0.
- Phân nhóm field.

### Yêu cầu

- `hr_task_t`;
- `hr_task_state_t`;
- priority;
- stack metadata;
- list nodes;
- task magic;
- static assertions.

### Host output

In:

```text
sizeof(hr_task_t)
offsetof(saved_sp)
offsetof(ready_node)
offsetof(all_task_node)
```

### Fault injection

Đưa `name` lên trước `saved_sp`, xác nhận static assertion fail.

### Tiêu chí hoàn thành

- Layout document.
- Static assertions.
- Build 32-bit target assumptions được ghi rõ.
- Không phụ thuộc STM32 trong host layout test nếu không cần.

---

## Bài 05 — Static task creation

### Mục tiêu

- Tạo task bằng TCB và stack do application cấp.
- Validate input.
- Init nodes.
- Init metadata.

### Yêu cầu

- create static API;
- name;
- entry;
- argument;
- priority;
- stack pointer;
- stack size;
- state CREATED;
- task magic.

### Tests

- NULL arguments;
- invalid priority;
- stack too small;
- misaligned stack;
- duplicate creation;
- valid creation.

### Tiêu chí hoàn thành

- Không malloc.
- ASan/UBSan pass.
- Failure không để TCB ở trạng thái nửa hợp lệ.
- Status code rõ ràng.

---

## Bài 06 — Task stack initialization

### Mục tiêu

- Fill pattern.
- Guard word.
- Initial frame.
- Saved SP range.

### Yêu cầu

- stack fill;
- guard;
- build initial frame;
- high-water calculation;
- saved SP validation.

### Fault injection

- Thumb bit 0;
- stack misalignment;
- guard corruption;
- saved SP ngoài range.

### Tiêu chí hoàn thành

- Host tests cho frame.
- Target inspection bằng GDB.
- Guard detection hoạt động.
- High-water report hợp lý.

---

## Bài 07 — All-task registry

### Mục tiêu

- Lưu mọi task trong intrusive all-task list.
- Iterate và lookup.

### Yêu cầu

- registry init;
- add task;
- count;
- find by name;
- iterate;
- dump snapshot.

### Tests

- empty registry;
- one task;
- multiple tasks;
- duplicate add;
- name lookup;
- invalid task.

### Tiêu chí hoàn thành

- Registry không dùng ready node.
- Task order được xác định.
- Diagnostic không sửa list.
- Host tests pass.

---

## Bài 08 — Per-priority ready queues

### Mục tiêu

- Tổ chức ready queue theo priority.
- Quản lý ready bitmap.

### Yêu cầu

- queue array;
- insert tail;
- remove;
- select highest priority;
- rotate equal priority;
- bitmap update.

### Tests

- one task;
- multiple priorities;
- same-priority FIFO;
- remove last task;
- invalid priority;
- duplicate ready insertion.

### Tiêu chí hoàn thành

- Queue/list consistent.
- Bitmap consistent.
- Host tests pass.
- Idle rule được ghi rõ.

---

## Bài 09 — TCB in multiple lists

### Mục tiêu

- Một task đồng thời nằm trong all-task list và ready list.
- Dùng hai intrusive node khác nhau.

### Yêu cầu

- create 4 tasks;
- registry;
- ready queues;
- move task READY/RUNNING ở mức dữ liệu;
- validate all nodes.

### Fault injection

Cố ý dùng `ready_node` cho all-task list và quan sát corruption test.

### Tiêu chí hoàn thành

- Hai list hoạt động độc lập.
- Container-of dùng đúng member.
- Task lookup đúng.
- Ready selection đúng.

---

## Bài 10 — Target task inspector

### Mục tiêu

- Chạy trên STM32.
- Tạo nhiều static task.
- In TCB và list diagnostics qua UART.

### UART commands

```text
help
tasks
ready
stack
validate
```

### Output

```text
Name      State    Base  Effective  SavedSP     Stack
idle      READY    3     3          0x...       OK
worker-a  READY    1     1          0x...       OK
worker-b  READY    1     1          0x...       OK
monitor   RUNNING  2     2          0x...       OK
```

### Tiêu chí hoàn thành

- Registry dump.
- Ready queue dump.
- Stack guard check.
- Saved SP range check.
- Không UART trong PendSV.
- Có target validation report.

---

# 47. Project tổng kết chủ đề

## Tên project

```text
Task Inspector
```

## 47.1 Mục tiêu

Xây dựng firmware độc lập cho STM32F103 để quan sát:

- static task creation;
- TCB layout;
- initial task stack;
- all-task registry;
- ready queues;
- ready bitmap;
- current task;
- task name;
- priority;
- stack guard;
- stack high-water;
- runtime counter;
- list invariants.

## 47.2 Kiến trúc

```text
+---------------------------------------------------+
| Application                                       |
|                                                   |
| Worker A  Worker B  Monitor  Idle                 |
+-------------------------+-------------------------+
                          |
                          v
+---------------------------------------------------+
| Task Management                                   |
|                                                   |
| Static task creation                              |
| TCB validation                                    |
| Stack initialization                              |
| Task registry                                     |
+-------------------------+-------------------------+
                          |
                          v
+---------------------------------------------------+
| Kernel Data Structures                            |
|                                                   |
| Intrusive lists                                   |
| All-task list                                     |
| Ready queues                                      |
| Ready bitmap                                      |
+-------------------------+-------------------------+
                          |
                          v
+---------------------------------------------------+
| Cortex-M3 Port                                    |
|                                                   |
| Initial frame  SVC  PendSV  PSP/MSP               |
+-------------------------+-------------------------+
                          |
                          v
+---------------------------------------------------+
| STM32F103 Platform                                |
|                                                   |
| Startup  Clock  UART  GPIO  SysTick               |
+---------------------------------------------------+
```

## 47.3 Task set

### Worker A

- priority 1;
- local counter;
- static stack.

### Worker B

- priority 1;
- local counter;
- static stack.

### Monitor

- priority 2;
- snapshot task diagnostics;
- xử lý UART command.

### Idle

- lowest priority;
- luôn READY;
- có thể `WFI`.

## 47.4 UART commands

```text
help
tasks
ready
current
stacks
registry
validate
trace
reset
```

## 47.5 Output mẫu

```text
HairRTOS Task Inspector
-----------------------

Task count: 4
Ready bitmap: 0x0000000E
Current: monitor

All tasks:
[0] idle
    state       : READY
    base prio   : 3
    eff prio    : 3
    saved_sp    : 0x2000....
    stack guard : OK
    high water  : ... words

[1] worker-a
    state       : READY
    base prio   : 1
    eff prio    : 1
    saved_sp    : 0x2000....
    stack guard : OK
    high water  : ... words

Ready queues:
P0: empty
P1: worker-a -> worker-b
P2: monitor
P3: idle

Validation:
all-task list : PASS
ready queues  : PASS
TCB magic     : PASS
stack ranges  : PASS
```

## 47.6 Requirements

- Static TCB.
- Static task stack.
- Intrusive doubly list.
- Debug owner optional.
- All-task registry.
- Per-priority ready queue.
- Ready bitmap.
- Duplicate insertion detection.
- Task magic.
- `saved_sp` offset assertion.
- Stack guard.
- High-water calculation.
- UART diagnostic outside PendSV.
- Host tests.
- Target validation.

## 47.7 Không được làm

- malloc trong task creation;
- dùng cùng node cho hai list;
- in UART trong PendSV;
- full list validation mỗi context switch;
- application sửa trực tiếp list pointer;
- application sửa trực tiếp task state;
- task name trỏ local buffer;
- hidden list ownership.

## 47.8 Definition of Done

- Root firmware build không warning.
- Host list tests pass.
- Host TCB tests pass.
- ASan pass.
- UBSan pass.
- All-task registry đúng.
- Ready queue đúng.
- Ready bitmap đúng.
- Task Inspector chạy trên board.
- UART report đúng.
- Stack guards pass.
- README và target report hoàn chỉnh.

---

# 48. Cấu trúc repository đề xuất

```text
03-kernel-data-structures-task/
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
│   ├── cortex_m3_port.h
│   ├── critical_section.h
│   ├── list.h
│   ├── task.h
│   ├── task_registry.h
│   ├── ready_queue.h
│   └── task_inspector.h
│
├── src/
│   ├── main.c
│   ├── runtime.c
│   ├── clock.c
│   ├── gpio.c
│   ├── uart.c
│   ├── systick.c
│   ├── cortex_m3_port.c
│   ├── cortex_m3_portasm.S
│   ├── critical_section.c
│   ├── list.c
│   ├── task.c
│   ├── task_registry.c
│   ├── ready_queue.c
│   └── task_inspector.c
│
├── labs/
│   ├── README.md
│   ├── 01-singly-linked-list/
│   ├── 02-intrusive-doubly-list/
│   ├── 03-list-invariants/
│   ├── 04-tcb-layout/
│   ├── 05-static-task-creation/
│   ├── 06-task-stack-initialization/
│   ├── 07-all-task-registry/
│   ├── 08-priority-ready-queues/
│   ├── 09-tcb-multiple-lists/
│   └── 10-target-task-inspector/
│
├── docs/
│   ├── linked-list-types.md
│   ├── intrusive-list.md
│   ├── list-invariants.md
│   ├── tcb-layout.md
│   ├── static-task-creation.md
│   ├── task-stack.md
│   ├── task-registry.md
│   ├── ready-queues.md
│   └── target-validation.md
│
├── tools/
│   ├── check_structure.py
│   ├── check_format.sh
│   └── run_host_tests.sh
│
└── build/
```

---

# 49. Liên hệ với roadmap HairRTOS

Chủ đề này liên quan trực tiếp:

## Phase 2 — Kernel data structures

- intrusive doubly linked list;
- ready queue;
- ready bitmap;
- host tests;
- sanitizer;
- invariant validation.

## Phase 3 — TCB and static task creation

- TCB layout;
- saved SP;
- stack bounds;
- initial stack;
- static task creation;
- task registry;
- task-return panic;
- stack diagnostics.

## Quan hệ với Phase 4–8

Các phase scheduling/context switch sử dụng:

- ready queues;
- current TCB;
- saved SP;
- task priority;
- task state;
- idle task.

Chủ đề 3 cung cấp data model để các phase này hoạt động ổn định.

---

# 50. Tiêu chí hoàn thành

## Linked list

- Phân biệt singly và doubly list.
- Giải thích circular list.
- Giải thích sentinel.
- Giải thích intrusive list.
- Cài node init.
- Cài list init.
- Push front/back.
- Pop front/back.
- Insert before/after.
- Remove known node.
- Count.
- Forward traversal.
- Backward traversal.
- Duplicate insertion detection.
- Wrong-list removal detection.
- Validator.
- Randomized tests.
- ASan pass.
- UBSan pass.

## TCB

- Thiết kế TCB.
- `saved_sp` offset 0.
- Stack bounds.
- Stack word count.
- Task entry.
- Task argument.
- Name.
- State.
- Base priority.
- Effective priority.
- Ready node.
- All-task node.
- Task magic.
- Static assertions.

## Task creation

- Static TCB.
- Static stack.
- Input validation.
- Stack alignment.
- Stack minimum size.
- Stack fill pattern.
- Guard word.
- Initial frame.
- Saved SP validation.
- Failure rollback.
- Duplicate creation detection.

## Registry and ready queues

- All-task registry.
- Registry count.
- Find by name.
- Iterate.
- Per-priority ready queues.
- Ready bitmap.
- FIFO.
- Rotation.
- Idle rule.
- Multiple-list membership đúng node.
- Invariants pass.

## Target project

- Root firmware build.
- ELF/BIN/HEX/MAP.
- UART Task Inspector.
- Task dump.
- Ready queue dump.
- Current task.
- Stack guard.
- High-water.
- Validation command.
- Không UART trong PendSV.
- Board validation report.

---

# 51. Tổng kết

Chủ đề 3 trả lời hai câu hỏi:

```text
Kernel lưu và tổ chức task như thế nào?
```

và:

```text
Kernel dùng cấu trúc dữ liệu nào để quản lý task hiệu quả?
```

Luồng kiến thức:

```text
Linked-list types
        |
        v
Intrusive nodes
        |
        v
List ownership
        |
        v
List invariants
        |
        v
Host tests
        |
        v
TCB layout
        |
        v
Static task creation
        |
        v
All-task registry
        |
        v
Ready queues
        |
        v
Task Inspector
```

Các nguyên tắc cần nhớ:

1. Kernel list không nên cấp phát động ngầm.
2. Intrusive list phù hợp static-first kernel.
3. Một node chỉ thuộc một list tại một thời điểm.
4. Một TCB có thể nằm trong nhiều list bằng nhiều node khác nhau.
5. List owner phải rõ.
6. Duplicate insertion phải được phát hiện.
7. Remove node sai list không được âm thầm thực hiện.
8. Head, tail, count và links phải nhất quán.
9. List policy nên được test trên host.
10. `saved_sp` là contract giữa TCB và assembly.
11. TCB cần stack bounds và metadata.
12. Static task creation phải validate input.
13. Creation failure không được để object nửa hợp lệ.
14. All-task list dùng cho registry và diagnostics.
15. Ready queue dùng cho scheduler.
16. Không dùng all-task list để chọn task.
17. Không dùng cùng node cho all-task và ready list.
18. Stack guard và high-water giúp debug.
19. Full validator không chạy trong PendSV.
20. Task Inspector là bằng chứng trực quan cho data model của kernel.

Sau chủ đề này, kết quả cần đạt:

```text
Một intrusive list library đã được kiểm thử
+
Một TCB layout rõ ràng
+
Một static task creation API
+
Một all-task registry
+
Một hệ thống ready queues
+
Một Task Inspector chạy trên STM32F103
```

Chủ đề tiếp theo:

```text
Task State + Synchronization
```

sẽ sử dụng trực tiếp:

- TCB;
- ready node;
- wait node;
- timeout node;
- state field;
- effective priority;
- list invariants.

---

## Nguồn chương trình

Phạm vi gốc của README được lấy từ Chủ đề 3 trong tài liệu:

```text
self-develop-rtos.pdf

Cấu trúc dữ liệu trong kernel + Task
    - Tasks trong AK-mOS
    - Task Control Block
    - Các loại linked list
    - Case study danh sách liên kết trong AK-mOS
```

Những phần như intrusive list API, HairRTOS TCB layout, static task creation, host testing, Task Inspector và cấu trúc repository là phần mở rộng thực hành dành cho HairRTOS; chúng không được mô tả chi tiết trong bảng chương trình một trang của AKOS.
