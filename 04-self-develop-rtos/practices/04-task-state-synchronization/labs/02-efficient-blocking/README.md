# Lab 02 - Efficient Blocking

## Mục tiêu

- Mô hình hóa ready queue, block và wake.
- Đảm bảo BLOCKED task không còn READY.
- Ngăn duplicate wake.

## Build và test

Makefile chỉ quản lý lab này:

```bash
cd labs/02-efficient-blocking
make test
make run
make clean
```

## Kết quả

Unit test chạy với ASan/UBSan. Demo được tạo tại `build/demo`. Hãy chủ động sửa một invariant, xác nhận test fail, rồi khôi phục implementation.

## Câu hỏi

1. Busy-wait khác blocking ở CPU usage thế nào?
2. BLOCKED task còn trong ready queue gây lỗi gì?
3. Wake task priority cao cần request gì?
4. Vì sao duplicate wake nguy hiểm?
