# Labs — Independent Makefiles

Các bài thực hành trong `01-embedded-foundation` được tổ chức độc lập. Mỗi lab có source code, Makefile, README và thư mục `build/` riêng.

## 1. Mục tiêu

- Tách từng khái niệm thành một bài thực hành nhỏ.
- Build và kiểm thử từng lab mà không ảnh hưởng project root.
- Phân biệt executable host với firmware target.
- Giữ output sinh tự động trong `build/` của đúng lab.
- Dùng cùng một bố cục README cho toàn bộ chương trình.

## 2. Build và chạy

Ví dụ host lab:

```bash
cd labs/01-endianness
make
make run
```
Ví dụ target lab:

```bash
cd labs/04-startup-code
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
| 01 | Endianness | Host Linux | `labs/01-endianness/build/` |
| 02 | Memory Access | Host Linux | `labs/02-memory-access/build/` |
| 03 | Memory Sections | Host Linux | `labs/03-memory-sections/build/` |
| 04 | Startup Code | STM32F103 | `labs/04-startup-code/build/` |
| 05 | Linker Script | STM32F103 | `labs/05-linker-script/build/` |
| 06 | GPIO Register-Level | STM32F103 | `labs/06-gpio-register/build/` |
| 07 | UART Polling | STM32F103 | `labs/07-uart-polling/build/` |
| 08 | Map File Analysis | STM32F103 | `labs/08-map-analysis/build/` |

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
