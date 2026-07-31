# Labs — Independent Makefiles

Mỗi thư mục lab là một build unit độc lập.

- Không build lab từ Makefile root.
- Đi vào đúng thư mục lab rồi chạy Makefile của lab đó.
- Output được tạo trong `build/` bên trong chính lab.
- `make clean` chỉ xóa output của lab hiện tại.
- `make sanitize` chạy AddressSanitizer và UndefinedBehaviorSanitizer.
- Tất cả 18 lab chạy trên host Linux.

Ví dụ:

```bash
cd labs/08-queue-latency
make
make run
make sanitize
```

Các lệnh chung:

```bash
make          # Build executable
make run      # Build rồi chạy
make test     # Alias của make run
make sanitize # Build và chạy với ASan/UBSan
make clean    # Chỉ xóa build/ của lab hiện tại
```

| Bài | Chủ đề | Output chính |
|---:|---|---|
| 01 | Unit Test Event Queue | `empty=PASS fifo=PASS full=PASS wrap=PASS overflow=1` |
| 02 | Unit Test Event Pool | `allocate=PASS exhaustion=PASS reuse=PASS double_free=PASS hwm=4` |
| 03 | Unit Test State Machine | `final=IDLE transitions=7 unhandled=1 PASS` |
| 04 | Event Spy | `calls=2 last_signal=20 last_argument=7 PASS` |
| 05 | Fake Timer | `one_shot=1 periodic=3 now=3000 PASS` |
| 06 | UART Shell | `commands=6 known=6 unknown=1 PASS` |
| 07 | Binary Event Trace | `count=128 overwritten=5 oldest=5 clear=PASS` |
| 08 | Queue Latency | `min=5 max=20 avg=11 PASS` |
| 09 | Handler Execution Time | `DISPLAY handler max: 1420 us threshold=1000 exceeded=1` |
| 10 | Crash Record | `valid=1 fatal=0xE001 task=3 signal=42 state=2 uptime=12345 reset=1` |
| 11 | HardFault Record | `pc=0x08001234 lr=0xFFFFFFF9 xpsr=0x21000000 cfsr=0x00008200 PASS` |
| 12 | Automated Use Case | `steps=6 final=IDLE PASS` |
| 13 | Fault Injection | `pool=1 crc=1 sensor=1 flash=1 triggers=4 PASS` |
| 14 | Regression Test | `bug_reproduced=1 fixed=1 stale_timeout=0 PASS` |
| 15 | Stress Test | `events=10000 drop=0 queue_max=16 pool_max=16 handler_max=900 PASS` |
| 16 | Soak Test | `hours=12 samples=12 leak=0 resets=0 PASS` |
| 17 | Continuous Integration | `stages=6 passed=6 failed=0 PASS` |
| 18 | Hardware-in-the-Loop | `reset=PASS commands=4 passed=4 failed=0 HIL=PASS` |
