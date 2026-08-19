# Chủ đề 1 — Giới thiệu RTOS và quản lý bộ nhớ trong Kernel
## Real-Time Systems, Memory Layout, Allocation và Fragmentation

> Tài liệu này trình bày nền tảng lý thuyết của RTOS và memory management trong kernel. Trọng tâm là hiểu “real-time” theo nghĩa deadline/determinism, hiểu những vùng bộ nhớ mà một kernel phải quản lý, và hiểu sâu trade-off giữa static allocation, heap allocator, memory pool và first-fit.

---

## 1. RTOS là gì?

**RTOS — Real-Time Operating System** là một hệ điều hành/kernel cung cấp cơ chế quản lý execution contexts và resource sao cho hành vi thời gian của hệ thống có thể được kiểm soát và phân tích tốt hơn.

RTOS thường cung cấp:

- task/thread management;
- scheduling;
- context switching;
- time/tick/delay;
- synchronization;
- inter-task communication;
- software timer;
- memory/resource management;
- diagnostics.

Điểm cốt lõi không phải “có nhiều task”, mà là **có một mô hình rõ để quyết định task nào được chạy, khi nào bị block, khi nào được đánh thức, và latency có thể bị ảnh hưởng bởi những thành phần nào**.

---

## 2. Real-time không đồng nghĩa với “rất nhanh”

Một hệ thống nhanh nhưng thỉnh thoảng trễ không dự đoán được có thể kém realtime hơn một hệ thống chậm hơn nhưng luôn đáp ứng trong bound xác định.

Real-time quan tâm tới mối quan hệ:

```text
Computation result + time at which result becomes available
```

Một kết quả đúng nhưng đến sau deadline có thể bị xem là sai.

---

## 3. Các đại lượng thời gian cốt lõi

### 3.1 Latency

Latency là khoảng thời gian từ một mốc bắt đầu đến một mốc kết thúc đã định nghĩa. Không có “latency” chung chung; phải nói rõ hai điểm đo.

### 3.2 Response time

Response time thường bao gồm toàn bộ thời gian từ khi request/event xuất hiện đến khi công việc tương ứng hoàn tất.

Có thể phân rã:

```text
Response time
= release delay
+ scheduling delay
+ execution time
+ blocking/interference
```

### 3.3 Deadline

Deadline là thời điểm kết quả phải có. Có deadline tương đối so với release và deadline tuyệt đối theo time line hệ thống.

### 3.4 Jitter

Jitter là độ biến thiên của thời điểm hoặc latency giữa các lần thực hiện tương tự. Jitter thấp quan trọng trong control, sampling, communication và audio.

### 3.5 WCET

**Worst-Case Execution Time** là bound của thời gian execution trong điều kiện được mô hình hóa. “Giá trị lớn nhất đo được” không tự động là WCET vì measurement chỉ quan sát một tập execution hữu hạn.

### 3.6 Determinism

Determinism trong embedded thường nghĩa là hành vi timing và state transition có thể dự đoán trong bound hữu ích. Kernel được thiết kế để giảm nguồn bất định không cần thiết.

---

## 4. Hard, firm và soft real-time

### 4.1 Hard real-time

Miss deadline được xem là failure không chấp nhận được ở mức system requirement.

### 4.2 Firm real-time

Kết quả sau deadline gần như vô giá trị; miss thỉnh thoảng có thể được chấp nhận ở mức hệ thống nhưng không nên diễn ra thường xuyên.

### 4.3 Soft real-time

Miss deadline làm giảm chất lượng nhưng hệ thống vẫn có giá trị.

Phân loại này ảnh hưởng tới scheduler, memory policy, logging và mức chứng minh timing cần thiết.

---

## 5. Super-loop và RTOS

Super-loop có cấu trúc khái niệm:

```text
initialize
while (1) {
    service_A();
    service_B();
    service_C();
}
```

Ưu điểm là nhỏ, ít overhead, dễ hiểu. Nhưng response time của một service phụ thuộc tổng thời gian của các service đứng trước và mọi blocking operation.

RTOS thêm abstraction task và scheduler. Điều này giúp tách execution flow nhưng đổi lại tạo thêm:

- context-switch overhead;
- stack cho nhiều task;
- synchronization complexity;
- race/deadlock risk;
- kernel state cần bảo toàn.

Không phải firmware nào cũng cần RTOS. RTOS là công cụ giải bài toán concurrency/timing, không phải tiêu chuẩn “project chuyên nghiệp”.

---

## 6. Cấu trúc logic của một RTOS kernel nhỏ

```text
Application tasks
      ↓
Kernel API
      ↓
Task management ─ Scheduler ─ Synchronization
      │                │              │
      ├──────── Time/Delay ───────────┤
      │                │              │
      └──── Communication/Timer ──────┘
                       ↓
Architecture port
                       ↓
CPU exceptions / timer / interrupt
```

