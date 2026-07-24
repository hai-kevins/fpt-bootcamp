# Lab 01 - Task State Machine

## Mục tiêu

- Cài enum READY/RUNNING/BLOCKED/SUSPENDED.
- Kiểm tra transition hợp lệ và không hợp lệ.
- Không đổi state khi transition bị reject.

## Build và test

Makefile chỉ quản lý lab này:

```bash
cd labs/01-task-state-machine
make test
make run
make clean
```

## Kết quả

Unit test chạy với ASan/UBSan. Demo được tạo tại `build/demo`. Hãy chủ động sửa một invariant, xác nhận test fail, rồi khôi phục implementation.

## Câu hỏi

1. Vì sao SUSPENDED không được chuyển trực tiếp sang RUNNING?
2. State enum có đủ để mô tả task không?
3. Transition helper phải cập nhật thêm những list nào trong kernel thật?
4. CREATED khác READY ở đâu?
