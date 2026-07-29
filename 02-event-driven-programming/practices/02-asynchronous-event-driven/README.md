# 02 - Asynchronous Event-Driven Programming

Repository thực hành cho **Chủ đề 2: Lập trình bất đồng bộ và hướng sự kiện**:

- Chuyển firmware từ blocking sang non-blocking.
- Event, Signal, Event Queue và Mailbox.
- Dispatcher và Event Handler.
- Software Timer và Timer Event.
- Deferred Processing từ ISR sang event context.
- Button debounce không blocking.
- LED State Machine.
- UART command parser.
- Event Trace, queue statistics và stress test.
- Product State Machine và so sánh với super-loop.

Project được tổ chức theo cùng khuôn mẫu:

- Phần root là firmware Event-Driven tổng kết chạy trên STM32F103C8T6.
- `labs/` chứa 10 bài thực hành độc lập.
- `tests/` chứa host unit test cho framework và State Machine.
- `docs/` chứa kiến trúc, event list, sequence diagram và state diagram.
- `build/` chỉ chứa artifact sinh tự động.
- Makefile root chỉ quản lý firmware tổng kết và unit test root.
- Mỗi lab có Makefile riêng và không được build từ root.

Cấu hình mặc định:

- Board: STM32F103C8T6 Blue Pill.
- CPU: ARM Cortex-M3.
- Clock: HSI 8 MHz.
- LED: PC13, active-low.
- Button ngoài: PA0, active-low, pull-up nội, EXTI0 hai cạnh.
- UART: USART1 PA9/PA10, 9600 baud, 8-N-1.
- Event Queue: 32 event.
- Event Trace: 128 record.
- Phần platform dùng register-level để giữ framework độc lập với HAL/SPL.

---

## 1. Mục tiêu

Sau khi hoàn thành repository này, người học có thể:

- Phân biệt synchronous, asynchronous, polling, interrupt và Event-Driven.
- Phân tích nhược điểm của blocking delay và super-loop lớn.
- Thiết kế `event_t`, signal list và destination list.
- Viết Circular Event Queue bằng bộ nhớ tĩnh.
- Xử lý queue full, wraparound và high-water mark.
- Viết Dispatcher route event theo destination.
- Viết Event Handler theo nguyên tắc run-to-completion.
- Thay delay bằng Software Timer và timeout event.
- Chuyển xử lý nặng ra khỏi ISR.
- Viết Button debounce bằng EXTI và Timer Event.
- Viết LED State Machine OFF/ON/BLINKING.
- Viết Product State Machine BOOT/IDLE/ACTIVE/ERROR.
- Parse lệnh UART rồi chuyển thành event.
- Ghi Event Trace CSV và theo dõi queue statistics.
- Unit test queue, timer và State Machine trên host.
- Tích hợp framework lên STM32F103.

---

## 2. Cấu trúc repository

```text
02-asynchronous-event-driven/
├── README.md
├── Makefile
├── .gitignore
├── LICENSE
├── VALIDATION.md
├── include/
│   ├── app.h
│   ├── button.h
│   ├── compiler.h
│   ├── dispatcher.h
│   ├── event.h
│   ├── event_queue.h
│   ├── event_trace.h
│   ├── led.h
│   ├── platform.h
│   ├── software_timer.h
│   ├── stm32f1.h
│   └── uart_service.h
├── src/
│   ├── app.c
│   ├── button.c
│   ├── dispatcher.c
│   ├── event_queue.c
│   ├── event_trace.c
│   ├── led.c
│   ├── main.c
│   ├── runtime.c
│   ├── software_timer.c
│   └── uart_service.c
├── platform/
│   ├── critical_section.c
│   ├── gpio.c
│   ├── platform.c
│   ├── systick.c
│   └── uart.c
├── startup/
│   └── startup.c
├── linker/
│   └── memory.ld
├── tests/
│   ├── fakes.c
│   ├── test.h
│   ├── test_app_sm.c
│   ├── test_button_sm.c
│   ├── test_event_queue.c
│   ├── test_led_sm.c
│   ├── test_main.c
│   ├── test_support.c
│   └── test_timer.c
├── labs/
│   ├── README.md
│   ├── 01-blocking-to-nonblocking/
│   ├── 02-event-queue/
│   ├── 03-dispatcher/
│   ├── 04-button-debounce/
│   ├── 05-led-state-machine/
│   ├── 06-uart-command-parser/
│   ├── 07-event-trace/
│   ├── 08-queue-stress-test/
│   ├── 09-product-state-machine/
│   └── 10-superloop-vs-event-driven/
├── docs/
│   ├── architecture.md
│   ├── event-list.md
│   ├── sequence-diagrams.md
│   └── state-machines.md
└── build/
```

---

## 3. Firmware root làm gì?

