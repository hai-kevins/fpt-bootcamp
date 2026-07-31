# Lab 03 - Delayed List and Tick Wrap

## Mục tiêu

- Sắp task theo wake tick.
- Wake nhiều task tới hạn.
- Kiểm thử wrap-around 32-bit.

## Build và chạy

Makefile chỉ quản lý lab này:

```bash
cd labs/03-delayed-list-tick-wrap
make test
make run
make clean
```

## Kết quả

Unit test chạy với ASan/UBSan. Demo được tạo tại `build/demo`. Hãy chủ động sửa một invariant, xác nhận test fail, rồi khôi phục implementation.

## Câu hỏi

1. Vì sao SysTick chỉ cần xem head của sorted delayed list?
2. Signed-difference comparison có contract timeout nào?
3. Hai task cùng deadline được sắp thế nào?
4. Timeout node và wait node khác nhau ra sao?
