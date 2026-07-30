# Lab 12 - Parser State Machine

## Mục tiêu

- Feed từng byte vào parser thay vì chờ cả frame blocking.
- Bỏ qua byte rác trước Start of Frame.
- Nhận đủ frame đã serialize.
- Xác nhận parser trả frame ready và payload đúng.
- Hiểu yêu cầu resynchronization sau frame lỗi.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/12-parser-state-machine
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

Artifact chính nằm cục bộ trong lab:

```text
build/lab
```

Output dự kiến:

```text
ready=1 frames=1
```

Parser bỏ byte rác ban đầu, nhận đủ một frame hợp lệ và trả payload bằng 42.

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đều đúng.

## Câu hỏi

1. Parser byte-by-byte phù hợp với UART ISR và ring buffer như thế nào?
2. Parser nên resynchronize ra sao khi length hoặc CRC sai?
3. Làm sao tránh đọc vượt payload buffer?
4. Timeout giữa các byte nên được xử lý ở parser hay Data-Link layer?