```text
SysTick / EXTI0 / USART1 IRQ
            |
            v
          Event
            |
            v
     Circular Event Queue
            |
            v
        Dispatcher
   +--------+--------+---------+
   |        |        |         |
   v        v        v         v
Button SM  App SM   LED SM   UART Service
   |        |        |         |
   +--------+--------+---------+
            |
            v
       Event Trace
```

Luồng demo:

1. EXTI0 ISR chỉ post `BUTTON_EDGE`.
2. Button handler arm timer debounce 20 ms.
3. Timeout handler đọc PA0 và post `BUTTON_PRESSED` hoặc `BUTTON_RELEASED`.
4. App chuyển `IDLE <-> ACTIVE`.
5. App post `LED_BLINK_START` hoặc `LED_OFF`.
6. LED State Machine dùng periodic timer để toggle PC13.
7. USART1 ISR post từng byte cho UART Service.
8. UART Service parse lệnh và post event.
9. Dispatcher ghi trace cho post, dispatch, complete và drop.

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

Công cụ tùy chọn:

```bash
sudo apt install \
    openocd \
    stlink-tools \
    gdb-multiarch \
    picocom
```

Kiểm tra:

```bash
arm-none-eabi-gcc --version
gcc --version
make --version
```

Có thể build target bằng Clang/LLD:

```bash
make TOOLCHAIN=clang
```

---

## 5. Build firmware tổng kết

```bash
make
```

Hoặc:

```bash
make TOOLCHAIN=clang
```

Artifact:

```text
build/asynchronous_event_driven.elf
build/asynchronous_event_driven.bin
build/asynchronous_event_driven.hex
build/asynchronous_event_driven.map
build/asynchronous_event_driven.lst
```

Unit test root:

```bash
make test
```

Các lệnh khác:

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

Kết nối button ngoài:

```text
PA0 ---- Button ---- GND
```

PA0 dùng pull-up nội, nên:

```text
Released -> logic 1
Pressed  -> logic 0
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

Command:

```text
help
led on
led off
blink 500
blink stop
status
trace
reset
error
```

`trace` in CSV:

```text
timestamp,source,destination,signal,parameter,phase
```

---

## 8. Build các lab

Makefile root không build lab. Mỗi lab là một build unit độc lập.

Ví dụ host lab:

```bash
cd labs/02-event-queue
make test
make run
```

Ví dụ target lab:

```bash
cd labs/04-button-debounce
make
make flash-stlink
```

Quy ước:

- Host labs: 01, 02, 03, 05, 06, 07, 08, 09, 10.
- Target lab: 04.
- Output nằm trong `build/` của chính lab.
- `make clean` chỉ xóa output của lab hiện tại.
- Không dùng `make lab04` tại root.
- Muốn flash Lab 04 phải đứng trong `labs/04-button-debounce`.

---

## 9. Danh sách bài thực hành

| Bài | Chủ đề | Môi trường | Kết quả chính |
|---:|---|---|---|
| 1 | Blocking sang non-blocking | Host | So sánh latency |
| 2 | Circular Event Queue | Host | FIFO, full, wraparound |
| 3 | Dispatcher | Host | Route ba module và invalid event |
| 4 | Button debounce | STM32F103 | EXTI -> timer -> pressed/released |
| 5 | LED State Machine | Host | OFF/ON/BLINKING |
| 6 | UART command parser | Host | `led on`, `led off`, `blink`, `status` |
| 7 | Event Trace | Host | CSV ring buffer |
| 8 | Queue stress test | Host | Post, dispatch, drop, high-water mark |
| 9 | Product State Machine | Host | BOOT/IDLE/ACTIVE/ERROR |
| 10 | So sánh kiến trúc | Host | Super-loop và Event-Driven |

---

## 10. Quy trình học đề xuất

```text
Đọc README của lab
      |
      v
Xác định event source, signal và owner
      |
      v
Build trong đúng thư mục lab
      |
      v
Chạy demo hoặc unit test
      |
      v
Quan sát event sequence và state transition
      |
      v
Tạo một trường hợp queue full / invalid event / timeout
      |
      v
Giải thích nguyên nhân và failure policy
      |
      v
Tích hợp khái niệm vào firmware root
```

---

## 11. Ghi chú kỹ thuật

### Run-to-completion

Handler không được dùng delay blocking hoặc vòng chờ dài.

### Event ownership

`event_t` được copy vào queue. Không truyền pointer tới biến local qua event.

### Critical section

Queue được bảo vệ ở lớp Dispatcher vì producer có thể là ISR còn consumer là main loop.

### Timer

Software Timer dùng phép so sánh signed-difference để hỗ trợ tick wraparound.

### Platform

Framework không phụ thuộc thanh ghi. Chỉ thư mục `platform/` biết GPIO, UART, SysTick và PRIMASK.

### SPL

Platform hiện dùng register-level để repository tự chứa hoàn toàn. Có thể thay `platform/*.c` bằng STM32 SPL mà không thay đổi Event Queue, Dispatcher, Timer hoặc State Machine.
