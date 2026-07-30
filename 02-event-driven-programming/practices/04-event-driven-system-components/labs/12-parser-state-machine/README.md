# Lab 12 - Parser State Machine

## Mục tiêu

- Feed từng byte vào parser thay vì chờ cả frame blocking.
- Bỏ qua byte rác trước SOF.
- Nhận đủ frame đã serialize.
- Xác nhận parser trả frame ready và payload đúng.

## Build và test

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/12-parser-state-machine
make test
make run
```

Có thể dùng:

```bash
make          # Build executable
make test     # Build rồi chạy bài kiểm tra
make run      # Build rồi chạy demo/test
make sanitize # Chạy với ASan/UBSan
make clean    # Chỉ xóa build/ của lab này
```

## Kết quả

Artifact chính nằm cục bộ trong lab:

```text
build/lab
```

Output dự kiến: `ready=1 frames=1`. Parser bỏ byte `0x00` ban đầu, nhận đủ frame hợp lệ và trả payload bằng 42.

## Câu hỏi

1. Parser byte-by-byte phù hợp với UART ISR/ring buffer như thế nào?
2. Parser nên resynchronize ra sao khi length hoặc CRC sai?
3. Làm sao tránh đọc vượt payload buffer?
4. Timeout giữa các byte nên được xử lý ở parser hay Data-Link layer?
