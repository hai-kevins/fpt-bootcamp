# 05 - Embedded Test & Debug

Repository thực hành cho **Chủ đề 5: Kỹ thuật Test và Debug trong Embedded Programming**:

- Design for Testability và Dependency Injection trong C.
- Stub, Fake, Spy và Mock.
- Unit Test trên host và Test Runner tối thiểu.
- Event Queue, Event Pool, Software Timer và State Machine tests.
- UART-style Command Line để inject event và query trạng thái.
- Binary Event Trace bằng ring buffer.
- Mailbox/Event Pool statistics và high-water mark.
- Crash Record, reset reason và checksum.
- Fault Injection, automated use-case test, stress test và HIL workflow.

Project được tổ chức theo cùng một bố cục:

- Phần root là demo Test/Debug tổng kết cuối chủ đề.
- `labs/` chứa từng bài thực hành độc lập.
- `docs/` chứa phần giải thích sâu hơn.
- `tools/` chứa script validation và công cụ phân tích.
- `build/` chỉ chứa artifact sinh tự động.
- Makefile gốc chỉ quản lý demo và test tổng kết.
- Mỗi lab có Makefile riêng và không được build từ root.

Cấu hình mặc định:

- Môi trường: Linux host simulation.
- Ngôn ngữ: C11.
- Compiler: GCC hoặc Clang native.
- Build policy: `-Wall -Wextra -Werror -Wpedantic`.
- Unit Test: framework tối thiểu tự viết.
- Memory: fixed-size queue và Event Pool, không dùng heap trong framework.
- Time: fake monotonic time cho timer test.
- Validation: AddressSanitizer và UndefinedBehaviorSanitizer.
- Target integration: UART shell, crash record và HIL được mô phỏng bằng interface có thể port sang STM32.

---

## 1. Mục tiêu

Sau khi hoàn thành repository này, người học có thể:

- Thiết kế module có khả năng kiểm thử ngay từ đầu.
- Tách application logic khỏi hardware dependency.
- Phân biệt Stub, Fake, Spy và Mock.
- Viết Test Runner và assertion macro tối thiểu.
- Viết Unit Test cho Event Queue và overflow policy.
- Viết Unit Test cho Event Pool, exhaustion và reference count.
- Dùng fake time để test one-shot và periodic timer.
- Test State Machine theo mẫu Given-When-Then.
- Dùng UART-style shell để inject event và query state/statistics.
- Thiết kế Binary Event Trace có kích thước record cố định.
- Theo dõi queue depth, pool usage và high-water mark.
- Tạo Crash Record có magic, version, build ID và checksum.
- Chủ động tạo lỗi bằng Fault Injection.
- Tự động hóa product use case bằng event sequence.
- Viết stress test, soak-test checklist và regression test.
- Mô tả workflow CI và Hardware-in-the-Loop.

---

## 2. Cấu trúc repository

```text
05-embedded-test-debug/
├── README.md
├── Makefile
├── .gitignore
├── VALIDATION.md
├── include/
│   ├── app_sm.h
│   ├── crash_record.h
│   ├── event.h
│   ├── event_pool.h
│   ├── event_queue.h
│   ├── event_trace.h
│   ├── fake_time.h
│   ├── fault_injection.h
│   ├── shell.h
│   ├── software_timer.h
│   ├── statistics.h
│   ├── test_assert.h
│   └── test_runner.h
├── src/
│   ├── app_sm.c
│   ├── crash_record.c
│   ├── event_pool.c
│   ├── event_queue.c
│   ├── event_trace.c
│   ├── fake_time.c
│   ├── fault_injection.c
│   ├── main.c
│   ├── shell.c
│   ├── software_timer.c
│   ├── statistics.c
│   └── test_runner.c
├── tests/
│   ├── test_all.c
│   ├── test_app_sm.c
│   ├── test_cases.h
│   ├── test_crash_record.c
│   ├── test_event_pool.c
│   ├── test_event_queue.c
│   ├── test_fault_injection.c
│   ├── test_shell.c
│   ├── test_timer.c
│   └── test_trace.c
├── labs/
│   ├── README.md
│   ├── 01-unit-test-event-queue/
│   ├── 02-unit-test-event-pool/
│   ├── 03-unit-test-state-machine/
│   ├── 04-event-spy/
│   ├── 05-fake-timer/
│   ├── 06-uart-shell/
│   ├── 07-binary-event-trace/
│   ├── 08-queue-latency/
│   ├── 09-handler-execution-time/
│   ├── 10-crash-record/
│   ├── 11-hardfault-record/
│   ├── 12-automated-use-case/
│   ├── 13-fault-injection/
│   ├── 14-regression-test/
│   ├── 15-stress-test/
│   ├── 16-soak-test/
│   ├── 17-ci/
│   └── 18-hardware-in-the-loop/
├── docs/
│   ├── architecture.md
│   ├── ci-hil.md
│   ├── crash-record.md
│   ├── test-strategy.md
│   ├── trace-format.md
│   └── uart-shell.md
├── tools/
│   ├── decode_trace.py
│   ├── run_hil_tests.py
│   └── validate.sh
└── build/
```

