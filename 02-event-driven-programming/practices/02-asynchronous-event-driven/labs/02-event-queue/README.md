# Lab 02 - Circular Event Queue

## Mục tiêu

- Cài FIFO queue bằng circular buffer và bộ nhớ tĩnh.
- Cài `post()`, `get()`, queue full và queue empty.
- Test wraparound khi head/tail quay về đầu mảng.
- Ghi overflow counter và high-water mark.

## Build và test

```bash
cd labs/02-event-queue
make
make test
make run
```

Artifact:

```text
build/lab02_event_queue
build/test_runner
```

## Kết quả

Test phải báo:

```text
[PASS] fifo
[PASS] wraparound-full
```

Queue giữ thứ tự FIFO, từ chối event khi đầy và không ghi đè dữ liệu chưa xử lý.

## Câu hỏi

1. Invariant nào liên hệ `head`, `tail` và `count`?
2. Vì sao `count` giúp phân biệt queue empty và full?
3. Drop-newest khác overwrite-oldest như thế nào?
4. High-water mark dùng để sizing queue ra sao?
