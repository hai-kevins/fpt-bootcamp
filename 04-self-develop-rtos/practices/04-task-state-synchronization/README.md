# 04 - Task State and Synchronization

Repository thực hành cho **Chủ đề 4** của lộ trình tự phát triển RTOS:

- Task states: READY, RUNNING, BLOCKED và SUSPENDED.
- Efficient blocking thay cho busy-wait.
- Delayed task list, timeout và tick wrap-around.
- Binary semaphore và counting semaphore.
- Mutex ownership, priority inversion và priority inheritance.
- ISR-to-task synchronization và deferred context switch.

Project được tổ chức theo cùng bố cục với `01-rtos-introduction-memory-management`:

- Phần root là firmware tổng kết cuối chủ đề.
- `labs/` chứa từng bài thực hành độc lập.
- `docs/` chứa phần giải thích sâu hơn.
- `build/` chỉ chứa artifact sinh tự động.
- Makefile gốc chỉ quản lý project tổng kết.
- Mỗi lab có Makefile riêng và không được build từ root.

Cấu hình mặc định:

- Board: STM32F103C8T6 Blue Pill.
- CPU: ARM Cortex-M3.
- Flash: 64 KiB.
- SRAM: 20 KiB.
- Clock: HSI 8 MHz.
- LED: PC13, active-low.
- Button demo: PA0/EXTI0, rising edge.
- UART: USART1, PA9/PA10, 9600 baud, 8-N-1.
- Kernel tick: SysTick 1 kHz.
- Scheduler: fixed-priority preemptive.
- Priority số nhỏ hơn có mức ưu tiên cao hơn.
- Code style: bare-metal, register-level, không HAL/SPL.

---

## 1. Mục tiêu

Sau khi hoàn thành repository này, người học có thể:

- Vẽ và kiểm tra task state machine.
- Phân biệt BLOCKED và SUSPENDED.
- Remove task khỏi ready queue khi block và insert lại khi wake.
- Cài delayed list được sắp theo wake tick.
- So sánh tick an toàn khi counter wrap-around.
- Cài `delay()` và `delay_until()`.
- Xử lý no-wait, finite timeout và wait forever.
- Ngăn timeout và object event wake cùng một task hai lần.
- Cài binary semaphore và counting semaphore.
- Cài mutex có ownership.
- Giải thích priority inversion.
- Cài priority inheritance cơ bản bằng effective priority.
- Wake task từ ISR mà không block trong ISR.
- Suspend và resume task.
- Kiểm tra kernel/synchronization invariants trên host và target.

---

## 2. Cấu trúc repository

```text
04-task-state-synchronization/
├── README.md
├── Makefile
├── .gitignore
├── linker/
│   └── memory.ld
├── startup/
│   └── startup.c
├── include/
│   ├── list.h
│   ├── task.h
│   ├── scheduler.h
│   ├── timeout.h
│   ├── wait_list.h
│   ├── semaphore.h
│   ├── mutex.h
│   ├── synchronization_inspector.h
│   └── ... platform headers
├── src/
│   ├── list.c
│   ├── task.c
│   ├── scheduler.c
│   ├── timeout.c
│   ├── wait_list.c
│   ├── semaphore.c
│   ├── mutex.c
│   ├── synchronization_inspector.c
│   ├── cortex_m3_portasm.S
│   └── ... platform sources
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
├── docs/
├── tools/
└── build/
```

---

## 3. Firmware root làm gì?

Firmware root là **HairRTOS Synchronization Playground**:

```text
Reset
  |
  v
Startup + HSI 8 MHz + UART + GPIO
  |
  v
Static tasks and synchronization objects
  |-- Binary semaphore: button-event
  |-- Counting semaphore: items
  |-- Mutex: resource
  v
Scheduler starts first task through SVC
  |
  v
SysTick + PendSV
  |-- Delay/timeouts
  |-- Priority preemption
  |-- Round-robin
  v
Application demonstrations
  |-- PA0 ISR wakes event task
  |-- Producer/consumer use counting semaphore
  |-- High/Medium/Low demonstrate mutex inheritance
  |-- UART monitor prints kernel state
```

Task set:

| Task | Priority | Vai trò |
|---|---:|---|
| `event` | 0 | Chờ binary semaphore từ EXTI0 |
| `high` | 0 | Chờ mutex và tạo priority-inversion scenario |
| `medium` | 1 | CPU-bound task |
| `consumer` | 1 | Take counting semaphore |
| `low` | 2 | Giữ mutex trong critical workload |
| `producer` | 2 | Give counting semaphore |
| `monitor` | 2 | UART diagnostics |
| `idle` | 3 | Luôn READY, dùng `WFI` |

---

## 4. Cài toolchain

Ubuntu/Debian:

