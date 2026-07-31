# Lab 02 - Initial Task Stack Builder

## Mục tiêu

- Tạo Cortex-M3 initial task frame trong một mảng `uint32_t` trên host.
- Kiểm tra alignment, Thumb bit, task entry, task-return LR và R0 argument.
- Ghi lại một frame lỗi có chủ đích và giải thích vì sao exception return sẽ thất bại.

## Build và chạy

Makefile của lab này chỉ quản lý source, test và executable của chính lab. Từ thư mục root:

```bash
cd labs/02-initial-task-stack
make test
make run
```

Có thể dùng:

```bash
make       # Build và chạy test
make test  # Chỉ build/chạy unit test
make run   # Build rồi chạy demo
make clean # Chỉ xóa build/ của lab này
```

## Kết quả

Artifact demo chính nằm cục bộ trong lab:

```text
build/lab02_initial_task_stack-demo
```

Chạy trực tiếp bằng `./build/lab02_initial_task_stack-demo` hoặc `make run`.

Chạy demo để in 16 word của initial frame. Unit test kiểm tra input không hợp lệ, stack range, alignment và các giá trị register bằng ASan/UBSan.

## Câu hỏi

1. Vì sao task chưa từng chạy vẫn cần hardware exception frame giả?
2. Tại sao xPSR phải đặt Thumb bit?
3. R0 trong initial frame được dùng để truyền dữ liệu gì?
4. Điều gì xảy ra nếu task entry return mà LR không trỏ tới error handler?
