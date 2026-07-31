# 04 - Event-Driven System Components

Repository thực hành cho **Chủ đề 4: Các thành phần chính của hệ thống Event-Driven**:

- Active Object, Mailbox và Run-to-Completion Scheduler.
- Flat State Machine, Table-Driven State Machine và Hierarchical State Machine.
- Static Event Pool, ownership và reference count.
- Router, Direct Post và Publish-Subscribe.
- Data-Link frame, serialization, CRC-16 và Parser State Machine.
- Timeout, retry, link failure và Event Trace.
- Unit Test, sanitizer test và stress test trên host Linux.

Project được tổ chức theo cùng khuôn mẫu của `01-embedded-foundation`:

- Phần root là project tích hợp cuối chủ đề.
- `labs/` chứa từng bài thực hành độc lập.
- `docs/` chứa phần giải thích kiến trúc và thiết kế.
- `build/` chỉ chứa artifact sinh tự động.
- Makefile root chỉ quản lý demo và test tổng kết.
- Mỗi lab có Makefile riêng và được build trong thư mục của chính lab.

Cấu hình mặc định:

- Môi trường: Linux host simulation.
- Ngôn ngữ: C11.
- Compiler: GCC hoặc Clang native.
- Build policy: `-Wall -Wextra -Werror -Wpedantic`.
- Scheduler: cooperative và run-to-completion.
- Dynamic Event: fixed-size static Event Pool, không dùng heap hệ thống.
- Validation: AddressSanitizer và UndefinedBehaviorSanitizer.
- Phạm vi: không triển khai CAN transport vật lý, Multi-MCU hoặc distributed game.

---

## 1. Mục tiêu

Sau khi hoàn thành repository này, người học có thể:

- Giải thích Active Object gồm ID, priority, mailbox, handler và context riêng.
- Cài đặt Mailbox FIFO có capacity, overflow counter và high-water mark.
- Giải thích nguyên tắc run-to-completion và tác động của handler blocking.
- Cài đặt cooperative scheduler chọn Active Object ready theo priority.
- Viết Flat FSM, Table-Driven FSM và HSM có event bubbling.
- Cài đặt Static Event Pool có allocation statistics.
- Quản lý Dynamic Event bằng ownership, retain/release và reference count.
- Route event trực tiếp tới destination object.
- Publish một signal tới nhiều subscriber mà producer không biết consumer cụ thể.
- Serialize event thành frame có addressing, sequence, length, payload và CRC-16.
- Cài parser nhận từng byte và resynchronize sau dữ liệu không hợp lệ.
- Cài timeout, retry limit và trạng thái link down.
- Ghi Event Trace bằng ring buffer giữ các record mới nhất.
- Viết unit test, negative test, stress test và sanitizer test.
- Phân tích queue depth, pool usage, overflow và ownership invariant.

---

## 2. Cấu trúc repository

```text
04-event-driven-system-components/
├── README.md
├── Makefile
├── .gitignore
├── VALIDATION.md
├── framework/
│   ├── include/
│   │   ├── active_object.h
│   │   ├── event.h
│   │   ├── event_pool.h
│   │   ├── event_trace.h
│   │   ├── fsm.h
│   │   ├── hsm.h
│   │   ├── mailbox.h
│   │   ├── pubsub.h
│   │   ├── router.h
│   │   └── scheduler.h
│   └── src/
├── datalink/
│   ├── include/
│   │   ├── crc16.h
│   │   ├── datalink.h
│   │   ├── frame.h
│   │   ├── parser.h
│   │   └── serializer.h
│   └── src/
├── application/
│   ├── include/
│   └── src/
├── tests/
│   ├── test_all.c
│   └── test_support.h
├── labs/
│   ├── README.md
│   ├── 01-active-object/
│   ├── 02-mailbox/
│   ├── 03-run-to-completion-scheduler/
│   ├── 04-flat-state-machine/
│   ├── 05-hierarchical-state-machine/
│   ├── 06-table-driven-state-machine/
│   ├── 07-event-pool/
│   ├── 08-reference-count-ownership/
│   ├── 09-router-direct-post/
│   ├── 10-publish-subscribe/
│   ├── 11-uart-datalink/
│   ├── 12-parser-state-machine/
│   ├── 13-link-failure/
│   ├── 14-event-trace/
│   └── 15-stress-test/
├── docs/
│   ├── active-objects.md
│   ├── architecture.md
│   ├── event-list.md
│   ├── failure-policy.md
│   ├── ownership-model.md
│   ├── pool-sizing.md
│   ├── routing-table.md
│   ├── sequence-diagrams.md
│   ├── state-machines.md
│   └── wire-protocol.md
├── tools/
│   └── validate.sh
└── build/
```

