# Chủ đề 2 — Scheduling và Context Switch trên ARM Cortex-M3
## Priority Scheduler, SysTick, SVC, PendSV và Exception Context

> Tài liệu này trình bày sâu hai lớp cần tách biệt khi tự phát triển RTOS: **scheduler policy** quyết định task nào nên chạy và **context-switch mechanism** hiện thực việc chuyển CPU từ task hiện tại sang task tiếp theo trên Cortex-M3.

---

## 1. Scheduling là gì?

Scheduling là quá trình chọn execution context tiếp theo được quyền sử dụng CPU.

Một scheduler phải trả lời:

```text
Trong tập task READY, task nào được chạy bây giờ?
```

Điều này khác với context switch:

```text
Làm thế nào lưu trạng thái task A và khôi phục trạng thái task B?
```

Tách policy khỏi mechanism là nguyên tắc kernel quan trọng.

---

## 2. Task readiness

Một task chỉ có thể được scheduler chọn nếu ở trạng thái **READY**. Task blocked chờ semaphore/delay không được cạnh tranh CPU.

Do đó scheduler correctness phụ thuộc vào invariant của task state lists, không chỉ thuật toán “find highest priority”.

---

## 3. Cooperative scheduling

Trong cooperative scheduling, task đang chạy chỉ mất CPU khi tự yield/block hoặc vào kernel ở điểm xác định.

Ưu điểm:

- context switch ít xảy ra;
- critical reasoning đơn giản hơn;
- task không bị preempt ở điểm tùy ý.

Nhược điểm:

- một task không yield có thể giữ CPU vô hạn;
- high-priority event không thể preempt ngay.

---

## 4. Preemptive scheduling

Preemptive scheduler có thể thay task hiện tại khi task ưu tiên cao hơn trở thành READY, thường nhờ interrupt/tick/kernel event.

Ưu điểm là response latency cho high-priority task tốt hơn.

Đổi lại, shared state giữa task có thể bị truy cập xen kẽ tại nhiều instruction boundary, nên synchronization trở nên bắt buộc.

---

## 5. Fixed-priority scheduling

Mỗi task có priority tương đối cố định. Scheduler luôn chọn highest-priority READY task.

Invariant cốt lõi:

> Không được để task priority thấp chạy nếu tồn tại task priority cao hơn đang READY, trừ khi kernel đang ở vùng tạm thời chưa cho phép scheduling/preemption.

Fixed-priority dễ hiện thực và phổ biến trên MCU.

---

## 6. Round-robin cùng priority

Nếu nhiều task cùng priority, kernel cần fairness policy. Round-robin luân phiên các task cùng priority sau time slice hoặc yield.

Điều quan trọng: round-robin chỉ giải fairness **trong cùng priority**. Nó không làm task thấp cạnh tranh ngang task cao.

---

## 7. Earliest Deadline First

EDF là dynamic-priority policy chọn job có deadline gần nhất. Về lý thuyết EDF có tính tối ưu mạnh trong một số model uniprocessor, nhưng implementation và overload behavior phức tạp hơn fixed-priority.

Chủ đề tự phát triển kernel nhỏ thường dùng fixed-priority để tập trung hiểu cơ chế context switch.

---

## 8. Starvation

Nếu task priority cao luôn READY, task thấp có thể không bao giờ chạy. Đây là starvation và có thể hoàn toàn hợp lệ theo fixed-priority policy nhưng không hợp requirement hệ thống.

Giải pháp không nhất thiết là “tăng fairness”; có thể cần sửa workload, priority assignment hoặc blocking behavior.

---

## 9. Ready queues

Một thiết kế phổ biến có một queue/list cho mỗi priority:

```text
priority Pmax: [T7][T9]
...
priority 2   : [T3]
priority 1   : [T1][T4]
priority 0   : [Idle]
```

Scheduler tìm priority cao nhất non-empty rồi chọn head.

### 9.1 Ready bitmap

Nếu số priority nhỏ, bitmap đánh dấu queue nào non-empty. Tìm highest set bit nhanh hơn scan toàn bộ danh sách.

Bitmap phải luôn đồng bộ với ready queues. Đây là một invariant kernel quan trọng.

---

## 10. Task Control Block và saved stack pointer

TCB chứa metadata task. Field quan trọng nhất cho port layer thường là **saved stack pointer**.

