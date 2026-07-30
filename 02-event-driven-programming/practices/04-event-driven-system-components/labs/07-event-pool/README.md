# Lab 07 - Static Event Pool

## Mục tiêu

- Allocate toàn bộ block trong fixed-size Event Pool.
- Xác nhận lần allocate tiếp theo thất bại deterministic.
- Release toàn bộ event về pool.
- Quan sát high-water mark và allocation failure count.

## Build và test

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/07-event-pool
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

Output dự kiến cho thấy high-water mark bằng capacity của pool và có đúng một allocation failure. Cuối chương trình `used_count == 0`.

## Câu hỏi

1. Fixed pool tránh external fragmentation như thế nào?
2. Điều gì quyết định số block cần thiết?
3. Allocation failure nên được xử lý khác nhau theo criticality của signal ra sao?
4. Vì sao invariant `used_count == 0` quan trọng sau mỗi test?
