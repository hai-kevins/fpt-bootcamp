# Lab 07 - Static Event Pool

## Mục tiêu

- Allocate toàn bộ block trong fixed-size Event Pool.
- Xác nhận lần allocate tiếp theo thất bại deterministic.
- Release toàn bộ event về pool.
- Quan sát high-water mark và allocation failure count.
- Kiểm tra invariant `used_count == 0` sau cleanup.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/07-event-pool
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
hwm=32 failures=1
```

High-water mark đạt đúng capacity, có một allocation failure và toàn bộ block được trả về pool.

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đều đúng.

## Câu hỏi

1. Fixed pool tránh external fragmentation như thế nào?
2. Điều gì quyết định số block cần thiết?
3. Allocation failure nên được xử lý khác nhau theo criticality của signal ra sao?
4. Vì sao invariant `used_count == 0` quan trọng sau mỗi test?