Khi task không chạy, phần lớn CPU context của nó nằm trên task stack. TCB chỉ cần giữ pointer tới vị trí stack frame đã save.

Mô hình:

```text
TCB_A.saved_sp ──→ [saved R4-R11][hardware exception frame] ...
```

Context switch đổi `current_tcb` và nạp stack pointer của task mới.

---

## 11. Cortex-M3 execution modes

Cortex-M có:

- **Thread mode**: chạy application/task;
- **Handler mode**: chạy exception/interrupt handler.

Context switch thường được thực hiện trong Handler mode để CPU exception mechanism hỗ trợ stacking/unstacking.

---

## 12. MSP và PSP

Cortex-M có hai stack pointer:

- **MSP — Main Stack Pointer**;
- **PSP — Process Stack Pointer**.

Một RTOS thường dùng:

- MSP cho kernel/exception handlers;
- PSP cho task thread context.

Cách này tách exception stack khỏi từng task stack và giúp context-switch logic rõ hơn.

---

## 13. Register context trên Cortex-M3

Registers có thể chia theo cơ chế save:

### 13.1 Hardware-stacked

Khi exception entry, CPU tự push một frame gồm các register caller-saved quan trọng như R0-R3, R12, LR, PC, xPSR.

### 13.2 Software-saved

R4-R11 là callee-saved theo ABI và không được hardware tự push trong basic exception frame. RTOS PendSV thường save/restore chúng bằng software.

Do đó complete task context conceptually là:

```text
Software frame: R4-R11
Hardware frame: R0-R3, R12, LR, PC, xPSR
```

---

## 14. Exception entry

Khi exception được nhận:

1. CPU hoàn tất/điều phối instruction theo kiến trúc;
2. push hardware exception frame lên stack đang được dùng;
3. cập nhật LR bằng một giá trị EXC_RETURN đặc biệt;
4. chuyển vào Handler mode;
5. lấy handler address từ vector table.

Điểm quan trọng là exception handler không phải một function call C thông thường dù compiler có thể tạo prologue/epilogue như function.

---

## 15. EXC_RETURN

EXC_RETURN là giá trị đặc biệt trong LR ở Handler mode. Nó mã hóa thông tin về cách return khỏi exception, ví dụ return về Thread/Handler mode và dùng MSP hay PSP.

Khi load giá trị EXC_RETURN vào PC theo cơ chế exception return, CPU tự unstack hardware frame.

Context switch dựa mạnh vào cơ chế này.

---

## 16. Initial task stack frame

Task chưa từng chạy nhưng context restore logic phải coi nó giống task đã từng bị exception preempt. Vì vậy kernel “giả lập” một exception frame ban đầu trên stack task.

Frame phải chứa conceptually:

- xPSR hợp lệ với Thumb state;
- PC = task entry;
- LR = task-return trap/handler;
- R0 = task argument nếu ABI yêu cầu;
- initial values cho software-saved registers.

Sau restore + exception return, CPU bắt đầu task như thể đang quay lại một thread đã bị interrupt.

---

## 17. Thumb state

Cortex-M chỉ thực thi Thumb instruction set. xPSR của initial frame phải biểu diễn Thumb state hợp lệ. Sai bit trạng thái có thể dẫn tới UsageFault/HardFault ngay khi task bắt đầu.

---

## 18. Stack alignment

ARM ABI yêu cầu alignment nhất định, thường 8-byte tại public interface/exception boundary. Initial stack và context frame phải giữ alignment đúng.

Sai alignment có thể không crash ngay mọi lúc, làm bug khó phát hiện.

---

## 19. SVC — Supervisor Call

SVC là synchronous exception do software kích hoạt. Trong kernel nhỏ, SVC thường dùng để chuyển từ startup code sang controlled kernel exception context, đặc biệt khi bắt đầu task đầu tiên.

SVC cũng có thể là system-call boundary nếu Thread mode chạy unprivileged.

---

## 20. PendSV

PendSV là exception được thiết kế rất phù hợp cho deferred context switching.

### 20.1 Vì sao PendSV phù hợp?

PendSV có thể được set pending từ SysTick/ISR nhưng chỉ chạy khi các exception priority cao hơn đã hoàn tất.

Nếu đặt PendSV ở priority thấp nhất, context switch không chen vào giữa ISR quan trọng.

