# Labs — Independent Makefiles

Các bài thực hành trong `02-asynchronous-event-driven` được tổ chức độc lập. Mỗi lab có source code, Makefile, README và thư mục `build/` riêng.

## 1. Mục tiêu

- Tách từng khái niệm thành một bài thực hành nhỏ.
- Build và kiểm thử từng lab mà không ảnh hưởng project root.
- Phân biệt executable host với firmware target.
- Giữ output sinh tự động trong `build/` của đúng lab.
- Dùng cùng một bố cục README cho toàn bộ chương trình.

## 2. Build và chạy

Ví dụ host lab:

```bash
cd labs/01-blocking-to-nonblocking
make
make run
```
Ví dụ target lab:

```bash
cd labs/04-button-debounce
make
make flash-stlink
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
| 01 | Blocking to Non-blocking | Host Linux | `labs/01-blocking-to-nonblocking/build/` |
| 02 | Circular Event Queue | Host Linux | `labs/02-event-queue/build/` |
| 03 | Dispatcher | Host Linux | `labs/03-dispatcher/build/` |
| 04 | Button Debounce | STM32F103 | `labs/04-button-debounce/build/` |
| 05 | LED State Machine | Host Linux | `labs/05-led-state-machine/build/` |
| 06 | UART Command Parser | Host Linux | `labs/06-uart-command-parser/build/` |
| 07 | Event Trace | Host Linux | `labs/07-event-trace/build/` |
| 08 | Queue Stress Test | Host Linux | `labs/08-queue-stress-test/build/` |
| 09 | Product State Machine | Host Linux | `labs/09-product-state-machine/build/` |
| 10 | Super-loop vs Event-Driven | Host Linux | `labs/10-superloop-vs-event-driven/build/` |

## 4. Quy ước

- Không build lab bằng Makefile root trừ khi root Makefile ghi rõ target tương ứng.
- Luôn chạy lệnh trong đúng thư mục lab.
- Host executable không thể flash lên STM32.
- `make flash-stlink` trong một target lab chỉ nạp firmware của lab đó.
- `make clean` trong lab không được xóa artifact của project root hoặc lab khác.
- README từng lab luôn theo thứ tự: Mục tiêu, Build và chạy, Kết quả, Câu hỏi.

## 5. Kiểm tra toàn bộ

Kiểm tra từng lab trong thư mục riêng:

```bash
cd labs/<lab-name>
make clean
make run
```

Với target lab, thay `make run` bằng target flash hoặc report được mô tả trong README của lab.
