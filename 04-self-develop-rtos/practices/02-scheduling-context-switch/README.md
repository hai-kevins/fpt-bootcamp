# 02 - Scheduling and Context Switch

Repository thực hành cho **Chủ đề 2** của lộ trình tự phát triển RTOS:

- Priority-based scheduling.
- Kiến trúc ARM Cortex-M3 phục vụ context switch.
- Initial task stack, MSP và PSP.
- SVC để khởi động task đầu tiên.
- PendSV để lưu và phục hồi context.
- SysTick, preemption và round-robin.

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

- Phân biệt scheduling decision và context-switch mechanism.
- Phân biệt cooperative scheduling và preemptive scheduling.
- Giải thích fixed-priority scheduling và round-robin.
- Phân biệt Thread mode và Handler mode.
- Phân biệt Main Stack Pointer và Process Stack Pointer.
- Giải thích hardware exception frame của Cortex-M3.
- Tạo initial stack frame cho một task chưa từng chạy.
- Dùng SVC để khởi động task đầu tiên bằng PSP.
- Dùng PendSV để lưu và phục hồi `R4-R11`.
- Dùng SysTick để tạo kernel tick và yêu cầu chuyển task.
- Tổ chức ready queue theo priority và ready bitmap.
- Kiểm tra local variable, stack guard và context qua nhiều lần switch.
- Phân tích firmware bằng `size`, `nm`, `objdump`, map file và listing file.

---

## 2. Cấu trúc repository

```text
02-scheduling-context-switch/
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
│   ├── panic.h
│   ├── scheduler.h
│   ├── scheduler_trace.h
│   ├── stm32f1.h
│   ├── systick.h
│   ├── task.h
│   └── uart.h
├── src/
│   ├── clock.c
│   ├── cortex_m3_port.c
│   ├── cortex_m3_portasm.S
│   ├── critical_section.c
│   ├── gpio.c
│   ├── main.c
│   ├── panic.c
│   ├── runtime.c
│   ├── scheduler.c
│   ├── scheduler_trace.c
│   ├── systick.c
│   ├── task.c
│   └── uart.c
├── labs/
│   ├── README.md
│   ├── 01-exception-stack-inspection/
│   ├── 02-initial-task-stack/
│   ├── 03-svc-first-task/
│   ├── 04-cooperative-pendsv/
│   ├── 05-priority-scheduler-host/
│   ├── 06-fixed-priority-target/
│   ├── 07-systick-time-slice/
│   ├── 08-isr-preemption/
│   ├── 09-round-robin/
│   └── 10-context-switch-stress/
├── docs/
│   ├── cortex-m3-exception-model.md
│   ├── gdb-context-switch.md
│   ├── initial-task-frame.md
│   ├── pendsv-context-switch.md
│   ├── scheduler-invariants.md
│   ├── scheduling-models.md
│   ├── svc-first-task.md
│   ├── systick-scheduling.md
│   └── target-validation.md
├── tools/
│   ├── check_all.sh
│   └── check_structure.py
└── build/
```

---

## 3. Firmware root làm gì?

Firmware root là **RTOS Scheduler Playground** chạy trên STM32F103:

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
  |-- Scheduler initialization
  |-- Tạo 5 task tĩnh
  |-- SysTick 1 kHz
  v
rtos_scheduler_start()
  |
  v
SVC_Handler
  |-- Restore initial R4-R11
  |-- Ghi PSP
  |-- Return về Thread mode bằng PSP
  v
Task execution
  |-- High task, priority 0
  |-- Worker A/B, priority 1
  |-- Monitor task, priority 2
  |-- Idle task, priority 3
  v
SysTick + PendSV
  |-- Delay và wake task
  |-- Time slice cho task cùng priority
  |-- Save/restore R4-R11
  |-- Fixed-priority preemption
```

Scheduler dùng static TCB và static task stack. Project không dùng dynamic allocation trong đường context switch.

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
build/scheduler_playground.elf
build/scheduler_playground.bin
build/scheduler_playground.hex
build/scheduler_playground.map
build/scheduler_playground.lst
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
gdb-multiarch build/scheduler_playground.elf
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
RTOS Scheduler Playground
h=help s=stats t=trace
```

Các lệnh:

| Lệnh | Chức năng |
|---|---|
| `h` | In hướng dẫn lệnh. |
| `s` | In kernel tick, switch count, ready bitmap và trạng thái task. |
| `t` | In snapshot scheduler trace gần nhất. |

Không in UART trong PendSV hoặc trong mỗi lần context switch.

---

## 8. Build các lab

Makefile ở thư mục gốc **chỉ quản lý project tổng kết RTOS Scheduler Playground**. Nó không gọi hoặc điều khiển Makefile của các lab.

Mỗi lab được build ngay trong thư mục của chính lab đó.

Ví dụ với lab chạy trên host:

```bash
cd labs/05-priority-scheduler-host
make test
make run
```

Ví dụ với lab chạy trên STM32:

```bash
cd labs/04-cooperative-pendsv
make
make flash-stlink
```

Để quay lại thư mục gốc:

```bash
cd ../..
```

Quy ước:

- Host labs: `02-initial-task-stack`, `05-priority-scheduler-host`.
- Target labs: `01-exception-stack-inspection`, `03-svc-first-task`, `04-cooperative-pendsv`, `06-fixed-priority-target`, `07-systick-time-slice`, `08-isr-preemption`, `09-round-robin`, `10-context-switch-stress`.
- Các lab host chạy AddressSanitizer và UndefinedBehaviorSanitizer.
- Mỗi lab tạo output trong thư mục `build/` của chính lab đó.
- `make clean` tại root chỉ xóa output của project tổng kết.
- `make clean` trong một lab chỉ xóa output của lab đó.
- Root Makefile không có rule tổng hợp như `lab01`, `all-labs`, `run-labs` hoặc `run-host-labs`.

---

## 9. Danh sách bài thực hành

| Bài | Chủ đề | Môi trường | Kết quả chính |
|---:|---|---|---|
| 1 | Exception Stack Inspection | STM32F103 | Quan sát MSP, PSP, EXC_RETURN và hardware exception frame |
| 2 | Initial Task Stack | Host | Tạo Cortex-M3 initial stack frame và kiểm tra layout |
| 3 | SVC First Task | STM32F103 | Khởi động task đầu tiên bằng SVC và PSP |
| 4 | Cooperative PendSV | STM32F103 | Hai task tự `yield()` qua PendSV và giữ local context |
| 5 | Priority Scheduler Policy | Host | Kiểm tra ready queue, bitmap, FIFO và rotation policy |
| 6 | Fixed-priority Scheduler | STM32F103 | Cho task ưu tiên cao preempt task ưu tiên thấp |
| 7 | SysTick Time Slice | STM32F103 | Luân phiên hai worker cùng priority mà không cần `yield()` |
| 8 | ISR Preemption | STM32F103 | EXTI0 đánh thức high task và defer switch qua PendSV |
| 9 | Round-robin | STM32F103 | Luân phiên ba task cùng priority mà không starvation |
| 10 | Context-switch Stress | STM32F103 | Kiểm tra stack guard, local counter và switch dài hạn |

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

Không nên chỉ chạy code có sẵn. Với context switch, cần kiểm tra thêm PSP, MSP, saved stack pointer, local variable và register frame bằng GDB.

---

## 11. Ghi chú kỹ thuật

### LED PC13 active-low

```text
PC13 = 0 -> LED sáng
PC13 = 1 -> LED tắt
```

### Clock và SysTick

Startup giữ HSI 8 MHz. SysTick dùng reload `7999`, tương ứng tick 1 kHz khi nguồn clock là core clock 8 MHz.

### Task stack

Mỗi task có stack tĩnh riêng. Stack được fill bằng `0xA5A5A5A5` và có guard word `0xDEADBEEF`.

### SVC

SVC khởi động task đầu tiên:

```text
main() dùng MSP
    ->
SVC Handler dùng MSP
    ->
restore initial task frame
    ->
Thread mode dùng PSP
```

### PendSV

PendSV có priority thấp nhất trong các system exception được cấu hình. Handler:

```text
Save R4-R11
Store current PSP vào TCB
Chọn task tiếp theo
Restore R4-R11
Write PSP
Exception return
```

### Scheduler

Scheduler là fixed-priority preemptive:

```text
Priority 0 -> cao nhất
Priority 3 -> thấp nhất
```

Task cùng priority được xoay vòng bằng time slice mặc định 5 tick. Idle task luôn READY để ready bitmap không rỗng khi kernel đang chạy.