### 20.2 PendSV responsibility

Một PendSV handler điển hình chỉ:

1. save software context của current task;
2. lưu PSP vào current TCB;
3. chọn/nhận next TCB;
4. nạp PSP của next task;
5. restore software context;
6. exception return.

Không nên log UART hoặc làm list operation dài trong assembly handler nếu có thể tách ra.

---

## 21. SysTick

SysTick là system timer tích hợp Cortex-M, thường dùng tạo periodic kernel tick.

Tick có thể phục vụ:

- time accounting;
- task delay timeout;
- time slice;
- software timer base.

SysTick không bắt buộc phải trực tiếp save/restore context. Nó có thể chỉ cập nhật kernel time và **pend PendSV** nếu cần reschedule.

---

## 22. Deferred preemption

Giả sử ISR làm một high-priority task trở thành READY. Thay vì context switch ngay trong ISR:

```text
ISR wakes task
   ↓
marks need_reschedule
   ↓
pend PendSV
   ↓
ISR returns / nested IRQ completes
   ↓
PendSV switches task
```

Đây là deferred preemption. Nó làm interrupt nesting và context-switch ownership đơn giản hơn.

---

## 23. SysTick, ISR và PendSV priority

Một arrangement thường gặp:

- device interrupts: priority theo latency requirement;
- SysTick: kernel-aware priority;
- PendSV: thấp nhất.

Mục tiêu là context switch chỉ xảy ra khi CPU không còn xử lý exception quan trọng hơn.

---

## 24. Scheduler lock và critical section

Ready list, current task và bitmap là shared kernel state có thể bị truy cập từ Thread mode và ISR/kernel handlers.

Critical section đảm bảo update nhiều field trở thành atomic ở mức kernel invariant.

### 24.1 PRIMASK

Có thể mask tất cả configurable interrupts. Đơn giản nhưng tăng interrupt latency.

### 24.2 BASEPRI

Trên core hỗ trợ, BASEPRI mask interrupt từ một priority threshold, cho phép interrupt rất cao vẫn chạy. Đây là cơ sở của nhiều RTOS port có “max syscall interrupt priority”.

Cortex-M3 hỗ trợ BASEPRI.

---

## 25. Scheduler decision và switch commit

Có thể tách:

- **decision**: tính `next_task`;
- **request**: pend PendSV;
- **commit**: PendSV thực sự save/restore context.

Tách ba bước giúp tránh ready list bị mutate ở nhiều nơi và làm reasoning rõ hơn.

---

## 26. Yield

Yield là yêu cầu scheduler xem xét task khác cùng/khác priority theo policy.

Trong fixed-priority preemptive kernel, yield của task current thường chỉ có ý nghĩa fairness với peer cùng priority; nó không làm task thấp chạy nếu current vẫn highest ready và policy không cho phép.

---

## 27. Time slicing

Time slice giới hạn thời gian một task giữ CPU trước khi peer cùng priority được rotate.

Tick-based slice có resolution theo tick. Slice quá nhỏ tăng context-switch overhead; quá lớn giảm fairness.

---

## 28. Tick frequency trade-off

Tick rate cao:

- timeout resolution tốt;
- time slice mịn;
- nhưng ISR overhead cao.

Tick rate thấp:

- overhead thấp;
- timing resolution thô.

Tickless kernel đi xa hơn bằng cách program timer tới deadline kế tiếp để tránh periodic interrupt khi idle.

---

## 29. Context-switch latency

Context-switch latency có thể phân tách:

```text
request-to-PendSV start
+ save current context
+ scheduler/commit cost
+ restore next context
+ exception return
```

Tùy định nghĩa benchmark, có thể chỉ đo một phần. Vì vậy report phải ghi measurement boundaries.

---

## 30. Interrupt-to-task latency

Đây là metric quan trọng hơn context-switch latency trong nhiều sản phẩm:

```text
hardware event
→ ISR entry
→ kernel wake
→ PendSV
→ high-priority task begins
```

Nó bị ảnh hưởng bởi interrupt masking, higher-priority ISR và current kernel critical section.

---

## 31. Scheduler invariants

Một fixed-priority scheduler nhỏ nên giữ ít nhất:

1. mỗi READY task nằm đúng một ready queue;
2. non-READY task không nằm ready queue;
3. ready bitmap bit = 1 iff queue tương ứng non-empty;
4. current task ở trạng thái RUNNING hoặc representation tương đương;
5. selected task là highest-priority READY theo policy;
6. idle task luôn có thể chạy khi không có task khác READY;
7. list mutation và state mutation diễn ra atomically theo kernel contract.

---

## 32. Context-switch invariants

1. Saved PSP của task trỏ đúng đầu software context frame.
2. R4-R11 restore đúng thứ tự đã save.
3. Hardware frame còn nguyên cho exception return.
4. EXC_RETURN được bảo toàn đúng.
5. Stack alignment hợp ABI.
6. `current_tcb` và PSP luôn cùng chỉ một task sau switch commit.
7. Không có C compiler-generated stack manipulation ngoài contract assembly đã tính tới.

---

## 33. Vì sao assembly handler dễ lỗi?

Compiler có thể tự push register khi function C được gọi. Nếu assembly giả định PSP/MSP layout không tính prologue đó, frame bị lệch.

Do đó low-level context switch thường cần:

- naked handler hoặc assembly hoàn toàn;
- contract rõ với C helper;
- tránh compiler tự tạo frame ngoài dự kiến.

---

## 34. Idle task

Idle task là task priority thấp nhất, luôn READY. Nó đảm bảo scheduler luôn có context hợp lệ.

Idle có thể:

- sleep CPU bằng WFI;
- reclaim resource deferred;
- update idle statistics.

Nhưng idle không được block theo cách khiến ready set rỗng nếu kernel không hỗ trợ “no current task”.

---

## 35. Scheduling và realtime analysis

Fixed-priority scheduler chỉ là mechanism. Để task thực sự đáp ứng deadline còn cần xét:

- period/release pattern;
- WCET;
- higher-priority interference;
- blocking do resource sharing;
- ISR interference.

Response-time analysis cho fixed priority thường có dạng lặp, trong đó response của task gồm own execution + interference của task ưu tiên cao + blocking. Đây là cầu nối giữa kernel implementation và realtime theory.

---

## 36. Priority assignment

Các heuristic như Rate Monotonic gán priority cao hơn cho task period ngắn hơn trong một số model periodic độc lập. Nhưng hệ thực tế có mutex, ISR, sporadic event và precedence constraint nên priority assignment phải dựa requirement, không dựa cảm giác.

---

## 37. Tick wrap-around

Kernel tick là integer hữu hạn và sẽ wrap. Scheduling/time slicing chỉ so delta ngắn thường dễ xử lý, nhưng timeout ở chủ đề sau phải dùng modular comparison đúng.

---

## 38. Fault symptoms của context switch

Các symptom phổ biến về mặt lý thuyết:

- crash ngay task đầu → initial frame/xPSR/PC/alignment;
- crash sau vài switch → save/restore order hoặc stack corruption;
- task sai argument → R0 initial frame;
- switch không xảy ra → PendSV pending/priority/interrupt mask;
- task thấp chạy khi task cao READY → scheduler invariant;
- HardFault ở exception return → EXC_RETURN/frame corrupt.

Việc phân loại symptom theo layer giúp debug nhanh hơn.

---

## 39. Các nguyên tắc cốt lõi

1. Scheduler policy và context-switch mechanism là hai vấn đề khác nhau.
2. READY state là điều kiện để task cạnh tranh CPU.
3. Fixed-priority scheduler phải luôn chọn highest-priority READY task.
4. Ready bitmap chỉ là acceleration structure và phải đồng bộ với ready queues.
5. Task context chủ yếu nằm trên task stack; TCB giữ saved stack pointer.
6. Cortex-M hardware tự save một phần context khi exception entry.
7. R4-R11 thường do RTOS save bằng software.
8. Initial task stack giả lập frame của một task đã bị exception ngắt.
9. SVC phù hợp khởi tạo/system-call boundary; PendSV phù hợp deferred context switch; SysTick cung cấp time base.
10. PendSV nên ở priority thấp để không chen vào ISR quan trọng.
11. Critical section kernel phải đủ ngắn để không phá interrupt latency.
12. Context-switch correctness phụ thuộc chặt vào ABI, stack alignment và EXC_RETURN.
13. Context-switch latency không bằng interrupt-to-task latency.
14. Realtime behavior cuối cùng phụ thuộc cả policy priority lẫn WCET/blocking/interference.