Kernel chia thành hai nhóm trách nhiệm:

- **policy**: task nào nên chạy, waiter nào được wake;
- **mechanism**: save/restore context, disable interrupt, program timer.

Tách hai nhóm này giúp kernel dễ reasoning và dễ test hơn.

---

## 7. Memory layout của firmware RTOS

Các section cơ bản vẫn là `.text`, `.rodata`, `.data`, `.bss`, heap và stack, nhưng RTOS thêm nhiều đối tượng runtime.

### 7.1 Kernel global state

Thường nằm trong `.bss`/`.data`: ready lists, current task pointer, tick count, object registries.

### 7.2 TCB

Task Control Block chứa metadata của từng task. Có thể static hoặc dynamic.

### 7.3 Task stack

Mỗi task có stack riêng nếu dùng preemptive/threaded model. Stack chứa call frames và saved context.

### 7.4 Queue/storage objects

Queue có metadata và storage buffer. Static allocation làm footprint rõ ngay khi link.

### 7.5 Synchronization objects

Semaphore/mutex thường nhỏ nhưng có wait list chứa task blocked.

### 7.6 Timer objects

Software timer cần expiry, period, callback/message, state và list node.

### 7.7 Diagnostic buffers

Trace buffer, stack guard, statistics đều tiêu thụ RAM và cần được tính trong budget.

---

## 8. Memory budget là một phần của kernel design

Trong MCU, RAM có giới hạn cố định. Một cách phân rã:

```text
Total SRAM
= static application data
+ kernel globals
+ TCBs
+ task stacks
+ queues/pools
+ heap
+ interrupt/main stack reserve
+ safety margin
```

Nếu memory allocation được làm “đến đâu tính đến đó”, hệ thống dễ rơi vào failure muộn ở runtime.

RTOS nhỏ thường ưu tiên khả năng biết trước maximum resource usage.

---

## 9. Static allocation

Static allocation quyết định storage trước runtime hoặc lúc link.

Ưu điểm:

- footprint biết trước;
- không allocation failure sau startup;
- không fragmentation;
- execution time của create/object init dễ bound.

Nhược điểm:

- có thể reserve quá nhiều;
- khó tạo số object thay đổi linh hoạt;
- cấu hình compile-time nhiều hơn.

Static allocation rất phù hợp với kernel object có số lượng tối đa biết trước.

---

## 10. Stack allocation

Automatic/local object thường nằm trên task stack. Lifetime gắn với call frame.

Trong RTOS, stack là resource riêng từng task. Vì vậy một local array lớn chỉ làm tăng requirement của stack task đó, không phải toàn heap.

Nhưng pointer tới local object không được dùng sau khi function return hoặc sau khi object lifetime kết thúc. Điều này đặc biệt nguy hiểm với asynchronous queue/message.

---

## 11. Dynamic allocation

Dynamic allocator quản lý một vùng heap và trả block theo request runtime.

Lợi ích:

- linh hoạt;
- tạo object theo nhu cầu;
- memory có thể tái sử dụng giữa các phase.

Rủi ro:

- fragmentation;
- allocation latency phụ thuộc trạng thái heap;
- failure giữa runtime;
- ownership phức tạp;
- corruption lan rộng.

Trong realtime path, câu hỏi quan trọng không chỉ là “malloc có nhanh không?” mà là “thời gian và khả năng thành công có bound đủ mạnh không?”.

---

## 12. Memory pool

Pool chứa các block kích thước cố định.

Giả sử có `N` block, mỗi block kích thước `B`, memory cost gần:

```text
Pool cost ≈ N × B + metadata
```

Allocation có thể là pop một node từ free list, deallocation là push lại — thường O(1).

Trade-off là **internal fragmentation** nếu request nhỏ hơn block.

Pool rất phù hợp TCB, message hoặc object có size class rõ.

---

## 13. Internal fragmentation

Internal fragmentation là phần memory nằm trong block đã cấp nhưng application không dùng.

Ví dụ allocator cấp theo 16-byte alignment cho request 17 byte, block payload thực có thể thành 32 byte. Phần dư là internal waste.

Internal fragmentation thường có bound rõ nếu size class cố định.

---

## 14. External fragmentation

External fragmentation xảy ra khi tổng free memory đủ nhưng bị chia thành nhiều hole nhỏ, không có block liên tục đủ lớn cho request.

Ví dụ:

```text
[used 20][free 10][used 20][free 10][used 20][free 10]
```

Tổng free = 30 nhưng request 20 có thể thất bại nếu không có hole ≥ 20.

External fragmentation là vấn đề chính của variable-size heap.

---

## 15. Memory leak

