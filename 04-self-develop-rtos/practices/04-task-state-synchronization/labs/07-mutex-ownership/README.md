# Lab 07 - Mutex Ownership

## Mục tiêu

- Owner-only unlock.
- Non-recursive lock policy.
- Priority-ordered waiters và ownership transfer.

## Build và chạy

Makefile chỉ quản lý lab này:

```bash
cd labs/07-mutex-ownership
make test
make run
make clean
```

## Kết quả

Unit test chạy với ASan/UBSan. Demo được tạo tại `build/demo`. Hãy chủ động sửa một invariant, xác nhận test fail, rồi khôi phục implementation.

## Câu hỏi

1. Vì sao chỉ owner được unlock?
2. Recursive lock policy trong lab là gì?
3. Mutex khác binary semaphore ở đâu?
4. Ownership transfer nên xảy ra trước hay sau wake waiter?
