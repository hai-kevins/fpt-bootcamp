# Chủ đề 3 — Cấu trúc dữ liệu trong Kernel và Task
## Intrusive Lists, Ready Structures, Task Control Block và Kernel Invariants

> Tài liệu này giải thích vì sao RTOS kernel cần những cấu trúc dữ liệu có ownership và complexity rõ ràng, đặc biệt là intrusive linked list và Task Control Block. Trọng tâm là quan hệ giữa data structure, task lifetime, scheduler và các invariant mà kernel phải bảo toàn.

---

## 1. Vì sao kernel cần cấu trúc dữ liệu riêng?

Kernel liên tục phải quản lý tập object thay đổi trạng thái:

- task READY;
- task delayed;
- task chờ semaphore/mutex/queue;
- software timer;
- free memory blocks;
- registry/diagnostics.

Các collection này cần operation predictable, không allocation ngầm và hỗ trợ remove object đã biết nhanh chóng.

---

## 2. Array và linked list trong kernel

### Array

Ưu điểm:

- memory locality tốt;
- indexing O(1);
- metadata nhỏ.

Nhược điểm:

- insert/remove giữa mảng cần shift;
- capacity cố định;
- tìm object theo condition vẫn O(n).

### Linked list

Ưu điểm:

- insert/remove node đã biết O(1);
- không cần move element;
- phù hợp object thay đổi membership thường xuyên.

Nhược điểm:

- pointer overhead;
- locality kém hơn;
- corruption pointer nguy hiểm.

Kernel thường dùng cả array và list tùy operation pattern.

---

## 3. Singly linked list

Node chỉ có `next`. Memory overhead thấp.

Phù hợp:

- free list;
- push/pop head;
- queue nếu giữ head + tail.

Remove một node bất kỳ cần biết predecessor hoặc scan từ head.

---

## 4. Doubly linked list

Node có `prev` và `next`. Remove node đã biết O(1) mà không cần predecessor.

Đây là lợi thế lớn cho RTOS khi task cần bị remove khỏi wait/ready/delay list do timeout, cancel hoặc wake-up.

Trade-off là thêm pointer và invariant phức tạp hơn.

---

## 5. Circular list

Trong circular list, tail liên kết lại head hoặc sentinel. Nó thuận tiện round-robin vì next của phần tử cuối tự quay lại đầu.

Nhưng cần convention rõ về empty/sentinel để tránh infinite loop khi corruption.

---

## 6. Sentinel node

Sentinel là node giả đại diện biên list. Doubly linked list với sentinel có thể giảm special case:

```text
sentinel <-> A <-> B <-> sentinel
```

Empty list là sentinel tự trỏ chính nó.

Ưu điểm là insert/remove thống nhất, ít branch đặc biệt.

---

## 7. Intrusive linked list

Trong intrusive list, list node nằm **bên trong object** thay vì allocator tạo wrapper node bên ngoài.

Conceptually:

```text
TCB
├─ task fields
├─ ready_node
├─ wait_node
└─ timeout_node
```

List chỉ nối các node; từ node có thể tìm object chứa nó bằng `container_of`.

### 7.1 Vì sao intrusive list hợp kernel?

- không allocation khi insert;
- node lifetime = object lifetime;
- remove O(1);
- memory footprint deterministic;
- object có thể có nhiều membership role bằng nhiều node khác nhau.

---

## 8. Non-intrusive list

List node chứa pointer tới object. Linh hoạt hơn nếu object cần xuất hiện nhiều list mà không muốn thêm field, nhưng thường cần node storage riêng.

Trong RTOS nhỏ, allocation ngầm cho wrapper node thường không mong muốn.

---

## 9. Ownership của list node

Mỗi node cần có owner rõ: list nào đang chứa nó hoặc nó đang detached.

Một node không được đồng thời nằm trong hai linked lists nếu chỉ có một cặp `prev/next`.

Đây là lý do TCB cần **nhiều node** nếu task có thể đồng thời nằm trong object wait list và timeout list.

---

## 10. List invariant

Với doubly linked list:

- `node.next.prev == node`;
- `node.prev.next == node`;
- head/tail/sentinel consistent;
- size đúng số node thực;
- không cycle bất thường ngoài circular convention;
- detached node không giả vờ thuộc list.

Kernel list corruption thường lan sang scheduler và gây crash rất xa điểm insert/remove sai.

---

## 11. Duplicate insertion

Nếu cùng node được insert hai lần mà không remove, pointers có thể tạo cycle hoặc size sai.

Debug kernel nên có optional membership flag/list owner để detect duplicate insertion sớm.

Trong release build có thể bỏ metadata nếu cần tối ưu footprint, nhưng invariant logic vẫn tồn tại.

---

## 12. `container_of`

`container_of` là kỹ thuật lấy địa chỉ object từ địa chỉ một member bằng offset của member trong type.

Về lý thuyết:

```text
object_address = member_address − offset_of(member)
```

Kỹ thuật này là nền của intrusive data structures trong Linux kernel và nhiều RTOS.

Nó chỉ hợp lệ khi pointer thực sự trỏ đúng member của object đúng type.

---

## 13. Task là gì trong RTOS?

Task là execution context độc lập về logical flow, có stack và scheduling state riêng.

Kernel nhìn task không phải như function, mà như một object có lifecycle và context:

```text
Task = CPU context + stack + scheduling metadata + wait metadata
```

Function entry chỉ là điểm bắt đầu của task.

---

## 14. Task Control Block

TCB là object trung tâm đại diện task trong kernel.

Các nhóm field:

### 14.1 Architecture context

- saved stack pointer;
- optional privilege/MPU context;
- architecture-specific flags.

### 14.2 Scheduler metadata

- priority;
- state;
- ready node;
- time-slice data.

### 14.3 Blocking metadata

- wait object;
- wait result;
- timeout/deadline;
- wait node;
- timeout node.

### 14.4 Diagnostics

- name/ID;
- stack bounds;
- high-watermark;
- runtime statistics.

TCB layout là một contract giữa C kernel logic và architecture assembly port.

---

## 15. Saved stack pointer

`saved_sp` phải trỏ tới vị trí context frame mà PendSV restore logic mong đợi.

Nếu assembly truy cập field theo offset cố định, thay đổi TCB layout có thể phá port. Có thể tránh coupling bằng generated offsets hoặc static assertions.

---

## 16. Task stack metadata

TCB thường cần:

- stack base;
- stack end/limit;
- stack size;
- saved SP.

Từ đó kernel có thể validate SP range và tính high-watermark.

Stack direction phụ thuộc architecture; Cortex-M thường dùng descending stack.

---

## 17. Priority: base và effective

Ở giai đoạn scheduler đơn giản có thể chỉ có một priority. Nhưng để hỗ trợ priority inheritance sau này nên phân biệt:

- **base priority**: priority cấu hình của task;
- **effective priority**: priority scheduler đang dùng sau inheritance/boost.

Nếu data model không chuẩn bị, mutex PI về sau sẽ khó thêm đúng.

---

## 18. Task state

Tối thiểu:

- READY;
- RUNNING;
- BLOCKED;
- SUSPENDED.

Có thể thêm CREATED/TERMINATED theo lifecycle policy.

State field không đủ để xác định membership; kernel invariant phải đồng bộ **state + list placement**.

---

## 19. Task entry và argument

Task entry tuân ABI như function thread entry. Argument thường được đặt vào initial R0 trong fake exception frame.

Task thường không nên return. Nếu return, cần trap/cleanup policy rõ vì LR initial phải dẫn tới một handler hợp lệ.

---

## 20. Static task creation

Static creation nghĩa là TCB và stack storage do caller/application cung cấp hoặc reserve compile-time.

Lợi ích:

- không heap allocation;
- footprint biết trước;
- create failure chủ yếu do parameter/invariant, không do heap fragmentation.

Kernel vẫn phải initialize toàn bộ metadata về trạng thái detached sạch.

---

## 21. Dynamic task creation

Dynamic create cần allocate cả TCB và stack hoặc một combined block. Failure giữa hai allocation cần rollback atomic để không leak.