Leak xảy ra khi block vẫn marked used nhưng không còn owner hợp lệ để free. Trong MCU chạy lâu, leak nhỏ có thể trở thành exhaustion sau hàng giờ/ngày.

Leak là lỗi ownership protocol, không chỉ allocator bug.

---

## 16. Double free, invalid free và use-after-free

### Double free

Cùng block được trả allocator hai lần. Free list có thể chứa duplicate pointer và sau đó cùng memory được cấp cho hai owner.

### Invalid free

Pointer không phải start của payload hợp lệ hoặc không thuộc heap bị free.

### Use-after-free

Code tiếp tục truy cập block đã trả allocator; block có thể đã được owner khác sử dụng.

Ba lỗi này làm heap corruption thường biểu hiện xa điểm gây lỗi.

---

## 17. Heap metadata

Variable-size allocator cần metadata cho từng block, thường gồm:

- size;
- free/used flag;
- link tới block kế/prev;
- optional magic/checksum.

Metadata có thể nằm ngay trước payload:

```text
[block header][payload bytes]
```

Ưu điểm là tìm header từ payload dễ; nhược điểm là buffer underflow từ payload có thể phá header.

---

## 18. Alignment

Allocator phải trả pointer thỏa alignment của kiểu dữ liệu target. Nếu alignment requirement là `A`, requested size thường được round-up:

```text
aligned = ceil(size / A) × A
```

Với `A` là power-of-two, có thể dùng bit arithmetic. Nhưng về mặt lý thuyết cần đảm bảo không integer overflow trước khi round-up.

---

## 19. First-fit allocator

First-fit duyệt free blocks theo thứ tự memory/list và chọn block đầu tiên có kích thước đủ lớn.

### 19.1 Ưu điểm

- implementation đơn giản;
- thường dừng sớm;
- metadata không quá phức tạp.

### 19.2 Nhược điểm

- allocation time O(n) theo số block trong trường hợp xấu;
- hình thành nhiều hole nhỏ ở vùng đầu heap;
- fragmentation phụ thuộc workload.

### 19.3 Bản chất policy

“First-fit” chỉ là policy chọn free block. Một allocator hoàn chỉnh còn cần splitting, coalescing, validation và error handling.

---

## 20. Splitting

Nếu free block lớn hơn request đáng kể, allocator có thể chia:

```text
Large free block
      ↓
Allocated block + remaining free block
```

Không nên split nếu phần còn lại quá nhỏ để chứa header + minimum payload, vì sẽ tạo unusable fragment.

Điều kiện split là một invariant quan trọng.

---

## 21. Coalescing

Khi free một block, nếu block vật lý kế bên cũng free, allocator có thể merge chúng để tạo block lớn hơn.

Coalescing giảm external fragmentation nhưng yêu cầu allocator biết adjacency trong address space, không chỉ adjacency trong free list.

Có thể coalesce:

- immediate khi free;
- deferred khi allocation cần block lớn.

Immediate coalescing tăng cost của free nhưng giữ heap ít fragment hơn.

---

## 22. Free-list organization

Có nhiều tổ chức:

### 22.1 Address-ordered list

Block theo địa chỉ. Coalescing dễ vì hàng xóm logic thường là hàng xóm vật lý.

### 22.2 Size-ordered list

Tìm block phù hợp có thể tốt hơn cho một số policy nhưng coalescing cần metadata phụ.

### 22.3 Segregated free lists

Nhiều bucket theo size class. Allocation nhanh hơn, phức tạp hơn.

Một RTOS nhỏ thường chọn cấu trúc đơn giản để invariant dễ chứng minh.

---

## 23. First-fit, best-fit và worst-fit

### First-fit

Chọn block đầu đủ lớn.

### Best-fit

Chọn block nhỏ nhất vẫn đủ. Có thể giảm waste tức thời nhưng thường phải scan nhiều hơn và có thể tạo fragment rất nhỏ.

### Worst-fit

Chọn block lớn nhất, cố giữ các remainder lớn. Ít phổ biến trong embedded kernel nhỏ.

Không có policy luôn tối ưu; workload mới quyết định fragmentation thực.

---

## 24. Buddy allocator

Buddy allocator chia memory theo power-of-two block và merge buddy khi cùng free.

Ưu điểm:

- split/merge có cấu trúc;
- dễ tìm buddy;
- bounded hơn một số heap tổng quát.

Nhược điểm:

- internal fragmentation cao với size không gần power of two.

Nó minh họa trade-off giữa flexibility và deterministic structure.

---

## 25. TLSF và bounded allocation

Các allocator realtime như Two-Level Segregated Fit được thiết kế để có operation time gần O(1) bằng bitmap/size class. Chúng phức tạp hơn first-fit nhưng cho thấy rằng dynamic allocation không nhất thiết đồng nghĩa unbounded linear search nếu allocator được thiết kế chuyên cho realtime.

