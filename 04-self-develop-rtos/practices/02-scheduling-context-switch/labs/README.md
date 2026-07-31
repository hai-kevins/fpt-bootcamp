# Labs — Independent Makefiles

Các bài thực hành trong `02-scheduling-context-switch` được tổ chức độc lập. Mỗi lab có source code, Makefile, README và thư mục `build/` riêng.

## 1. Mục tiêu

- Tách từng nội dung thành một bài thực hành nhỏ.
- Build và kiểm thử từng lab mà không ảnh hưởng project root.
- Phân biệt executable chạy trên host và firmware chạy trên target.
- Giữ artifact sinh tự động trong `build/` của đúng lab.
- Dùng cùng một bố cục README cho toàn bộ repository.

## 2. Build và chạy

Ví dụ host lab:

```bash
cd labs/02-initial-task-stack
make
make run
```
Ví dụ target lab:

```bash
cd labs/01-exception-stack-inspection
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
| 01 | Cortex-M3 Exception and Stack Inspection | STM32F103 | `labs/01-exception-stack-inspection/build/` |
| 02 | Initial Task Stack Builder | Host Linux | `labs/02-initial-task-stack/build/` |
| 03 | Start First Task Using SVC | STM32F103 | `labs/03-svc-first-task/build/` |
| 04 | Cooperative PendSV Context Switch | STM32F103 | `labs/04-cooperative-pendsv/build/` |
| 05 | Priority Scheduler Policy on Host | Host Linux | `labs/05-priority-scheduler-host/build/` |
| 06 | Fixed-Priority Scheduler on Target | STM32F103 | `labs/06-fixed-priority-target/build/` |
| 07 | SysTick Kernel Tick and Time Slice | STM32F103 | `labs/07-systick-time-slice/build/` |
| 08 | Preemption from EXTI0 Interrupt | STM32F103 | `labs/08-isr-preemption/build/` |
| 09 | Equal-Priority Round-Robin | STM32F103 | `labs/09-round-robin/build/` |
| 10 | Context-Switch Stress and Diagnostics | STM32F103 | `labs/10-context-switch-stress/build/` |

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