---

## 3. Project root làm gì?

Demo root mô phỏng vòng đời Test/Debug của một application Event-Driven:

```text
UART-style Shell
       |
       v
Inject APP_START event
       |
       v
Event Queue
       |
       v
Application State Machine
       |
       v
RUNNING
       |
       v
Fake Time + Software Timer
       |
       v
APP_TIMEOUT event
       |
       v
ERROR
       |
       +--> Binary Event Trace
       +--> Queue/Pool Statistics
       +--> Crash Record + Checksum
```

Demo đồng thời:

- Allocate và release một Dynamic Event.
- Ghi event dispatch vào trace ring buffer.
- Tạo Crash Record hợp lệ.
- In state, transition count, timer expiry và statistics.

---

## 4. Cài toolchain

Ubuntu/Debian:

```bash
sudo apt update
sudo apt install \
    gcc \
    make \
    gdb \
    valgrind \
    python3
```

Có thể dùng Clang:

```bash
sudo apt install clang lldb
```

Kiểm tra:

```bash
gcc --version
make --version
python3 --version
```

ASan/UBSan được bật bằng compiler flag và không cần test framework ngoài.

---

## 5. Build project tổng kết

Từ thư mục root:

```bash
make
```

Lệnh này build demo và chạy 12 root test.

Artifact:

```text
build/embedded_test_debug_demo
build/test_runner
```

Các lệnh thường dùng:

```bash
make demo      # Chỉ build demo
make run       # Chạy demo tổng kết
make test      # Chạy 12 root tests
make sanitize  # Chạy test với ASan/UBSan
make list-labs # Liệt kê 18 lab
make validate  # Chạy root và toàn bộ lab
make clean     # Chỉ xóa build/ ở root
```

`make clean` tại root không xóa `build/` bên trong từng lab.

---

## 6. Chạy demo và test

Chạy demo:

```bash
make run
```

Output dự kiến:

```text
Embedded Test/Debug demo
state=ERROR transitions=2
timer_expiry=1 trace=2
queue=0 hwm=1 overflow=0 pool=0 pool_hwm=1 pool_fail=0
crash_record=VALID
```

Chạy unit test:

```bash
make test
```

Kết quả:

```text
05-embedded-test-debug tests
[PASS] event_queue_fifo
...
Summary: 12/12 PASS
```

Chạy sanitizer:

```bash
make sanitize
```

---

## 7. Build các lab

Makefile root **không build lab**. Mỗi lab được build trong thư mục riêng.

Ví dụ:

```bash
cd labs/06-fake-time-timer-tests
make
make run
make sanitize
```

Quay lại root:

```bash
cd ../..
```

Quy ước:

- Tất cả 18 lab chạy trên host Linux.
- Mỗi lab tạo executable `build/lab`.
- `make run` chỉ chạy lab hiện tại.
- `make test` là alias của `make run`.
- `make sanitize` chạy ASan/UBSan.
- `make clean` chỉ xóa output của lab hiện tại.
- Không có target `make labXX` ở root.

---

## 8. Danh sách bài thực hành

