# Labs — Independent Makefiles

Các bài thực hành trong `03-kernel-data-structures-task` được tổ chức độc lập. Mỗi lab có source code, Makefile, README và thư mục `build/` riêng.

## 1. Mục tiêu

- Tách từng nội dung thành một bài thực hành nhỏ.
- Build và kiểm thử từng lab mà không ảnh hưởng project root.
- Phân biệt executable chạy trên host và firmware chạy trên target.
- Giữ artifact sinh tự động trong `build/` của đúng lab.
- Dùng cùng một bố cục README cho toàn bộ repository.

## 2. Build và chạy

Ví dụ host lab:

```bash
cd labs/01-singly-linked-list
make
make run
```
Ví dụ target lab:

```bash
cd labs/10-target-task-inspector
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
| 01 | Singly Linked List | Host Linux | `labs/01-singly-linked-list/build/` |
| 02 | Intrusive Doubly Linked List | Host Linux | `labs/02-intrusive-doubly-list/build/` |
| 03 | List Invariants and Ownership | Host Linux | `labs/03-list-invariants/build/` |
| 04 | TCB Layout and Static Assertions | Host Linux | `labs/04-tcb-layout/build/` |
| 05 | Static Task Creation | Host Linux | `labs/05-static-task-creation/build/` |
| 06 | Task Stack Initialization | Host Linux | `labs/06-task-stack-initialization/build/` |
| 07 | All-Task Registry | Host Linux | `labs/07-all-task-registry/build/` |
| 08 | Per-Priority Ready Queues | Host Linux | `labs/08-priority-ready-queues/build/` |
| 09 | TCB in Multiple Lists | Host Linux | `labs/09-tcb-multiple-lists/build/` |
| 10 | Target Task Inspector | STM32F103 | `labs/10-target-task-inspector/build/` |

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
