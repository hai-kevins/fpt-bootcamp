# Labs — Independent Makefiles

Các bài thực hành trong `03-active-kernel` được tổ chức độc lập. Mỗi lab có source code, Makefile, README và thư mục `build/` riêng.

## 1. Mục tiêu

- Tách từng khái niệm thành một bài thực hành nhỏ.
- Build và kiểm thử từng lab mà không ảnh hưởng project root.
- Phân biệt executable host với firmware target.
- Giữ output sinh tự động trong `build/` của đúng lab.
- Dùng cùng một bố cục README cho toàn bộ chương trình.

## 2. Build và chạy

Ví dụ host lab:

```bash
cd labs/01-ak-structure
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
| 01 | AK Structure | Host Linux | `labs/01-ak-structure/build/` |
| 02 | Two Tasks | Host Linux | `labs/02-two-tasks/build/` |
| 03 | Pure Message | Host Linux | `labs/03-pure-message/build/` |
| 04 | Common Message | Host Linux | `labs/04-common-message/build/` |
| 05 | Dynamic Message | Host Linux | `labs/05-dynamic-message/build/` |
| 06 | One-shot Timer | Host Linux | `labs/06-one-shot-timer/build/` |
| 07 | Periodic Timer | Host Linux | `labs/07-periodic-timer/build/` |
| 08 | FSM | Host Linux | `labs/08-fsm/build/` |
| 09 | TSM | Host Linux | `labs/09-tsm/build/` |
| 10 | Command Line | Host Linux | `labs/10-command-line/build/` |
| 11 | Pool Stress Test | Host Linux | `labs/11-pool-stress-test/build/` |
| 12 | Event Record | Host Linux | `labs/12-event-record/build/` |
| 13 | Fatal Record | Host Linux | `labs/13-fatal-record/build/` |

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
