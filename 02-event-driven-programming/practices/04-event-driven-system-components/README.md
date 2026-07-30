# 04 - Event-Driven System Components

Repository thực hành cho **Chủ đề 4: Các thành phần chính của hệ thống Event-Driven**:

- Active Object, Mailbox và Run-to-Completion Scheduler.
- Flat State Machine, Table-Driven State Machine và Hierarchical State Machine.
- Static Event Pool, ownership và reference count.
- Router, Direct Post và Publish-Subscribe.
- Frame serialization, CRC-16 và Parser State Machine.
- Data-Link timeout, retry và link failure.
- Event Trace, queue statistics, pool statistics và stress test.

Project được tổ chức theo cùng một bố cục:

- Phần root là demo tích hợp và bộ unit test tổng kết.
- `labs/` chứa từng bài thực hành độc lập.
- `docs/` chứa phần giải thích sâu hơn.
- `build/` chỉ chứa artifact sinh tự động.
- Makefile gốc chỉ quản lý demo và test tổng kết.
- Mỗi lab có Makefile riêng và không được build từ root.

Cấu hình mặc định:

- Môi trường: Linux host simulation.
- Ngôn ngữ: C11.
- Compiler: GCC hoặc Clang native.
- Build policy: `-Wall -Wextra -Werror -Wpedantic`.
- Dynamic event: fixed-size static Event Pool, không dùng heap hệ thống.
- Scheduler: cooperative, run-to-completion, chọn Active Object theo priority.
- Kiểm tra runtime: AddressSanitizer và UndefinedBehaviorSanitizer.

Repository tập trung vào kiến trúc và hành vi deterministic trên host. Bản này không triển khai CAN transport vật lý, hệ thống nhiều MCU hoặc mini game phân tán.

---

## 1. Mục tiêu

Sau khi hoàn thành repository này, người học có thể:

- Giải thích Active Object gồm mailbox, priority, handler và context riêng.
- Cài đặt mailbox FIFO có capacity, overflow policy và high-water mark.
- Giải thích run-to-completion và tác động của handler blocking.
- Cài scheduler cooperative chọn Active Object ready theo priority.
- Viết Flat FSM, Table-Driven FSM và HSM có event bubbling.
- Cài static Event Pool có allocation statistics.
- Quản lý ownership bằng retain/release và reference count.
- Route event local bằng destination node và destination object.
- Publish một signal tới nhiều subscriber.
- Serialize event thành frame có header, sequence, payload và CRC-16.
- Cài parser nhận từng byte và resynchronize sau dữ liệu không hợp lệ.
- Cài timeout, retry limit và trạng thái link down.
- Ghi Event Trace bằng ring buffer giữ các record mới nhất.
- Viết unit test, negative test, stress test và sanitizer test.

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
├── tools/
│   └── validate.sh
└── build/
```

---

## 3. Project root làm gì?

Root project là demo tích hợp chạy trên host:

```text
Static sensor event
       |
       v
Publish-Subscribe
       |
       +--------------------+
       |                    |
       v                    v
Display AO mailbox    Diagnostic AO mailbox
       |                    |
       +---------+----------+
                 |
                 v
        Priority Scheduler
                 |
                 v
      Run-to-Completion handlers
                 |
                 v
        Release dynamic events
```

Demo kiểm tra:

- Hai Active Object subscribe cùng một signal.
- PubSub tạo bản sao dynamic riêng cho từng subscriber.
- Scheduler dispatch đủ hai event.
- Event Pool trở về `used_count == 0` sau khi xử lý.

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

Clang là lựa chọn thay thế:

```bash
sudo apt install clang lldb
```

Kiểm tra:

```bash
gcc --version
make --version
gdb --version
```

AddressSanitizer và UndefinedBehaviorSanitizer được bật bằng compiler flag, không cần thư viện test riêng.

---

## 5. Build project tổng kết

Từ thư mục root:

```bash
make
```

Lệnh trên build demo và chạy bộ unit test root.

Các lệnh thường dùng:

```bash
make demo      # Chỉ build demo tích hợp
make run       # Build rồi chạy demo
make test      # Build rồi chạy 9 unit test
make sanitize  # Chạy unit test với ASan/UBSan
make list-labs # In danh sách 15 lab liên tục
make clean     # Chỉ xóa build/ ở root
```

Artifact root:

```text
build/event_driven_demo
build/test_runner
build/test_runner_sanitize
```

---

## 6. Build và chạy từng lab

Makefile root không build lab. Mỗi lab được build trong thư mục riêng:

```bash
cd labs/02-mailbox
make
make run
make sanitize
make clean
```

Quy ước:

- `make`: build executable của lab.
- `make run`: build rồi chạy demo/test của lab.
- `make test`: alias của `make run`.
- `make sanitize`: build và chạy với ASan/UBSan.
- `make clean`: chỉ xóa `build/` của lab hiện tại.

Không chạy `make run` ở root khi mục tiêu là chạy một lab cụ thể.

---

## 7. Danh sách bài thực hành

| Bài | Chủ đề | Môi trường | Thư mục |
|---:|---|---|---|
| 01 | Active Object | Host | `labs/01-active-object` |
| 02 | Mailbox FIFO và overflow | Host | `labs/02-mailbox` |
| 03 | Run-to-Completion Scheduler | Host | `labs/03-run-to-completion-scheduler` |
| 04 | Flat State Machine | Host | `labs/04-flat-state-machine` |
| 05 | Hierarchical State Machine | Host | `labs/05-hierarchical-state-machine` |
| 06 | Table-Driven State Machine | Host | `labs/06-table-driven-state-machine` |
| 07 | Static Event Pool | Host | `labs/07-event-pool` |
| 08 | Reference Count và Ownership | Host | `labs/08-reference-count-ownership` |
| 09 | Router và Direct Post | Host | `labs/09-router-direct-post` |
| 10 | Publish-Subscribe | Host | `labs/10-publish-subscribe` |
| 11 | UART-style Data-Link Frame | Host | `labs/11-uart-datalink` |
| 12 | Parser State Machine | Host | `labs/12-parser-state-machine` |
| 13 | Link Failure, Timeout và Retry | Host | `labs/13-link-failure` |
| 14 | Event Trace Ring Buffer | Host | `labs/14-event-trace` |
| 15 | Mailbox và Event Pool Stress Test | Host | `labs/15-stress-test` |

Thứ tự lab hiện liên tục từ `01` đến `15`.

---

## 8. Quy trình học đề xuất

```text
Đọc README của lab
      |
      v
