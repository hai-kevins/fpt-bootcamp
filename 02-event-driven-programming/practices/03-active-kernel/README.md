# 03 - Active Kernel

Repository thực hành cho **Chủ đề 3: Active Kernel (AK)**:

- Cooperative priority scheduler.
- Run-to-completion task handler.
- Task mailbox và message routing.
- Pure, Common và Dynamic Message.
- Fixed message pool và reference count.
- One-shot và periodic software timer.
- Function-based FSM và Table State Machine.
- Interrupt bridge, UART shell, event record và fatal record.

Project được tổ chức theo cùng khuôn mẫu:

- Phần root là firmware tổng kết cuối chủ đề.
- `labs/` chứa từng bài thực hành độc lập.
- `docs/` chứa kiến trúc, task list, signal list và memory pool.
- `build/` chỉ chứa artifact sinh tự động.
- Makefile root chỉ quản lý firmware tổng kết và host unit test.
- Mỗi lab có Makefile riêng và không được build từ root.
- **Lab 14 Mini Game không được đưa vào repository theo yêu cầu.**

> Lưu ý: đây là **training implementation tự xây dựng theo các khái niệm trong tài liệu Active Kernel**, không phải bản sao mã nguồn chính thức của AK Embedded Base Kit. API được giữ nhỏ để phục vụ học tập, unit test và port sang STM32F103.

Cấu hình mặc định:

- Board: STM32F103C8T6 Blue Pill.
- CPU: ARM Cortex-M3.
- Flash: 64 KiB.
- SRAM: 20 KiB.
- Clock: HSI 8 MHz.
- LED: PC13, active-low.
- Button: PA0, EXTI0, pull-up.
- UART: USART1 PA9/PA10, 9600 baud, 8-N-1.
- Code style: C11, bare-metal register-level, không HAL/SPL.

---

## 1. Mục tiêu

Sau khi hoàn thành repository này, người học có thể:

- Phân biệt Active Kernel với RTOS preemptive truyền thống.
- Giải thích run-to-completion và cooperative scheduling.
- Thiết kế task table, priority và mailbox.
- Gửi Pure, Common và Dynamic Message.
- Kiểm tra payload length và allocation failure.
- Theo dõi message pool high-water mark.
- Hiểu ownership và reference count.
- Tạo one-shot và periodic timer.
- Viết function-based FSM và transition-table State Machine.
- Giữ ISR ngắn và post message từ interrupt.
- Xây UART shell không bypass task ownership.
- Ghi 64 event cuối cùng bằng ring buffer.
- Lưu fatal record trong `.noinit`.
- Chạy host unit test trước khi flash target.

---

## 2. Cấu trúc repository

```text
03-active-kernel/
├── README.md
├── Makefile
├── LICENSE
├── VALIDATION.md
├── ak/
│   ├── inc/
│   │   ├── ak.h
│   │   ├── ak_config.h
│   │   ├── ak_port.h
│   │   ├── event_record.h
│   │   ├── fatal.h
│   │   ├── fsm.h
│   │   ├── message.h
│   │   ├── task.h
│   │   ├── timer.h
│   │   └── tsm.h
│   └── src/
├── application/
│   ├── include/
│   └── src/
├── driver/
│   ├── inc/
│   └── src/
├── platform/
│   ├── inc/
│   └── src/
├── startup/
├── linker/
├── tests/
├── labs/
│   ├── 01-ak-structure/
│   ├── 02-two-tasks/
│   ├── 03-pure-message/
│   ├── 04-common-message/
│   ├── 05-dynamic-message/
│   ├── 06-one-shot-timer/
│   ├── 07-periodic-timer/
│   ├── 08-fsm/
│   ├── 09-tsm/
│   ├── 10-command-line/
│   ├── 11-pool-stress-test/
│   ├── 12-event-record/
│   └── 13-fatal-record/
├── docs/
└── build/
```

---

## 3. Firmware root làm gì?

```text
Reset -> Startup Code -> platform_init -> ak_init -> app_init
                                             |
                                             v
                                    SYSTEM_START message
                                             |
                                             v
+-------------------------------------------------------------+
| Active Kernel                                               |
| Message Pool -> Task Mailbox -> Priority Scheduler          |
| Run-to-Completion -> Timer -> FSM/TSM -> Event/Fatal Record |
+-------------------------------------------------------------+
      |          |          |          |          |
   Button       App        LED       Shell      Sensor
```

Task priority:

