# Lab 05 - Fake Timer

## Mục tiêu

- Không dùng thời gian thật.
- Kiểm thử one-shot timer.
- Kiểm thử periodic timer.
- Advance fake time deterministic.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/05-fake-timer
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
one_shot=1 periodic=3 now=3000 PASS
```

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đúng.

## Câu hỏi

1. Fake time làm test nhanh hơn thế nào?
2. Periodic timer catch-up hay skip missed periods?
3. Timer wraparound cần test ra sao?
4. Monotonic time khác wall-clock time thế nào?
