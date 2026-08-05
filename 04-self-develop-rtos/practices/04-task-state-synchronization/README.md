# 04 - Task State and Synchronization

Repository thực hành cho **Chủ đề 4** của lộ trình tự phát triển RTOS:

- Task states: READY, RUNNING, BLOCKED và SUSPENDED.
- Efficient blocking thay cho busy-wait.
- Delayed task list, timeout và tick wrap-around.
- Binary semaphore và counting semaphore.
- Mutex ownership, priority inversion và priority inheritance.
- ISR-to-task synchronization và deferred context switch.

Project được tổ chức theo cùng một bố cục:

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
├── LICENSE
├── VALIDATION.md
├── linker/
│   └── memory.ld
├── startup/
│   └── startup.c
├── include/
│   ├── clock.h
│   ├── compiler.h
│   ├── cortex_m3_port.h
│   ├── critical_section.h
│   ├── gpio.h
│   ├── list.h
│   ├── mutex.h
│   ├── panic.h
│   ├── ready_queue.h
│   ├── scheduler.h
│   ├── semaphore.h
│   ├── stm32f1.h
│   ├── synchronization_inspector.h
│   ├── systick.h
│   ├── task.h
│   ├── task_registry.h
│   ├── timeout.h
│   ├── uart.h
│   └── wait_list.h
├── src/
│   ├── clock.c
│   ├── cortex_m3_port.c
│   ├── cortex_m3_portasm.S
│   ├── critical_section.c
│   ├── gpio.c
│   ├── list.c
│   ├── main.c
│   ├── mutex.c
│   ├── panic.c
│   ├── ready_queue.c
│   ├── runtime.c
│   ├── scheduler.c
│   ├── semaphore.c
│   ├── synchronization_inspector.c
│   ├── systick.c
│   ├── task.c
│   ├── task_registry.c
│   ├── timeout.c
│   ├── uart.c
│   └── wait_list.c
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
│   ├── delayed-list.md
│   ├── efficient-blocking.md
│   ├── intrusive-list.md
│   ├── isr-safe-api.md
│   ├── linked-list-types.md
│   ├── list-invariants.md
│   ├── mutex.md
│   ├── priority-inheritance.md
│   ├── ready-queues.md
│   ├── semaphore.md
│   ├── static-task-creation.md
│   ├── target-validation.md
│   ├── task-registry.md
│   ├── task-stack.md
│   ├── task-state-machine.md
│   ├── tcb-layout.md
│   └── timeout-model.md
├── tools/
│   ├── check_all.sh
│   ├── check_structure.py
│   └── run_host_tests.sh
└── build/
```

---

## 3. Firmware root làm gì?

Firmware root là **RTOS Synchronization Playground**:

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
sudo apt install \
    gcc-arm-none-eabi \
    binutils-arm-none-eabi \
    make \
    gcc
```

Clang/LLD dùng làm toolchain thay thế:

```bash
sudo apt install clang lld llvm
```

Công cụ flash và debug tùy chọn:

```bash
sudo apt install openocd stlink-tools gdb-multiarch
```

Kiểm tra:

```bash
arm-none-eabi-gcc --version
arm-none-eabi-objcopy --version
arm-none-eabi-size --version
make --version
```

Makefile ưu tiên GNU Arm Embedded Toolchain; nếu không tìm thấy `arm-none-eabi-gcc`, nó tự chuyển sang Clang/LLD.

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

Các lệnh phân tích:

```bash
make size
make sections
make symbols
make symbols-size
make disasm
make clean
make rebuild
```

Kiểm tra cấu trúc repository:

```bash
python3 tools/check_structure.py
```

---

## 6. Flash firmware

ST-Link tools:

```bash
make flash-stlink
```

OpenOCD:

```bash
make flash-openocd
```

Mass erase:

```bash
make erase
```

Debug bằng OpenOCD và GDB:

```bash
openocd -f interface/stlink.cfg -f target/stm32f1x.cfg
```

Ở terminal khác:

```bash
gdb-multiarch build/synchronization_playground.elf
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

Banner dự kiến:

```text
RTOS Synchronization Playground
```

Các lệnh:

| Lệnh | Chức năng |
|---|---|
| `h` | In hướng dẫn. |
| `t` | In task states và priority. |
| `d` | In delayed list. |
| `s` | In trạng thái semaphore. |
| `m` | In mutex owner và waiters. |
| `u` | Suspend hoặc resume task `medium`. |
| `v` | Validate kernel structures. |

Synchronization Inspector chỉ tạo snapshot trong critical section ngắn; UART output được thực hiện ngoài PendSV và ngoài ISR.

---

## 8. Build các lab

Makefile ở thư mục gốc **chỉ quản lý project tổng kết RTOS Synchronization Playground**. Nó không gọi hoặc điều khiển Makefile của các lab.

Mỗi lab được build ngay trong thư mục của chính lab đó.

Ví dụ với lab chạy trên host:

```bash
cd labs/03-delayed-list-tick-wrap
make test
make run
```

Ví dụ với lab chạy trên STM32:

```bash
cd labs/06-isr-semaphore-wake
make
make flash-stlink
```

Để quay lại thư mục gốc:

```bash
cd ../..
```

Quy ước:

- Host labs: `01-task-state-machine`, `02-efficient-blocking`, `03-delayed-list-tick-wrap`, `04-delay-until`, `05-semaphore-host`, `07-mutex-ownership`, `08-priority-inheritance`, `09-suspend-resume`.
- Target labs: `06-isr-semaphore-wake`, `10-target-synchronization-playground`.
- Các lab host chạy AddressSanitizer và UndefinedBehaviorSanitizer.
- Mỗi lab tạo output trong thư mục `build/` của chính lab đó.
- `make clean` tại root chỉ xóa output của project tổng kết.
- `make clean` trong một lab chỉ xóa output của lab đó.
- Root Makefile không có rule tổng hợp như `lab01`, `all-labs`, `run-labs` hoặc `run-host-labs`.

---

## 9. Danh sách bài thực hành

| Bài | Chủ đề | Môi trường | Kết quả chính |
|---:|---|---|---|
| 1 | Task State Machine | Host | Kiểm tra transition matrix và phát hiện transition không hợp lệ |
| 2 | Efficient Blocking | Host | Chuyển `READY -> BLOCKED -> READY` và cập nhật ready bitmap |
| 3 | Delayed List and Tick Wrap | Host | Sắp xếp deadline và xử lý tick counter wrap-around |
| 4 | Delay and Delay-until | Host | So sánh chu kỳ tuyệt đối với drift của delay tương đối |
| 5 | Semaphore | Host | Kiểm tra binary/counting semaphore, timeout và waiter order |
| 6 | ISR Semaphore Wake | STM32F103 | EXTI0 đánh thức high task và defer switch qua PendSV |
| 7 | Mutex Ownership | Host | Chỉ owner được unlock và hỗ trợ timeout khi chờ mutex |
| 8 | Priority Inheritance | Host | Mô phỏng inversion High/Medium/Low và nâng effective priority |
| 9 | Suspend and Resume | Host | Đảm bảo task `SUSPENDED` không tự động wake theo timeout |
| 10 | Target Synchronization Playground | STM32F103 | Tích hợp task state, semaphore, mutex và priority inheritance |
---

## 10. Quy trình học đề xuất

```text
Đọc README của lab
      |
      v
Build và chạy lab
      |
      v
Quan sát output, UART, GDB hoặc unit test
      |
      v
Trả lời câu hỏi cuối lab
      |
      v
Cố ý tạo một lỗi hoặc phá một invariant
      |
      v
Giải thích nguyên nhân
      |
      v
Khôi phục và chuyển sang lab tiếp theo
```

Không nên chỉ chạy code có sẵn. Cần kiểm tra đồng thời state enum và list membership. Một task có state đúng nhưng vẫn nằm sai list vẫn là kernel corruption.

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
