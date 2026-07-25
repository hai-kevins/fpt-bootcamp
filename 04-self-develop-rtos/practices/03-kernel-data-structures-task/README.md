# 03 - Kernel Data Structures and Task

Repository thực hành cho **Chủ đề 3** của lộ trình tự phát triển RTOS:

- Các loại linked list thường dùng trong kernel.
- Intrusive doubly linked list và `container_of`.
- List ownership, membership và invariant.
- Task Control Block.
- Static task creation và task stack initialization.
- All-task registry, per-priority ready queues và ready bitmap.

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
- UART: USART1, PA9/PA10, 9600 baud, 8-N-1.
- Kernel tick: SysTick 1 kHz.
- Scheduler: fixed-priority preemptive.
- Priority số nhỏ hơn có mức ưu tiên cao hơn.
- Code style: bare-metal, register-level, không HAL/SPL.

---

## 1. Mục tiêu

Sau khi hoàn thành repository này, người học có thể:

- Phân biệt singly linked list và doubly linked list.
- Giải thích intrusive list khác non-intrusive list như thế nào.
- Dùng `container_of` để lấy object chứa list node.
- Thiết kế list ownership và phát hiện duplicate insertion.
- Kiểm tra head, tail, count, forward link, backward link và cycle.
- Thiết kế Task Control Block cho Cortex-M3.
- Giải thích vì sao `saved_sp` phải giữ contract với PendSV assembly.
- Tạo task bằng TCB và stack tĩnh do application cung cấp.
- Tạo initial task frame, stack guard và stack high-water.
- Tổ chức all-task registry.
- Tổ chức ready queue riêng cho từng priority.
- Dùng ready bitmap để biểu diễn các priority đang có task READY.
- Cho một TCB nằm đồng thời trong nhiều list bằng các node khác nhau.
- Chạy unit test trên host trước khi ghép dữ liệu kernel với target.
- Phân tích firmware bằng `size`, `nm`, `objdump`, map file và listing file.

---

## 2. Cấu trúc repository

```text
03-kernel-data-structures-task/
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
│   ├── panic.h
│   ├── ready_queue.h
│   ├── scheduler.h
│   ├── stm32f1.h
│   ├── systick.h
│   ├── task.h
│   ├── task_inspector.h
│   ├── task_registry.h
│   └── uart.h
├── src/
│   ├── clock.c
│   ├── cortex_m3_port.c
│   ├── cortex_m3_portasm.S
│   ├── critical_section.c
│   ├── gpio.c
│   ├── list.c
│   ├── main.c
│   ├── panic.c
│   ├── ready_queue.c
│   ├── runtime.c
│   ├── scheduler.c
│   ├── systick.c
│   ├── task.c
│   ├── task_inspector.c
│   ├── task_registry.c
│   └── uart.c
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
├── docs/
│   ├── intrusive-list.md
│   ├── linked-list-types.md
│   ├── list-invariants.md
│   ├── ready-queues.md
│   ├── static-task-creation.md
│   ├── target-validation.md
│   ├── task-registry.md
│   ├── task-stack.md
│   └── tcb-layout.md
├── tools/
│   ├── check_all.sh
│   ├── check_structure.py
│   └── run_host_tests.sh
└── build/
```

---

## 3. Firmware root làm gì?

Firmware root là **RTOS Task Inspector** chạy trên STM32F103:

```text
Reset
  |
  v
Vector Table
  |
  v
Reset_Handler
  |-- Copy .data từ Flash sang SRAM
  |-- Clear .bss
  v
main()
  |-- HSI 8 MHz
  |-- GPIO PC13
  |-- USART1 polling
  |-- Khởi tạo scheduler
  |-- Tạo 4 task tĩnh
  |-- SysTick 1 kHz
  v
Static task creation
  |-- Build Cortex-M3 initial frame
  |-- Gán saved_sp
  |-- Fill stack bằng 0xA5A5A5A5
  |-- Đặt guard 0xDEADBEEF
  |-- Init ready_node
  |-- Init all_task_node
  v
Kernel structures
  |-- Intrusive all-task registry
  |-- Ready queue theo priority
  |-- Ready bitmap
  v
SVC + PendSV
  |-- Task chạy bằng PSP
  |-- Exception chạy bằng MSP
  |-- Lưu/phục hồi R4-R11
  v
UART Task Inspector
```

Task được tạo trong firmware root:

| Task | ID | Priority | Stack |
|---|---:|---:|---:|
| `worker-a` | 0 | 1 | 160 words |
| `worker-b` | 1 | 1 | 160 words |
| `monitor` | 2 | 2 | 320 words |
| `idle` | 3 | 3 | 96 words |

