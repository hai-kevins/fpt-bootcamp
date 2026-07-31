# Lab 09 - Handler Execution Time

## Mục tiêu

- Đo thời gian bắt đầu và kết thúc handler.
- Theo dõi maximum execution time.
- Phát hiện handler vượt ngưỡng.
- Tạo dữ liệu tương tự `DISPLAY handler max`.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/09-handler-execution-time
make
make run
```

Có thể dùng:

```bash
make          # Build executable
make run      # Build rồi chạy lab
make test     # Alias của make run
make sanitize # Chạy với ASan/UBSan
make clean    # Chỉ xóa build/ của lab hiện tại
```

## Kết quả

Artifact chính:

```text
build/lab
```

Output dự kiến:

```text
DISPLAY handler max: 1420 us threshold=1000 exceeded=1
```

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đúng.

## Câu hỏi

1. WCET khác average execution time thế nào?
2. Đo bằng cycle counter cần xử lý overflow ra sao?
3. Instrumentation có thể làm thay đổi timing thế nào?
4. Khi vượt threshold, nên log, assert hay degrade chức năng?
