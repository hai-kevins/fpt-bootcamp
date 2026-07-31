# Lab 12 - Event Record

## Mục tiêu

- Lưu 64 event cuối cùng trong ring buffer.
- Ghi đè record cũ khi đầy.
- Hỗ trợ dump và clear.

## Build và chạy

Makefile của lab chỉ quản lý source và executable của chính lab.

```bash
cd labs/12-event-record
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

Sau 70 event, buffer giữ 64 event mới nhất và overwritten bằng 6; clear đưa count về 0.

## Câu hỏi

1. Vì sao ring buffer phù hợp debug?
2. Nên lưu ID hay chuỗi?
3. Record nào cần timestamp?
4. Dump record có được block task quan trọng không?