---

## 3. Project root làm gì?

Project root là demo tích hợp chạy trên host Linux:

```text
Static Sensor Event
        |
        v
Publish-Subscribe
        |
        +-----------------------+
        |                       |
        v                       v
Display AO Mailbox      Diagnostic AO Mailbox
        |                       |
        +-----------+-----------+
                    |
                    v
          Priority Scheduler
                    |
                    v
       Run-to-Completion Handler
                    |
                    v
          Release Dynamic Event
```

Demo thực hiện:

- Khởi tạo Event Pool.
- Khởi tạo hai Active Object.
- Đăng ký hai subscriber cho cùng một signal.
- Publish một static event.
- Tạo một Dynamic Event riêng cho mỗi subscriber.
- Post event vào hai mailbox.
- Scheduler dispatch đủ hai event.
- Release event sau khi handler return.
- Xác nhận Event Pool trở về `used_count == 0`.

Output dự kiến:

```text
Event-Driven components demo
delivered=2 dispatched=2
display handled=1 value=1234
diagnostic handled=1 value=1234
pool high-water=2 used=0
```

---

## 4. Cài toolchain

Ubuntu/Debian:

```bash
sudo apt update
sudo apt install \
    gcc \
    make \
    gdb \
    valgrind
```

Có thể dùng Clang:

```bash
sudo apt install \
    clang \
    lldb
```

Kiểm tra:

```bash
gcc --version
make --version
gdb --version
```

AddressSanitizer và UndefinedBehaviorSanitizer được bật bằng compiler flag, không cần framework test bên ngoài.

---

## 5. Build project tổng kết

Từ thư mục root:

```bash
make
```

Lệnh này:

1. Build demo tích hợp.
2. Build test runner.
3. Chạy toàn bộ 9 unit test root.

Artifact:

```text
build/event_driven_demo
build/test_runner
```

Các lệnh thường dùng:

```bash
make demo      # Chỉ build demo
make run       # Build rồi chạy demo
make test      # Build rồi chạy 9 unit test
make sanitize  # Chạy test với ASan/UBSan
make list-labs # In danh sách 15 lab
make clean     # Chỉ xóa build/ ở root
```

`make clean` tại root không xóa `build/` nằm trong từng lab.

---

## 6. Chạy và kiểm thử

Chạy demo tích hợp:

```bash
make run
```

Chạy unit test:

```bash
make test
```

Kết quả dự kiến:

```text
04-event-driven-system-components tests
[PASS] mailbox
[PASS] event_pool
[PASS] scheduler_priority
[PASS] fsm
[PASS] hsm
[PASS] pubsub
[PASS] serializer_parser
[PASS] datalink_retry
[PASS] event_trace
Summary: 9/9 PASS
```

Chạy sanitizer:

```bash
make sanitize
```

Chạy toàn bộ validation của repository:

```bash
./tools/validate.sh
```

Validation gồm:

```text
Root unit tests
Integrated root demo
Root ASan/UBSan
15 lab make run
15 lab make sanitize
```

---

## 7. Build các lab

Makefile ở thư mục root **không build lab**. Mỗi lab được build ngay trong thư mục của chính lab đó.

Ví dụ:

```bash
cd labs/02-mailbox
make
make run
make sanitize
```

Quay về root:

```bash
cd ../..
```

Quy ước:

- Tất cả 15 lab chạy trên host Linux.
- Mỗi lab tạo executable trong `build/` của chính lab đó.
- `make run` chỉ chạy lab hiện tại.
- `make test` là alias của `make run`.
- `make sanitize` build và chạy lab với ASan/UBSan.
- `make clean` chỉ xóa output của lab hiện tại.
- Không có target `make labXX` ở root.
- Không chạy `make run` ở root khi mục tiêu là chạy một lab cụ thể.

---

## 8. Danh sách bài thực hành

| Bài | Chủ đề | Môi trường | Kết quả chính |
|---:|---|---|---|
| 01 | Active Object | Host | Handler xử lý đúng một event |
| 02 | Mailbox FIFO và overflow | Host | FIFO, overflow và high-water mark |
| 03 | Run-to-Completion Scheduler | Host | Priority cao được dispatch trước |
| 04 | Flat State Machine | Host | Transition và action đúng |
| 05 | Hierarchical State Machine | Host | Event bubble lên parent |
| 06 | Table-Driven State Machine | Host | Chuỗi transition theo bảng |
| 07 | Static Event Pool | Host | Exhaustion deterministic và release toàn bộ |
| 08 | Reference Count và Ownership | Host | Block chỉ được trả khi ref-count về 0 |
| 09 | Router và Direct Post | Host | Event tới đúng destination object |
| 10 | Publish-Subscribe | Host | Hai subscriber nhận cùng signal |
| 11 | UART-style Data-Link Frame | Host | Encode/decode và CRC-16 |
| 12 | Parser State Machine | Host | Nhận từng byte và tạo frame hợp lệ |
| 13 | Link Failure, Timeout và Retry | Host | Retry đủ giới hạn rồi link DOWN |
| 14 | Event Trace Ring Buffer | Host | Giữ record mới nhất khi wraparound |
| 15 | Mailbox và Event Pool Stress Test | Host | Không leak event sau tải lớn |

