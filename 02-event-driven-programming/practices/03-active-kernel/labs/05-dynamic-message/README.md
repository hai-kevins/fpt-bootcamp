# Lab 05 - Dynamic Message

## Mục tiêu

- Gửi chuỗi dài hơn common payload.
- Dùng fixed block pool thay vì heap tổng quát.
- Theo dõi pool used max và exhaustion.

## Build và test

Makefile của lab chỉ quản lý source và executable của chính lab.

```bash
cd labs/05-dynamic-message
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

Pool đạt high-water mark 4, allocation thứ năm thất bại có kiểm soát và cuối cùng used trở về 0.

## Câu hỏi

1. Vì sao pool phù hợp hơn malloc trong real-time path?
2. Allocation failure phải được xử lý ở đâu?
3. Reference count giải quyết bài toán broadcast thế nào?
4. Làm sao phát hiện message leak?
