# Labs — Independent Makefiles

Các bài thực hành trong `05-embedded-test-debug` được tổ chức độc lập. Mỗi lab có source code, Makefile, README và thư mục `build/` riêng.

## 1. Mục tiêu

- Tách từng khái niệm thành một bài thực hành nhỏ.
- Build và kiểm thử từng lab mà không ảnh hưởng project root.
- Phân biệt executable host với firmware target.
- Giữ output sinh tự động trong `build/` của đúng lab.
- Dùng cùng một bố cục README cho toàn bộ chương trình.

## 2. Build và chạy

Ví dụ host lab:

```bash
cd labs/01-unit-test-event-queue
make
make run
```

Các lệnh thường dùng:

```bash
make          # Build lab hiện tại
make run      # Chạy host lab nếu Makefile hỗ trợ
make test     # Chạy test nếu Makefile hỗ trợ
make sanitize # Chạy ASan/UBSan nếu Makefile hỗ trợ
make clean    # Chỉ xóa output của lab hiện tại
```

## 3. Danh sách bài thực hành

| Bài | Chủ đề | Môi trường | Output |
|---:|---|---|---|
| 01 | Unit Test Event Queue | Host Linux | `labs/01-unit-test-event-queue/build/` |
| 02 | Unit Test Event Pool | Host Linux | `labs/02-unit-test-event-pool/build/` |
| 03 | Unit Test State Machine | Host Linux | `labs/03-unit-test-state-machine/build/` |
| 04 | Event Spy | Host Linux | `labs/04-event-spy/build/` |
| 05 | Fake Timer | Host Linux | `labs/05-fake-timer/build/` |
| 06 | UART Shell | Host Linux | `labs/06-uart-shell/build/` |
| 07 | Binary Event Trace | Host Linux | `labs/07-binary-event-trace/build/` |
| 08 | Queue Latency | Host Linux | `labs/08-queue-latency/build/` |
| 09 | Handler Execution Time | Host Linux | `labs/09-handler-execution-time/build/` |
| 10 | Crash Record | Host Linux | `labs/10-crash-record/build/` |
| 11 | HardFault Record | Host Linux | `labs/11-hardfault-record/build/` |
| 12 | Automated Use Case | Host Linux | `labs/12-automated-use-case/build/` |
| 13 | Fault Injection | Host Linux | `labs/13-fault-injection/build/` |
| 14 | Regression Test | Host Linux | `labs/14-regression-test/build/` |
| 15 | Stress Test | Host Linux | `labs/15-stress-test/build/` |
| 16 | Soak Test | Host Linux | `labs/16-soak-test/build/` |
| 17 | Continuous Integration | Host Linux | `labs/17-ci/build/` |
| 18 | Hardware-in-the-Loop | Host Linux | `labs/18-hardware-in-the-loop/build/` |

## 4. Quy ước

- Không build lab bằng Makefile root trừ khi root Makefile ghi rõ target tương ứng.
- Luôn chạy lệnh trong đúng thư mục lab.
- Host executable không thể flash lên STM32.
- `make flash-stlink` trong một target lab chỉ nạp firmware của lab đó.
- `make clean` trong lab không được xóa artifact của project root hoặc lab khác.
- README từng lab luôn theo thứ tự: Mục tiêu, Build và chạy, Kết quả, Câu hỏi.

## 5. Kiểm tra toàn bộ

Từ thư mục root:

```bash
./tools/validate.sh
```

Script build và chạy các bài kiểm tra được repository hỗ trợ.