---

## 9. Quy trình học đề xuất

```text
Đọc README của lab
      |
      v
Xác định component và invariant cần kiểm tra
      |
      v
Đọc main.c và source framework liên quan
      |
      v
Build trong đúng thư mục lab
      |
      v
make run
      |
      v
Đối chiếu output và exit code
      |
      v
make sanitize
      |
      v
Tạo một lỗi có chủ đích
      |
      v
Giải thích state/counter/ownership bị sai
      |
      v
Khôi phục code rồi chuyển lab tiếp theo
```

Nên học theo thứ tự:

```text
Active Object
      |
      v
Mailbox
      |
      v
Scheduler
      |
      v
State Machine
      |
      v
Event Pool và Ownership
      |
      v
Router và Publish-Subscribe
      |
      v
Data-Link và Parser
      |
      v
Retry, Trace và Stress Test
```

---

## 10. Kiến trúc và nguyên tắc quan trọng

### Active Object

Mỗi Active Object sở hữu:

- ID.
- Priority.
- Mailbox.
- Event handler.
- Context hoặc state riêng.

Module khác không được sửa trực tiếp state nội bộ của Active Object.

### Run-to-Completion

```text
Dequeue
   |
   v
Handler start
   |
   v
Action / Transition / Post Event
   |
   v
Handler return
```

Handler không được:

- Busy-wait dài.
- Delay blocking.
- Chờ I/O vô hạn.
- Tự post event vô hạn.

### Mailbox

Các invariant quan trọng:

```text
0 <= count <= capacity
head < capacity
tail < capacity
FIFO order được giữ
```

Cần theo dõi:

- Current count.
- High-water mark.
- Overflow count.
- Post count.
- Dispatch count.

### Dynamic Event Ownership

```text
Allocate
   |
   v
Owner giữ một reference
   |
   v
Post success
   |
   v
Mailbox sở hữu reference
   |
   v
Handler chỉ mượn event
   |
   v
Handler return
   |
   v
Scheduler release
```

Invariant sau khi hệ thống idle:

```text
event_pool.used_count == 0
```

### State Machine

Cần xác định rõ:

- Current state.
- Input signal.
- Guard.
- Action.
- Next state.
- Entry/exit action nếu có.
- Behavior khi event không được xử lý.

### Data-Link

```text
Event
  |
  v
Frame
  |
  v
Serialize + CRC
  |
  v
Byte Stream
  |
  v
Parser + CRC Check
  |
  v
Frame
  |
  v
Event
```

Parser phải kiểm tra length, version và CRC trước khi trả frame hợp lệ.

---

## 11. Ghi chú kỹ thuật

### Host simulation

Repository tập trung vào logic framework trên Linux. Nó không thay thế kiểm thử timing thật, ISR, DMA, UART vật lý hoặc CAN hardware.

### Heap

Dynamic Event không dùng `malloc()`. Event Pool dùng các block có kích thước cố định để allocation deterministic và tránh external fragmentation.

### Sanitizer

ASan/UBSan có thể phát hiện nhiều lỗi memory trên host, nhưng không mô phỏng đầy đủ memory map, alignment và interrupt behavior của Cortex-M.

### Priority

Giá trị priority chỉ có ý nghĩa khi tất cả Active Object sử dụng cùng một quy ước. Cần quy định rõ số lớn hơn hay số nhỏ hơn là priority cao hơn.

### Event fan-out

Publish-Subscribe hiện dùng copy-per-subscriber. Cách này làm ownership đơn giản nhưng Event Pool high-water mark tăng theo fan-out.

### Trace

Binary Event Trace phù hợp cho luồng tốc độ cao. Text log phù hợp cho thông tin cần đọc trực tiếp nhưng tốn nhiều CPU, Flash và bandwidth hơn.

### Validation

Kết quả PASS trên host chứng minh logic đã vượt qua test hiện có, không chứng minh hệ thống an toàn trong mọi tải, mọi compiler hoặc mọi platform.