| Task | Priority | Ownership |
|---|---:|---|
| Button | 6 | Debounce timer |
| Application | 4 | Product state |
| LED | 3 | LED and blink timer |
| Shell | 2 | UART line buffer |
| Sensor | 1 | Sensor sample state |

---

## 4. Cài toolchain

```bash
sudo apt update
sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi make gcc
```

Clang/LLD thay thế:

```bash
sudo apt install clang lld llvm
```

Flash/debug:

```bash
sudo apt install openocd stlink-tools gdb-multiarch picocom
```

---

## 5. Build firmware tổng kết

GNU Arm:

```bash
make
```

Clang/LLD:

```bash
make TOOLCHAIN=clang
```

Artifact:

```text
build/active_kernel.elf
build/active_kernel.bin
build/active_kernel.hex
build/active_kernel.map
build/active_kernel.lst
```

Host test:

```bash
make test
```

Phân tích:

```bash
make size
make sections
make symbols
make symbols-size
make disasm
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

---

## 7. UART Shell

Kết nối:

```text
PA9  TX -> USB-UART RX
PA10 RX <- USB-UART TX
GND      - USB-UART GND
```

```bash
picocom -b 9600 /dev/ttyUSB0
```

Commands:

```text
help
task
pool
timer
state
led on
led off
led toggle
event dump
event clear
fatal show
fatal test
fatal clear
error
reset
```

Shell chỉ parse và post message. Task owner mới được thay đổi application state hoặc peripheral.

---

## 8. Build các lab

Root Makefile không build lab. Đi vào đúng thư mục:

```bash
cd labs/05-dynamic-message
make test
make run
```

Mỗi lab tạo executable trong `build/` của chính lab. Toàn bộ Lab 01-13 chạy trên host để tập trung vào kernel logic và test nhanh. Firmware root là target demo chạy trên STM32F103.

---

## 9. Danh sách bài thực hành

| Bài | Nội dung | Kết quả chính |
|---:|---|---|
| 1 | Đọc cấu trúc AK | Task/message/timer/FSM/TSM |
| 2 | Hai task | START -> HELLO routing |
| 3 | Pure Message | LED signal không payload |
| 4 | Common Message | Player status và length check |
| 5 | Dynamic Message | Fixed pool và high-water |
| 6 | One-shot Timer | Button debounce 20 ms |
| 7 | Periodic Timer | Heartbeat và đổi chu kỳ |
| 8 | FSM | IDLE/RUNNING/PAUSED/ERROR |
| 9 | TSM | Transition table |
| 10 | Command Line | Shell post message |
| 11 | Pool Stress Test | Failure counter và latency model |
| 12 | Event Record | 64 event cuối cùng |
| 13 | Fatal Record | Code/task/time/restart |

Lab 14 Mini Game đã được bỏ.

---

## 10. Quy trình học đề xuất

```text
Đọc README lab
 -> đọc source
 -> dự đoán output/invariant
 -> make test
 -> thay đổi một giả thiết
 -> tạo lỗi có chủ đích
 -> giải thích counter/state
 -> khôi phục code
 -> chuyển lab tiếp theo
```

Ưu tiên kiểm tra các invariant:

- Handler luôn return.
- Queue count không vượt capacity.
- Message pool used trở về 0 sau test.
- Payload length được kiểm tra.
- Timer cũ bị cancel khi rời state.
- ISR không parse command hoặc block.
- Shell không gọi application driver trực tiếp.

---

## 11. Ghi chú kỹ thuật

### Active Kernel không phải preemptive RTOS

Mỗi handler xử lý một message rồi return. Scheduler chọn task ready có priority cao nhất ở lần dispatch tiếp theo. Task không có stack riêng trong implementation này.

### Message ownership

Khi `ak_task_post()` thành công, queue/scheduler sở hữu message. Khi post thất bại, caller vẫn sở hữu và phải release.

### Pool

Training kernel dùng 16 slot, mỗi slot chứa tối đa 96 byte payload. Common Message giới hạn 24 byte. Đây là thiết kế dễ quan sát; production có thể dùng nhiều pool block-size khác nhau.

### Fatal record

`.noinit` chỉ tránh Startup Code clear vùng nhớ. Nó không bảo đảm giữ dữ liệu sau mất nguồn hoàn toàn.

### Hardware validation

Code được thiết kế cho STM32F103C8T6. Kết quả build/test host không thay thế kiểm thử thật trên board, UART, button và ST-Link.
