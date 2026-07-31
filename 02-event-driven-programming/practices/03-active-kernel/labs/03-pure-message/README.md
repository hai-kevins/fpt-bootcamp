# Lab 03 - Pure Message

## Mục tiêu

- Gửi signal không có payload.
- Điều khiển trạng thái LED bằng ON/OFF/TOGGLE.
- Nhận biết khi nào pure message là đủ.

## Build và chạy

Makefile của lab chỉ quản lý source và executable của chính lab.

```bash
cd labs/03-pure-message
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

Ba signal được xử lý không cần copy payload và test kết thúc PASS.

## Câu hỏi

1. Pure message tiết kiệm phần nào?
2. Khi nào signal cần payload?
3. Signal nên mô tả sự kiện hay lệnh register?
4. Có nên dùng một signal cho nhiều ý nghĩa không?
