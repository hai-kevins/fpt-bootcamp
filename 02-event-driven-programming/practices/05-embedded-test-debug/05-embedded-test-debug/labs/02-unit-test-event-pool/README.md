# Lab 02 - Unit Test Event Pool

## Mục tiêu

- Allocate toàn bộ block.
- Kiểm thử exhaustion.
- Free và reuse block.
- Phát hiện double free.
- Theo dõi high-water mark.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/02-unit-test-event-pool
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
allocate=PASS exhaustion=PASS reuse=PASS double_free=PASS hwm=4
```

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đúng.

## Câu hỏi

1. Fixed-size pool tránh fragmentation như thế nào?
2. Double free phải được phát hiện bằng metadata nào?
3. High-water mark dùng để sizing pool ra sao?
4. Pool exhaustion nên dẫn đến drop, retry hay fatal khi nào?
