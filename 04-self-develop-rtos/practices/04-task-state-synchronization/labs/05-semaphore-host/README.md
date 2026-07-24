# Lab 05 - Binary and Counting Semaphore

## Mục tiêu

- Cài count/max_count.
- Waiter priority order và FIFO cùng priority.
- Give wake waiter mà không double-count.

## Build và test

Makefile chỉ quản lý lab này:

```bash
cd labs/05-semaphore-host
make test
make run
make clean
```

## Kết quả

Unit test chạy với ASan/UBSan. Demo được tạo tại `build/demo`. Hãy chủ động sửa một invariant, xác nhận test fail, rồi khôi phục implementation.

## Câu hỏi

1. Semaphore có ownership không?
2. Vì sao give không tăng count khi có waiter?
3. Binary semaphore khác mutex ở đâu?
4. FIFO cùng priority được bảo đảm bằng gì?
