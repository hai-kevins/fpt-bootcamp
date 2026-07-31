# Labs — Independent Makefiles

Các bài thực hành trong `01-rtos-introduction-memory-management` được tổ chức độc lập. Mỗi lab có source code, Makefile, README và thư mục `build/` riêng.

## 1. Mục tiêu

- Tách từng nội dung thành một bài thực hành nhỏ.
- Build và kiểm thử từng lab mà không ảnh hưởng project root.
- Phân biệt executable chạy trên host và firmware chạy trên target.
- Giữ artifact sinh tự động trong `build/` của đúng lab.
- Dùng cùng một bố cục README cho toàn bộ repository.

## 2. Build và chạy

Ví dụ host lab:

```bash
cd labs/03-stack-high-water
make
make run
```
Ví dụ target lab:

```bash
cd labs/01-memory-layout
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
| 01 | MCU Memory Layout | STM32F103 | `labs/01-memory-layout/build/` |
| 02 | Startup Sections | STM32F103 | `labs/02-startup-sections/build/` |
| 03 | Stack High-water Mark | Host Linux | `labs/03-stack-high-water/build/` |
| 04 | Static Memory Budget | Host Linux | `labs/04-static-memory-budget/build/` |
| 05 | First-fit Basic | Host Linux | `labs/05-first-fit-basic/build/` |
| 06 | Block Splitting | Host Linux | `labs/06-block-splitting/build/` |
| 07 | Block Coalescing | Host Linux | `labs/07-block-coalescing/build/` |
| 08 | Invalid Free Detection | Host Linux | `labs/08-invalid-free/build/` |
| 09 | Fragmentation | Host Linux | `labs/09-fragmentation/build/` |
| 10 | Allocator Target Demo | STM32F103 | `labs/10-target-demo/build/` |

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