---

## 26. Heap invariant

Một allocator đáng tin cậy cần invariant như:

1. mọi block nằm hoàn toàn trong heap;
2. block không overlap;
3. tổng kích thước block + metadata phủ đúng heap theo model;
4. free list chỉ chứa block free;
5. block free không xuất hiện hai lần;
6. size thỏa alignment/minimum;
7. adjacent free blocks được merge nếu policy yêu cầu immediate coalescing.

Invariant là công cụ reasoning tốt hơn việc chỉ nhìn kết quả malloc/free.

---

## 27. Fragmentation metrics

Một indicator phổ biến cho external fragmentation dựa trên:

```text
largest_free_block / total_free_memory
```

Nếu tỷ lệ thấp, free memory bị chia vụn. Tuy nhiên một metric đơn không mô tả toàn bộ workload; distribution của free block cũng quan trọng.

Internal fragmentation có thể ước tính từ:

```text
allocated_capacity − requested_bytes
```

---

## 28. Stack sizing trong RTOS

Task stack phải chứa:

- sâu nhất của call graph;
- local variables;
- saved registers;
- exception/context frame;
- compiler alignment;
- nested call/library usage.

Stack quá nhỏ gây corruption; quá lớn lãng phí SRAM.

### 28.1 High-watermark

Fill stack bằng pattern và quan sát vùng đã bị sử dụng cho phép ước lượng maximum observed usage. Đây không phải proof tuyệt đối nhưng là diagnostic quan trọng.

### 28.2 Guard region

Canary/guard pattern ở biên stack giúp phát hiện overflow trước khi metadata khác bị phá.

---

## 29. Kernel object lifetime

Kernel object có thể có lifetime:

- static whole-system;
- created once at initialization;
- dynamic runtime;
- recycled from pool.

Lifetime policy ảnh hưởng trực tiếp tới allocator requirement. Nếu mọi task/queue được tạo trước scheduler start và không delete, general heap có thể hoàn toàn không cần thiết sau init.

---

## 30. Allocation trong critical path

Allocation có thể làm tăng response time nếu:

- phải scan nhiều block;
- critical section dài;
- coalescing phức tạp;
- allocator lock bị contention.

Vì vậy hệ realtime thường tránh variable-size allocation trong ISR hoặc high-priority timing path.

---

## 31. Failure policy khi hết memory

Out-of-memory phải có semantics rõ:

- API trả failure;
- subsystem degrade;
- drop message;
- trigger fatal;
- reset có kiểm soát.

Không nên tiếp tục với NULL pointer hoặc state nửa tạo object.

Kernel API cần đảm bảo operation thất bại không phá invariant đã có.

---

## 32. Memory protection và MPU

Cortex-M3 có thể có MPU tùy implementation. MPU cho phép chia region với permission, giúp phát hiện stack overflow hoặc task access sai vùng tốt hơn.

RTOS nhỏ không nhất thiết dùng MPU, nhưng memory safety architectural boundary là hướng mở rộng quan trọng.

---

## 33. Quan hệ giữa memory management và các chủ đề RTOS sau

- **Scheduler** cần TCB và stack có lifetime ổn định.
- **Context switch** phụ thuộc stack alignment/layout.
- **Linked list** cần node không bị free khi còn trong kernel list.
- **Semaphore/queue** cần object lifetime dài hơn mọi waiter.
- **Message passing** cần ownership rõ.
- **Software timer** cần timer object không biến mất khi đang armed.

Vì vậy memory management không phải module phụ; nó là nền của mọi kernel invariant.

---

## 34. Các nguyên tắc cốt lõi

1. Real-time là đúng thời điểm, không chỉ chạy nhanh.
2. RTOS thêm scheduling abstraction nhưng cũng thêm memory và concurrency complexity.
3. Memory budget phải bao gồm TCB, task stack, queue, timer và diagnostics.
4. Static allocation cho predictability cao nhất khi số object biết trước.
5. Variable-size heap tạo external fragmentation và latency phụ thuộc heap state.
6. Fixed-size pool đổi flexibility lấy bounded behavior.
7. First-fit chỉ là block-selection policy; allocator còn cần split, coalesce và invariant validation.
8. Heap corruption thường là lỗi ownership hoặc bounds, biểu hiện xa điểm gây lỗi.
9. High-watermark là measurement, không phải proof stack sẽ không bao giờ overflow.
10. Kernel object lifetime phải rõ trước khi thiết kế allocator.
11. Out-of-memory là một failure mode phải có policy, không phải trường hợp “không thể xảy ra”.
12. Memory management phải được thiết kế cùng timing requirements của kernel.
