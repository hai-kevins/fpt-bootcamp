# 05 - Communication, Timer and Benchmark

Repository thực hành cho **Chủ đề 5** của lộ trình tự phát triển RTOS:

- Message passing giữa task và ISR.
- Static queue, mailbox semantics và message ownership.
- Fixed-block message pool.
- One-shot và periodic software timer.
- Timer service task.
- Context-switch, event, message và timer latency benchmark.

Project được tổ chức theo cùng triết lý với `01-rtos-introduction-memory-management`:

- Phần root là firmware tổng hợp cuối chủ đề.
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
- Timestamp: DWT `CYCCNT`.
- Scheduler: fixed-priority preemptive.
- Code style: bare-metal, register-level, không HAL/SPL.

---

## 1. Mục tiêu

Sau khi hoàn thành repository này, người học có thể:

- Phân biệt shared memory, queue, mailbox và event signaling.
- Thiết kế queue tĩnh bằng ring buffer.
- Quản lý blocking send/receive và timeout.
- Gửi message từ ISR mà không block.
- Giải thích copy-by-value, pointer message và ownership.
- Xây fixed-block message pool có double-free detection.
- Xây one-shot và periodic software timer.
- Chạy callback trong timer service task thay vì SysTick.
- Đo context-switch latency, message latency và timer expiry latency.
- Tính minimum, maximum, average và histogram.
- Ghi rõ điều kiện benchmark để kết quả có thể tái lập.
- Phân tích firmware bằng `size`, `nm`, `objdump`, map file và listing file.

---

## 2. Cấu trúc repository

```text
05-communication-timer-benchmark/
├── README.md
├── Makefile
├── .gitignore
├── linker/
├── startup/
├── src/
├── include/
├── labs/
│   ├── README.md
│   ├── 01-static-ring-buffer-queue
│   ├── 02-blocking-queue-host
│   ├── 03-queue-from-isr
│   ├── 04-message-pool
│   ├── 05-software-timer-list
│   ├── 06-timer-service-task
│   ├── 07-timestamp-backends
│   ├── 08-context-switch-benchmark
│   ├── 09-event-message-latency
│   └── 10-target-communication-benchmark-console
├── docs/
├── tools/
└── build/
```

---

## 3. Firmware root làm gì?

Firmware root là **HairRTOS Communication and Benchmark Console**:

```text
Producer task
    |
    v
Static message queue
    |
    v
Consumer task
    |
    v
Message-latency statistics

SysTick
    |
    v
Software timer list
    |
    v
Timer service task
    |
    v
One-shot/periodic callbacks

Switch Task A <-> Switch Task B
    |
    v
Context-switch statistics
```

UART console in trạng thái queue, pool, timer, benchmark và validator. UART không được sử dụng trong timing window, PendSV hoặc SysTick callback path.

---

## 4. Cài toolchain

```bash
sudo apt update
sudo apt install \
    gcc-arm-none-eabi \
    binutils-arm-none-eabi \
    make \
    gcc
```

Công cụ tùy chọn:

```bash
sudo apt install openocd stlink-tools gdb-multiarch
```

Makefile tự chuyển sang Clang/LLD nếu không tìm thấy `arm-none-eabi-gcc`.

---

## 5. Build firmware tổng kết

```bash
make
```

Artifact:

```text
build/communication_benchmark_console.elf
build/communication_benchmark_console.bin
build/communication_benchmark_console.hex
build/communication_benchmark_console.map
build/communication_benchmark_console.lst
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

Lệnh:

```text
h  help
q  queue statistics
p  message pool state
t  software timer state
s  benchmark statistics
v  validate kernel objects
```

---

## 8. Build các lab

Makefile gốc chỉ quản lý firmware tổng kết. Mỗi lab có Makefile riêng.

Lab host:

```bash
cd labs/01-static-ring-buffer-queue
make test
make run
```

Lab target:

```bash
cd labs/03-queue-from-isr
make
make flash-stlink
```

`make clean` chỉ xóa `build/` của thư mục hiện tại. Root Makefile không có `lab01`, `all-labs` hoặc `run-labs`.

---

## 9. Danh sách bài thực hành

| Bài | Chủ đề | Môi trường | Kết quả chính |
|---:|---|---|---|
| 1 | Static Ring-buffer Queue | Host | Kiểm tra FIFO, wraparound, full/empty và item copy |
| 2 | Blocking Queue | Host | Mô phỏng waiter, no-wait, timeout và direct handoff |
| 3 | Queue from ISR | STM32F103 | EXTI0 gửi message từ ISR và defer PendSV |
| 4 | Message Pool | Host | Quản lý fixed blocks, exhaustion và double-free detection |
| 5 | Software Timer List | Host | Kiểm tra one-shot, periodic, ordering và tick wrap |
| 6 | Timer Service Task | STM32F103 | Chạy callback trong task context thay vì SysTick ISR |
| 7 | Timestamp Backends | STM32F103 | So sánh DWT, GPIO marker và overhead đo thời gian |
| 8 | Context-switch Benchmark | STM32F103 | Đo yield/switch latency và thống kê min/max/average |
| 9 | Event and Message Latency | STM32F103 | Đo ISR-to-task latency và queue message latency |
| 10 | Communication Benchmark Console | STM32F103 | Tích hợp queue, pool, timer, benchmark và UART console |

---

## 10. Quy trình học đề xuất

```text
Đọc README lab
      |
      v
Build và chạy
      |
      v
Quan sát test/UART/GDB/logic analyzer
      |
      v
Ghi điều kiện đo
      |
      v
Tạo lỗi có chủ đích
      |
      v
Giải thích ownership hoặc timing invariant bị phá
```

---

## 11. Ghi chú kỹ thuật

### Queue

Queue dùng storage tĩnh, fixed item size, sender wait list và receiver wait list. Finite waiter đồng thời dùng `wait_node` và `timeout_node` trong TCB.

### Message pool

Pool chia vùng nhớ thành block kích thước cố định. Debug allocation map phát hiện pointer ngoài pool và double free.

### Software timer

SysTick chỉ phát signal. Timer service task lấy timer tới hạn và gọi callback ngoài critical section. Periodic timer lên lịch theo expiry trước đó để giảm drift.

### Benchmark

DWT `CYCCNT` cung cấp cycle timestamp. Kết quả phải ghi CPU clock, compiler, optimization, task priority, message size, queue capacity, sample count và timestamp overhead. Maximum quan sát được không phải WCET đã được chứng minh.
