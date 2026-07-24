# Lab 09 - Suspend and Resume

## Mục tiêu

- Suspend READY/RUNNING/BLOCKED theo cancel-wait policy.
- SUSPENDED không auto-wake.
- Chỉ explicit resume đưa task về READY.

## Build và test

Makefile chỉ quản lý lab này:

```bash
cd labs/09-suspend-resume
make test
make run
make clean
```

## Kết quả

Unit test chạy với ASan/UBSan. Demo được tạo tại `build/demo`. Hãy chủ động sửa một invariant, xác nhận test fail, rồi khôi phục implementation.

## Câu hỏi

1. Suspend khác block ở đâu?
2. Policy của lab khi suspend BLOCKED task là gì?
3. Vì sao timeout không được wake SUSPENDED task?
4. Idle task có nên suspend không?