Deletion càng phức tạp vì task có thể đang nằm trong wait/timer/object list. Vì vậy nhiều kernel nhỏ cố định task set để đơn giản lifetime.

---

## 22. Task registry

Registry chứa mọi task tồn tại để diagnostics/enumeration.

Registry khác ready list:

- registry trả lời “task nào tồn tại?”;
- ready list trả lời “task nào có thể chạy?”.

Không nên dùng all-task list để scheduling nếu có ready structure chuyên dụng.

---

## 23. Ready lists

Một ready list per priority thường chứa task READY cùng priority theo FIFO.

Nếu current RUNNING task được xem như vẫn nằm head của ready list hay remove riêng là design choice. Dù chọn cách nào, invariant phải nhất quán với context-switch và round-robin logic.

---

## 24. Delayed list

Task delay cần một collection sorted theo wake time hoặc cấu trúc timer khác.

Task delayed không READY, vì vậy nó không nằm ready list. Khi tick đạt deadline, task được chuyển từ delayed list sang ready list atomically.

---

## 25. Wait list

Synchronization object có wait list các task blocked chờ resource/event.

Ordering có thể theo:

- priority;
- FIFO;
- priority rồi FIFO.

Choice ảnh hưởng predictability và starvation.

---

## 26. Một task có thể ở nhiều list cùng lúc không?

Có, nếu mỗi list biểu diễn một dimension khác và TCB có **node riêng**.

Ví dụ finite-time semaphore wait:

```text
Task T
├─ wait_node    ∈ semaphore.waiters
└─ timeout_node ∈ timeout/delayed list
```

Task logic đang BLOCKED trên semaphore, đồng thời kernel cần theo dõi timeout deadline.

Dùng cùng node cho cả hai list là sai cấu trúc.

---

## 27. Membership matrix

Có thể reasoning bằng matrix:

| Task state | Ready list | Wait list | Timeout list |
|---|---:|---:|---:|
| READY | yes | no | no |
| RUNNING | policy-dependent | no | no |
| BLOCKED forever | no | yes | no |
| BLOCKED finite | no | yes | yes |
| DELAYED | no | no | yes |
| SUSPENDED | no | no* | no* |

Dấu `*` phụ thuộc policy suspend blocked task, nhưng kernel phải chọn rõ.

---

## 28. Transition là transaction nhiều cấu trúc

Ví dụ READY → BLOCKED không chỉ là đổi enum:

1. remove ready node;
2. set wait object/result;
3. insert wait node;
4. nếu finite timeout, insert timeout node;
5. update state;
6. reschedule.

Nếu interrupt chen giữa các bước, kernel có thể quan sát trạng thái không hợp lệ. Do đó transition phải nằm trong critical section hoặc có serialization khác.

---

## 29. Current task và next task

`current_task` là task CPU đang đại diện trong Thread mode. `next_task` có thể là kết quả scheduler chờ PendSV commit.

Nếu hai pointer tồn tại, cần định nghĩa thời điểm nào `current` được đổi để trace, ISR và scheduler không nhìn trạng thái nửa switch.

---

## 30. Ready bitmap như derived state

Bitmap có thể được xem là **derived/cache state** từ ready queues. Vì là cache, nó phải được cập nhật cùng transaction insert/remove.

Nếu bitmap nói priority có task nhưng queue rỗng, scheduler có thể dereference invalid head. Nếu queue non-empty nhưng bit clear, task có thể bị starvation vô hình.

---

## 31. Complexity của kernel operations

Kernel realtime cần biết complexity:

- enqueue/dequeue known list node: O(1);
- remove known doubly node: O(1);
- find highest priority bằng bitmap: gần O(1) theo word width;
- sorted delayed insert: O(n) nếu linear list;
- timeout expiry ở head: O(1) mỗi task expired.

Big-O không đủ để chứng minh timing, nhưng là bước đầu để nhận biết operation unbounded theo số object.

---

## 32. Sorted list và delta list

### Absolute sorted list

Mỗi node lưu absolute deadline; list sorted tăng dần. Insert cần tìm vị trí.

### Delta list

Mỗi node lưu thời gian tương đối so với node trước. Tick chỉ decrement head, nhưng insert/remove cần điều chỉnh delta của neighbor.

