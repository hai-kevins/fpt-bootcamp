# Lab 02 - Two Tasks

## Mục tiêu

- Tạo Task A và Task B.
- Task A nhận START_SIG rồi gửi HELLO_SIG.
- Quan sát message routing qua queue.

## Build và test

Makefile của lab chỉ quản lý source và executable của chính lab.

```bash
cd labs/02-two-tasks
make test
make run
```

Có thể dùng:

```bash
make       # Build và chạy test/demo
make test  # Chạy kiểm tra
make run   # Chạy demo
make clean # Chỉ xóa build/ của lab hiện tại
```

## Kết quả

Output phải có `Task B received HELLO_SIG` và queue trở về rỗng.

## Câu hỏi

1. Ai sở hữu message sau khi post?
2. Nếu mailbox Task B đầy thì xử lý thế nào?
3. Handler nào là run-to-completion?
4. Có cần hai stack riêng cho hai task không?