Xác định component và invariant
      |
      v
Đọc main.c và source framework liên quan
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
Giải thích counter/state/ownership bị sai
      |
      v
Khôi phục code và chuyển lab tiếp theo
```

Nên học theo thứ tự vì các lab sau sử dụng khái niệm của lab trước:

```text
Active Object -> Mailbox -> Scheduler -> State Machine
      -> Event Pool -> Ownership -> Router/PubSub
      -> Data-Link -> Parser -> Retry -> Trace -> Stress
```

---

## 9. Kiến trúc và invariant quan trọng

### Active Object

Mỗi Active Object sở hữu:

- ID và priority.
- Mailbox riêng.
- Event handler.
- Context/state riêng.

Module khác không được sửa trực tiếp state nội bộ của Active Object.

### Run-to-Completion

```text
Dequeue -> Handler start -> Action/transition/post -> Handler return
```

Handler không được busy-wait dài, delay blocking hoặc tự post vô hạn.

### Dynamic Event Ownership

```text
Allocate -> owner giữ một reference
Post success -> mailbox sở hữu reference
Dispatch -> handler chỉ mượn event
Handler return -> scheduler release
```

Invariant quan trọng sau khi hệ thống idle:

```text
event_pool.used_count == 0
```

### Mailbox và Pool Statistics

Cần theo dõi ít nhất:

- Current count.
- High-water mark.
- Overflow/allocation failure count.
- Post/dispatch/allocation/release count.

### Data-Link

```text
Event -> Frame -> Serialize -> CRC -> Byte stream
Byte stream -> Parser -> CRC check -> Frame -> Event
```

Parser phải kiểm tra length, version và CRC trước khi trả frame hợp lệ.

---

## 10. Kiểm thử và validation

Root test suite kiểm tra:

- Mailbox FIFO và overflow.
- Event Pool retain/release.
- Scheduler priority.
- Flat FSM.
- HSM bubbling.
- Publish-Subscribe.
- Serializer, CRC và Parser.
- Link timeout/retry.
- Event Trace wraparound.

Chạy toàn bộ validation:

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

Phạm vi validation là host Linux. Kết quả không thay thế kiểm thử trên UART vật lý, CAN hardware hoặc hệ thống nhiều node.

---

## 11. Tiêu chí hoàn thành

- [ ] Giải thích được Active Object, Mailbox và run-to-completion.
- [ ] Mailbox giữ đúng FIFO và có overflow policy.
- [ ] Scheduler dispatch priority cao trước priority thấp.
- [ ] Flat FSM và Table-Driven FSM chuyển state đúng.
- [ ] HSM bubble event từ leaf lên parent đúng.
- [ ] Event Pool phát hiện exhaustion và trở về `used_count == 0`.
- [ ] Reference count không underflow và không leak event.
- [ ] Router direct post đúng destination object.
- [ ] Publish-Subscribe giao event tới đủ subscriber.
- [ ] Frame encode/decode và CRC kiểm tra đúng.
- [ ] Parser nhận từng byte và tạo frame hợp lệ.
- [ ] Retry đạt giới hạn thì link chuyển sang DOWN.
- [ ] Event Trace giữ record mới nhất khi wraparound.
- [ ] Stress test kết thúc với `posted == handled`.
- [ ] Root tests PASS.
- [ ] Tất cả 15 lab PASS.
- [ ] Không có lỗi ASan/UBSan.