| Bài | Chủ đề | Kết quả chính |
|---:|---|---|
| 01 | Unit Test Event Queue | Empty, FIFO, Full, Wraparound và Overflow |
| 02 | Unit Test Event Pool | Allocate, Exhaustion, Free, Reuse và Double Free |
| 03 | Unit Test State Machine | Test mọi transition của IDLE/RUNNING/PAUSED/ERROR |
| 04 | Event Spy | Kiểm tra handler post đúng signal |
| 05 | Fake Timer | One-shot và periodic timer không dùng thời gian thật |
| 06 | UART Shell | help, state, pool, mailbox, event và test |
| 07 | Binary Event Trace | Ring buffer 128 record, dump/clear/stats |
| 08 | Queue Latency | Min, max và average từ post tới dispatch |
| 09 | Handler Execution Time | Phát hiện handler vượt ngưỡng |
| 10 | Crash Record | Fatal, task, signal, state, uptime và reset reason |
| 11 | HardFault Record | PC, LR, xPSR và Fault Status Register |
| 12 | Automated Use Case | Boot, Button, Mode, Timeout, Error và Recovery |
| 13 | Fault Injection | Pool, CRC, Sensor và Flash failure |
| 14 | Regression Test | Tái hiện bug cũ và xác nhận fix |
| 15 | Stress Test | Event burst, drop, queue max, pool max và handler max |
| 16 | Soak Test | Mô phỏng chạy 12 giờ và statistics định kỳ |
| 17 | Continuous Integration | Build, Test, Sanitizer, Firmware, Size và Artifact |
| 18 | Hardware-in-the-Loop | Reset board, gửi command, đọc result và report |

---

## 9. Quy trình học đề xuất

```text
Đọc README của lab
      |
      v
Xác định requirement và expected result
      |
      v
Đọc source và test oracle
      |
      v
Build trong đúng thư mục lab
      |
      v
make run
      |
      v
make sanitize
      |
      v
Tạo một lỗi có chủ đích
      |
      v
Xác nhận test fail đúng nguyên nhân
      |
      v
Sửa code và chạy regression
      |
      v
Chuyển sang lab tiếp theo
```

Mỗi bug thực tế nên được chuyển thành một regression test trước hoặc cùng lúc với fix.

---

## 10. Nguyên tắc Test/Debug quan trọng

### Design for Testability

Application logic không nên truy cập thanh ghi trực tiếp. Hardware dependency nên đi qua port interface hoặc callback để có thể thay bằng fake trong test.

### Deterministic Test

Test không nên phụ thuộc `sleep()` hoặc thời gian thật nếu có thể dùng fake monotonic time.

### Cleanup Invariant

Sau mỗi test:

```text
queue.count == 0
event_pool.used_count == 0
no timer pending ngoài expected
fault injection đã được clear
```

### Event Trace

Binary record nên có kích thước cố định và sử dụng ID thay vì pointer. Ring buffer giữ event mới nhất hữu ích cho crash investigation.

### Crash Record

Record nên có:

- Magic.
- Version.
- Size.
- Build ID.
- Reset reason.
- Fatal code.
- State và signal cuối.
- Statistics.
- Checksum hoặc CRC.

### Fault Injection

Fault chỉ được bật trong test/debug build và phải có cơ chế clear rõ ràng.

---

## 11. Ghi chú kỹ thuật

### Host và target

Repository kiểm tra logic trên host. Khi port lên STM32, UART transport, reset reason, HardFault register và vùng `.noinit` cần platform adapter riêng.

### Unit Test không thay thế HIL

Host test bắt lỗi logic nhanh nhưng không chứng minh interrupt timing, DMA, power failure, clock, pin mux hoặc peripheral thật hoạt động đúng.

### Sanitizer

ASan/UBSan có giá trị cao trên host nhưng không mô phỏng đầy đủ Cortex-M memory map và exception behavior.

### Logging overhead

Text log có thể thay đổi timing. Event tốc độ cao nên dùng Binary Trace và dump bất đồng bộ.

### Stress và Soak

Stress test kiểm tra tải cao trong thời gian ngắn. Soak test cần chạy nhiều giờ/ngày và theo dõi counter theo thời gian để phát hiện leak hoặc drift.
