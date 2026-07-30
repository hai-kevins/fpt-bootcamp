# Lab 11 - UART-style Data-Link Frame

## Mục tiêu

- Tạo frame chứa addressing, signal, sequence và payload.
- Serialize frame thành byte buffer.
- Tính và kiểm tra CRC-16.
- Decode byte buffer về frame ban đầu.
- Hiểu ranh giới giữa serializer và UART driver vật lý.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/11-uart-datalink
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
bytes=15 signal=0x55AA
```

Frame 15 byte được decode đúng signal, sequence và payload ban đầu.

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đều đúng.

## Câu hỏi

1. Frame cần trường length để giải quyết vấn đề gì?
2. CRC phát hiện lỗi nhưng có sửa lỗi được không?
3. Endianness của signal và sequence phải được protocol quy định thế nào?
4. UART driver vật lý nên nằm trên hay dưới serializer?
