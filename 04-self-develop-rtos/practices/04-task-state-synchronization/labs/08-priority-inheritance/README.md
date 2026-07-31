# Lab 08 - Priority Inversion and Inheritance

## Mục tiêu

- Mô hình High/Medium/Low.
- Boost effective priority của owner.
- Restore priority sau unlock hoặc waiter removal.

## Build và chạy

Makefile chỉ quản lý lab này:

```bash
cd labs/08-priority-inheritance
make test
make run
make clean
```

## Kết quả

Unit test chạy với ASan/UBSan. Demo được tạo tại `build/demo`. Hãy chủ động sửa một invariant, xác nhận test fail, rồi khôi phục implementation.

## Câu hỏi

1. Priority inversion cần ba task như thế nào?
2. Scheduler dùng base hay effective priority?
3. READY owner phải được di chuyển queue khi boost vì sao?
4. Transitive inheritance khác basic inheritance ở đâu?
