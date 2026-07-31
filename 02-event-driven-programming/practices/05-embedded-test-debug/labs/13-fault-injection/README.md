# Lab 13 - Fault Injection

## Mục tiêu

- Mô phỏng Pool fail.
- Mô phỏng CRC error.
- Mô phỏng Sensor timeout.
- Mô phỏng Flash fail.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/13-fault-injection
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
pool=1 crc=1 sensor=1 flash=1 triggers=4 PASS
```

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đúng.

## Câu hỏi

1. Fault Injection phải được giới hạn ở build nào?
2. One-shot và persistent fault khác nhau thế nào?
3. Fault trigger cần được trace vì sao?
4. Làm sao inject power loss giữa hai bước ghi Flash?
