# Lab 01 - Blocking to Non-blocking

## Mục tiêu

- So sánh độ trễ phản hồi giữa blocking delay và timer-based non-blocking.
- Quan sát một button event đến trong khi LED đang chờ timeout.
- Chứng minh công việc chờ thời gian không cần chiếm CPU.
- Ghi lại một trường hợp mà blocking làm mất hoặc trì hoãn event.

## Build và chạy

```bash
cd labs/01-blocking-to-nonblocking
make
make run
```

Có thể dùng:

```bash
make       # Build hai demo
make run   # Chạy blocking và non-blocking
make clean # Chỉ xóa build/ của Lab 01
```

## Kết quả

Blocking demo báo latency khoảng 700 ms khi event đến ở 300 ms nhưng chỉ được xử lý sau delay 1000 ms.

Non-blocking demo xử lý event ngay ở 300 ms và vẫn giữ LED timeout ở 1000 ms.

## Câu hỏi

1. Blocking delay làm tăng worst-case response time như thế nào?
2. Timer Event cần lưu những thông tin tối thiểu nào?
3. Non-blocking có đồng nghĩa với multi-threading không?
4. Handler nào trong firmware root thay thế blocking delay?
