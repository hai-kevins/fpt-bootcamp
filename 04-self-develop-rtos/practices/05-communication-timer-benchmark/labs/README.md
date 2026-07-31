# Labs — Independent Makefiles

Các bài thực hành trong `05-communication-timer-benchmark` được tổ chức độc lập. Mỗi lab có source code, Makefile, README và thư mục `build/` riêng.

## 1. Mục tiêu

- Tách từng nội dung thành một bài thực hành nhỏ.
- Build và kiểm thử từng lab mà không ảnh hưởng project root.
- Phân biệt executable chạy trên host và firmware chạy trên target.
- Giữ artifact sinh tự động trong `build/` của đúng lab.
- Dùng cùng một bố cục README cho toàn bộ repository.

## 2. Build và chạy

Ví dụ host lab:

```bash
cd labs/01-static-ring-buffer-queue
make
make run
```
Ví dụ target lab:

```bash
cd labs/03-queue-from-isr
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
| 01 | Static Ring-Buffer Queue | Host Linux | `labs/01-static-ring-buffer-queue/build/` |
| 02 | Blocking Queue on Host | Host Linux | `labs/02-blocking-queue-host/build/` |
| 03 | Queue from ISR | STM32F103 | `labs/03-queue-from-isr/build/` |
| 04 | Fixed-Block Message Pool | Host Linux | `labs/04-message-pool/build/` |
| 05 | Software Timer List | Host Linux | `labs/05-software-timer-list/build/` |
| 06 | Timer Service Task | STM32F103 | `labs/06-timer-service-task/build/` |
| 07 | Timestamp Backends | STM32F103 | `labs/07-timestamp-backends/build/` |
| 08 | Context-Switch Benchmark | STM32F103 | `labs/08-context-switch-benchmark/build/` |
| 09 | Event and Message Latency | STM32F103 | `labs/09-event-message-latency/build/` |
| 10 | Target Communication and Benchmark Console | STM32F103 | `labs/10-target-communication-benchmark-console/build/` |

## 4. Quy ước

- Không build lab từ Makefile root trừ khi root Makefile ghi rõ target tương ứng.
- Luôn chạy lệnh trong đúng thư mục lab.
- Executable host không thể flash lên STM32.
- `make flash-stlink` trong target lab chỉ nạp firmware của lab đó.
- `make clean` trong một lab không xóa artifact của project root hoặc lab khác.
- README từng lab luôn theo thứ tự: Mục tiêu, Build và chạy, Kết quả, Câu hỏi.

## 5. Kiểm tra toàn bộ

Kiểm tra từng lab độc lập:

```bash
cd labs/<lab-name>
make clean
make run
```

Với target lab, sử dụng lệnh build, flash hoặc report được ghi trong README của lab đó.