Delta list giảm per-tick work nhưng invariant phức tạp hơn.

---

## 33. Timing wheel

Timing wheel bucket timeout theo range tick. Insert/expiry có thể bounded tốt hơn nhiều timer, đổi lại resolution và wrap/index logic phức tạp.

Đây là ví dụ data structure được chọn theo workload/timing requirement chứ không theo “cấu trúc nào quen thuộc nhất”.

---

## 34. Object lifetime và list safety

Một object không được free trong khi node của nó còn nằm trong kernel list. Nếu TCB bị free nhưng ready list vẫn giữ `ready_node`, scheduler sẽ truy cập use-after-free.

Do đó deletion cần detach khỏi mọi structure trước khi reclaim memory.

---

## 35. ABA-like hazards

Nếu object memory được free rồi nhanh chóng tái sử dụng cho object mới cùng địa chỉ, pointer cũ có thể “trông hợp lệ” nhưng refer tới generation khác.

Kernel nhỏ có thể giảm risk bằng static object lifetime hoặc generation ID trong diagnostics.

---

## 36. Diagnostics trong TCB

Các field diagnostic hữu ích:

- task ID/name;
- current/base/effective priority;
- state;
- stack range/current SP;
- runtime counter;
- last wake reason;
- wait object type/ID.

Diagnostic không nên trở thành logic scheduler phụ thuộc, trừ khi field là state chính thức.

---

## 37. Trace list transitions

Một trace record cho transition task có thể chứa:

```text
timestamp, task_id, old_state, new_state, reason, object_id
```

Khi scheduler bug, trace transition thường hữu ích hơn printf trong list operations.

---

## 38. Defensive invariants

Trong debug build có thể kiểm tra:

- node detached trước insert;
- node owner đúng list trước remove;
- TCB magic/version;
- SP trong stack range;
- priority trong range;
- state tương thích membership;
- list size không vượt task count.

Assertion càng gần điểm invariant bị phá thì root cause càng dễ tìm.

---

## 39. TCB size và cache/footprint

Trên Cortex-M3 không có data cache điển hình ở core, nhưng TCB size vẫn ảnh hưởng SRAM. Nếu mỗi TCB thêm 32 bytes và có 20 task, cost tăng 640 bytes — đáng kể với MCU nhỏ.

Vì vậy diagnostic/debug fields có thể conditional compile, nhưng các node cần cho correctness phải giữ.

---

## 40. Data structure và concurrency

List operation không tự thread-safe. Nếu ready list có thể được sửa từ SysTick/ISR và task context, kernel cần critical section hoặc quy tắc “chỉ một context sở hữu mutation”.

Một linked list hoàn hảo về pointer vẫn có thể corrupt nếu hai insert xen kẽ.

---

## 41. Separation of concerns

Một architecture kernel sạch thường tách:

- generic intrusive list primitive;
- task state transition helpers;
- scheduler policy;
- timeout manager;
- synchronization objects.

Semaphore không nên tự sửa ready pointers tùy ý; nó yêu cầu kernel wake helper thực hiện transition đúng toàn cục.

---

## 42. Các nguyên tắc cốt lõi

1. Data structure trong kernel được chọn theo operation pattern và timing bound.
2. Intrusive list tránh allocation ngầm và gắn node lifetime với object lifetime.
3. Một intrusive node chỉ thuộc tối đa một list tại một thời điểm.
4. Task cần nhiều node nếu đồng thời thuộc nhiều logical collections.
5. TCB là hợp đồng giữa scheduler, blocking logic và architecture port.
6. Saved stack pointer là cầu nối trực tiếp tới context-switch frame.
7. State enum và list membership phải luôn nhất quán.
8. Task transition là transaction nhiều cấu trúc, không phải gán một biến.
9. Ready bitmap là derived state và phải atomic với ready queue update.
10. Task registry không thay thế ready list.
11. Object không được free khi node còn linked.
12. Kernel invariant checks nên phát hiện corruption tại điểm gần nguyên nhân nhất.
13. Complexity của insert/remove/search ảnh hưởng trực tiếp khả năng bound kernel latency.
