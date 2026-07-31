# Labs — Independent Makefiles

Các bài thực hành trong `04-event-driven-system-components` được tổ chức độc lập. Mỗi lab có source code, Makefile, README và thư mục `build/` riêng.

## 1. Mục tiêu

- Tách từng khái niệm thành một bài thực hành nhỏ.
- Build và kiểm thử từng lab mà không ảnh hưởng project root.
- Phân biệt executable host với firmware target.
- Giữ output sinh tự động trong `build/` của đúng lab.
- Dùng cùng một bố cục README cho toàn bộ chương trình.

## 2. Build và chạy

Ví dụ host lab:

```bash
cd labs/01-active-object
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
| 01 | Active Object | Host Linux | `labs/01-active-object/build/` |
| 02 | Mailbox FIFO và Overflow | Host Linux | `labs/02-mailbox/build/` |
| 03 | Run-to-Completion Scheduler | Host Linux | `labs/03-run-to-completion-scheduler/build/` |
| 04 | Flat State Machine | Host Linux | `labs/04-flat-state-machine/build/` |
| 05 | Hierarchical State Machine | Host Linux | `labs/05-hierarchical-state-machine/build/` |
| 06 | Table-Driven State Machine | Host Linux | `labs/06-table-driven-state-machine/build/` |
| 07 | Static Event Pool | Host Linux | `labs/07-event-pool/build/` |
| 08 | Reference Count và Ownership | Host Linux | `labs/08-reference-count-ownership/build/` |
| 09 | Router và Direct Post | Host Linux | `labs/09-router-direct-post/build/` |
| 10 | Publish-Subscribe | Host Linux | `labs/10-publish-subscribe/build/` |
| 11 | UART-style Data-Link Frame | Host Linux | `labs/11-uart-datalink/build/` |
| 12 | Parser State Machine | Host Linux | `labs/12-parser-state-machine/build/` |
| 13 | Link Failure, Timeout và Retry | Host Linux | `labs/13-link-failure/build/` |
| 14 | Event Trace Ring Buffer | Host Linux | `labs/14-event-trace/build/` |
| 15 | Mailbox và Event Pool Stress Test | Host Linux | `labs/15-stress-test/build/` |

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