```bash
sudo apt update
sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi make gcc
```

Công cụ flash/debug tùy chọn:

```bash
sudo apt install openocd stlink-tools gdb-multiarch
```

Makefile tự chuyển sang Clang/LLD khi không tìm thấy GNU Arm:

```bash
sudo apt install clang lld llvm
```

---

## 5. Build firmware tổng kết

```bash
make
```

Artifact:

```text
build/synchronization_playground.elf
build/synchronization_playground.bin
build/synchronization_playground.hex
build/synchronization_playground.map
build/synchronization_playground.lst
```

Lệnh phân tích:

```bash
make size
make sections
make symbols
make symbols-size
make disasm
make clean
make rebuild
```

---

## 6. Flash firmware

```bash
make flash-stlink
```

Hoặc:

```bash
make flash-openocd
```

Mass erase:

```bash
make erase
```

---

## 7. UART

Kết nối:

```text
STM32F103       USB-UART
PA9  TX   --->  RX
PA10 RX   <---  TX
GND        ---  GND
```

Mở terminal:

```bash
picocom -b 9600 /dev/ttyUSB0
```

Các lệnh:

```text
h  help
t  task states and priorities
d  delayed list
s  semaphore state
m  mutex owner and waiters
u  suspend/resume medium task
v  validate kernel structures
```

Task Inspector chỉ snapshot trong critical section ngắn; UART output được thực hiện bên ngoài PendSV.

---

## 8. Build các lab

Makefile ở root chỉ quản lý project tổng kết.

Host lab:

```bash
cd labs/03-delayed-list-tick-wrap
make test
make run
```

Target lab:

```bash
cd labs/06-isr-semaphore-wake
make
make flash-stlink
```

Quy ước:

- Lab 01–05 và 07–09 chạy trên Ubuntu với ASan/UBSan.
- Lab 06 và 10 là firmware STM32 độc lập.
- `make clean` chỉ xóa `build/` của thư mục hiện tại.
- Root Makefile không có rule `lab01`, `all-labs` hoặc `run-labs`.

---

## 9. Danh sách bài thực hành

| Bài | Chủ đề | Kết quả chính |
|---:|---|---|
| 1 | Task state machine | Transition matrix và invalid transition detection |
| 2 | Efficient blocking | READY → BLOCKED → READY và ready bitmap |
| 3 | Delayed list | Sorted deadlines và tick wrap-around |
| 4 | Delay-until | Chu kỳ tuyệt đối và drift comparison |
| 5 | Semaphore | Binary/counting semaphore, timeout và waiter order |
| 6 | ISR semaphore wake | EXTI0 wakes high task, PendSV deferred switch |
| 7 | Mutex ownership | Owner-only unlock và timeout |
| 8 | Priority inheritance | High/Medium/Low inversion scenario |
| 9 | Suspend/resume | SUSPENDED không auto-wake |
| 10 | Target playground | Toàn bộ task state và synchronization trên STM32 |

---

## 10. Quy trình học đề xuất

```text
Đọc README lab
      |
      v
Build và chạy test
      |
      v
Quan sát state/list/object
      |
      v
Cố ý phá một invariant
      |
      v
Giải thích lỗi
      |
      v
Khôi phục và chuyển lab tiếp theo
```

Cần kiểm tra đồng thời state enum và list membership. Một task có state đúng nhưng vẫn nằm sai list vẫn là kernel corruption.

---

## 11. Ghi chú kỹ thuật

### Task states

```text
CREATED -> READY -> RUNNING
RUNNING -> READY/BLOCKED/SUSPENDED
BLOCKED -> READY/SUSPENDED
SUSPENDED -> READY
```

### Finite wait

Task finite wait đồng thời dùng:

```text
wait_node    -> object wait list
timeout_node -> delayed list
```

### Tick wrap

Deadline được kiểm tra bằng signed difference:

```c
(int32_t)(now - deadline) >= 0
```

Finite timeout phải nhỏ hơn nửa range 32-bit.

### Semaphore

Semaphore không có ownership. Khi give gặp waiter, resource được chuyển trực tiếp cho waiter thay vì vừa wake waiter vừa tăng count.

### Mutex

Mutex có owner. Non-owner unlock bị reject. Scheduler dùng `effective_priority`; priority inheritance tạm thời boost owner tới priority của waiter cao nhất.

### ISR

ISR không block. `give_from_isr()` chỉ cập nhật object, wake task và request PendSV. Context switch xảy ra sau ISR return.

### Giới hạn đào tạo

Implementation priority inheritance trong repository hỗ trợ nhiều owned mutex bằng intrusive owner list, nhưng chưa triển khai transitive inheritance qua chuỗi mutex nhiều tầng hoặc deadlock detection graph.
