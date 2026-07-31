# Lab 08 - Queue Latency

## Mục tiêu

- Đo thời gian từ post tới dispatch.
- Tính min, max và average latency.
- Dùng timestamp monotonic.
- Phát hiện queue congestion.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/08-queue-latency
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
min=5 max=20 avg=11 PASS
```

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đúng.

## Câu hỏi

1. Queue latency khác handler execution time thế nào?
2. Average có thể che giấu worst-case ra sao?
3. Timestamp wraparound cần xử lý như thế nào?
4. Deadline nên kiểm tra theo max hay percentile?
