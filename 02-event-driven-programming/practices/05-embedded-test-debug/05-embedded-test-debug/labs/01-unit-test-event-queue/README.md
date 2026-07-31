# Lab 01 - Unit Test Event Queue

## Mục tiêu

- Kiểm thử trạng thái empty.
- Kiểm thử thứ tự FIFO.
- Kiểm thử queue full.
- Kiểm thử wraparound của head và tail.
- Kiểm tra overflow counter.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/01-unit-test-event-queue
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
empty=PASS fifo=PASS full=PASS wrap=PASS overflow=1
```

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đúng.

## Câu hỏi

1. Invariant nào phải đúng với count, head và tail?
2. Drop-newest khác drop-oldest ở hành vi nào?
3. Tại sao wraparound cần test riêng?
4. Queue test cần mở rộng thế nào khi producer chạy trong ISR?
