# Lab 10 - Target Task Inspector

## Mục tiêu

- Chạy static task creation, all-task registry, ready queues và Task Inspector trên STM32F103.
- Quan sát kết quả bằng UART và GDB thay vì chỉ đọc source code.
- Ghi lại một trường hợp lỗi có chủ đích và giải thích nguyên nhân.

## Build và chạy

Makefile của lab này chỉ build firmware của chính lab. Từ thư mục root:

```bash
cd labs/10-target-task-inspector
make
```

Các lệnh thường dùng:

```bash
make size
make disasm
make flash-stlink
make clean
```

## Kết quả

Artifact chính nằm cục bộ trong lab:

```text
build/lab10_task_inspector.elf
```

Flash file `.bin` bằng ST-Link rồi mở UART 9600 baud. Các lệnh là `h`, `t`, `r`, `c`, `s` và `v`.

## Câu hỏi

1. Task Inspector tạo snapshot trước khi in UART để giải quyết vấn đề gì?
2. Registry, ready queue và scheduler phải giữ những invariant nào?
3. Stack guard và saved-SP range phát hiện những lỗi gì?
4. Tại sao không được in UART trong PendSV?
