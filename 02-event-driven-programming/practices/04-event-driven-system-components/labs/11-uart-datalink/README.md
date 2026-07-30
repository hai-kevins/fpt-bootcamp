# Lab 11 - UART-style Data-Link Frame

## Mục tiêu

- Tạo frame chứa addressing, signal, sequence và payload.
- Serialize frame thành byte buffer.
- Tính và kiểm tra CRC-16.
- Decode byte buffer về frame ban đầu.

## Build và test

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/11-uart-datalink
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

Output in số byte của frame và signal `0x55AA`. Chương trình trả exit code `0` khi sequence và payload sau decode giống dữ liệu đầu vào.

## Câu hỏi

1. Frame cần trường length để giải quyết vấn đề gì?
2. CRC phát hiện lỗi nhưng có sửa lỗi được không?
3. Endianness của signal và sequence phải được protocol quy định thế nào?
4. UART driver vật lý nên nằm trên hay dưới serializer?