Hai worker cùng priority nằm trong cùng FIFO ready queue. Monitor xử lý lệnh UART và in snapshot của các cấu trúc kernel. Idle luôn READY ở priority thấp nhất.

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
build/task_inspector.elf
build/task_inspector.bin
build/task_inspector.hex
build/task_inspector.map
build/task_inspector.lst
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
gdb-multiarch build/task_inspector.elf
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
RTOS Task Inspector
h help | t tasks | r ready | c current | s stacks | v validate
```

Các lệnh:

| Lệnh | Chức năng |
|---|---|
| `h` | In hướng dẫn. |
| `t` | In tất cả TCB trong all-task registry. |
| `r` | In ready bitmap và từng ready queue. |
| `c` | In current task, kernel tick và context-switch count. |
| `s` | In stack guard và số word chưa sử dụng. |
| `v` | Validate registry, ready queues và scheduler. |

Task Inspector tạo snapshot trong critical section ngắn rồi mới in UART. Không in UART trong PendSV.

---

## 8. Build các lab

Makefile ở thư mục gốc **chỉ quản lý project tổng kết RTOS Task Inspector**. Nó không gọi hoặc điều khiển Makefile của các lab.

Mỗi lab được build ngay trong thư mục của chính lab đó.

Ví dụ với lab chạy trên host:

```bash
cd labs/04-tcb-layout
make test
make run
```

Ví dụ với lab chạy trên STM32:

```bash
cd labs/10-target-task-inspector
make
make flash-stlink
```

Để quay lại thư mục gốc:

```bash
cd ../..
```

Quy ước:

- Host labs: `01-singly-linked-list`, `02-intrusive-doubly-list`, `03-list-invariants`, `04-tcb-layout`, `05-static-task-creation`, `06-task-stack-initialization`, `07-all-task-registry`, `08-priority-ready-queues`, `09-tcb-multiple-lists`.
- Target labs: `10-target-task-inspector`.
- Các lab host chạy AddressSanitizer và UndefinedBehaviorSanitizer.
- Mỗi lab tạo output trong thư mục `build/` của chính lab đó.
- `make clean` tại root chỉ xóa output của project tổng kết.
- `make clean` trong một lab chỉ xóa output của lab đó.
- Root Makefile không có rule tổng hợp như `lab01`, `all-labs`, `run-labs` hoặc `run-host-labs`.

---

## 9. Danh sách bài thực hành

| Bài | Chủ đề | Kết quả chính |
|---:|---|---|
| 1 | Singly linked list | Push, pop, find, count và cycle detection |
| 2 | Intrusive doubly list | Insert/remove O(1), duyệt hai chiều và `container_of` |
| 3 | List invariants | Owner, duplicate insertion, wrong-list removal và corruption |
| 4 | TCB layout | `sizeof`, `offsetof` và `saved_sp` tại offset 0 |
| 5 | Static task creation | Tạo task từ TCB/stack tĩnh và validate input |
| 6 | Task stack initialization | Initial frame, guard word và high-water |
| 7 | All-task registry | Lưu, duyệt và tìm task bằng intrusive node |
| 8 | Priority ready queues | FIFO ready queues, rotation và ready bitmap |
| 9 | TCB in multiple lists | Một TCB tham gia registry và ready queue bằng hai node |
| 10 | Target Task Inspector | Quan sát TCB và list trên STM32F103 qua UART |

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

Không nên chỉ chạy code có sẵn. Với linked list, cần thay đổi thứ tự insert/remove và chủ động phá `owner`, `next`, `previous` hoặc `count`. Với TCB, cần kiểm tra `sizeof`, `offsetof`, stack range và initial frame.

---

## 11. Ghi chú kỹ thuật

### Intrusive node

TCB chứa hai node độc lập:

```text
ready_node
all_task_node
```

Một node chỉ thuộc một list tại một thời điểm. Cùng một TCB có thể thuộc nhiều list nếu mỗi list sử dụng một node khác nhau.

### `saved_sp`

`rtos_task_t.saved_sp` nằm ở offset 0 và được khóa bằng `_Static_assert`. PendSV assembly dựa vào contract này để lưu và phục hồi PSP.

### Stack

Stack task được fill bằng:

```text
0xA5A5A5A5
```

Guard word:

```text
0xDEADBEEF
```

Initial frame chứa software-saved `R4-R11` và hardware frame `R0-R3`, `R12`, `LR`, `PC`, `xPSR`.

### Ready queue

Project có bốn mức priority:

```text
0 = cao nhất
3 = thấp nhất
```

Mỗi priority có một intrusive FIFO list. Ready bitmap phải luôn phản ánh đúng queue nào không rỗng.

### Runtime

Firmware link bằng `-nostdlib` và `-nostartfiles`. Project không dùng HAL, SPL, C library heap hoặc dynamic allocation trong kernel.
