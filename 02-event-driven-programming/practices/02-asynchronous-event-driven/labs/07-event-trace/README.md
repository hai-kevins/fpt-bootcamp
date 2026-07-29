# Lab 07 - Event Trace

## Mục tiêu

- Ghi event vào ring buffer kích thước cố định.
- Giữ các record mới nhất khi buffer đầy.
- Xuất CSV gồm timestamp, source, destination, signal và parameter.
- Theo dõi số record bị overwrite.

## Build và chạy

```bash
cd labs/07-event-trace
make
make run
```

## Kết quả

Lab ghi 12 record vào buffer 8 record:

```text
overwritten=4
```

CSV chỉ còn tám record mới nhất và giữ đúng thứ tự thời gian.

## Câu hỏi

1. Vì sao binary record phù hợp hơn text log trong runtime?
2. Khi nào nên overwrite-oldest thay vì drop-newest?
3. Record cần thêm phase nào để đo queue latency?
4. Trace buffer nên được snapshot khi fatal như thế nào?
