# Lab 10 - Super-loop vs Event-Driven

## Mục tiêu

- Cài cùng một tình huống theo hai kiến trúc.
- So sánh biến global, nhánh trong main, latency và blocking risk.
- Phân biệt số đo minh họa với benchmark thật.
- Viết nhận xét trong `comparison.md`.

## Build và chạy

```bash
cd labs/10-superloop-vs-event-driven
make
make run
```

## Kết quả

Hai executable in metric cùng định dạng để so sánh. Event-Driven có thêm queue/framework cost nhưng tách event, state và handler rõ hơn.

`comparison.md` là worksheet để bổ sung số đo của firmware thật.

## Câu hỏi

1. Event-Driven đổi CPU cost lấy lợi ích kiến trúc nào?
2. Super-loop có thể non-blocking không?
3. Khi nào Event-Driven là over-engineering?
4. Cần đo thêm Flash, RAM, handler WCET và queue depth như thế nào?
