# Labs — Independent Makefiles

Các bài thực hành trong `04-task-state-synchronization` được tổ chức độc lập. Mỗi lab có source code, Makefile, README và thư mục `build/` riêng.

## 1. Mục tiêu

- Tách từng nội dung thành một bài thực hành nhỏ.
- Build và kiểm thử từng lab mà không ảnh hưởng project root.
- Phân biệt executable chạy trên host và firmware chạy trên target.
- Giữ artifact sinh tự động trong `build/` của đúng lab.
- Dùng cùng một bố cục README cho toàn bộ repository.

## 2. Build và chạy

Ví dụ host lab:

```bash
cd labs/01-task-state-machine
make
make run
```
Ví dụ target lab:

```bash
cd labs/06-isr-semaphore-wake
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
| 01 | Task State Machine | Host Linux | `labs/01-task-state-machine/build/` |
| 02 | Efficient Blocking | Host Linux | `labs/02-efficient-blocking/build/` |
| 03 | Delayed List and Tick Wrap | Host Linux | `labs/03-delayed-list-tick-wrap/build/` |
| 04 | Delay and Delay-Until | Host Linux | `labs/04-delay-until/build/` |
| 05 | Binary and Counting Semaphore | Host Linux | `labs/05-semaphore-host/build/` |
| 06 | ISR-to-Task Semaphore Wake | STM32F103 | `labs/06-isr-semaphore-wake/build/` |
| 07 | Mutex Ownership | Host Linux | `labs/07-mutex-ownership/build/` |
| 08 | Priority Inversion and Inheritance | Host Linux | `labs/08-priority-inheritance/build/` |
| 09 | Suspend and Resume | Host Linux | `labs/09-suspend-resume/build/` |
| 10 | Target Synchronization Playground | STM32F103 | `labs/10-target-synchronization-playground/build/` |

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
