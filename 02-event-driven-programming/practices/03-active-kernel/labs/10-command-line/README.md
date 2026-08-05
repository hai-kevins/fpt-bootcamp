# Lab 10 - Command Line

## Mục tiêu

- Parse command help/task/pool/timer/state/led.
- Shell chỉ post message thay vì gọi driver application trực tiếp.
- Tách parser khỏi owner task.

## Build và chạy

Makefile của lab chỉ quản lý source và executable của chính lab.

```bash
cd labs/10-command-line
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

Command hợp lệ tạo message; command không hợp lệ bị từ chối và không thay đổi application trực tiếp.

## Câu hỏi

1. Vì sao shell không được gọi GPIO driver?
2. Command dài quá buffer xử lý thế nào?
3. Parser chạy trong ISR có phù hợp không?
4. Lệnh diagnostics nào cần read-only?

## Table pattern

Lab dùng `cmd_line_t` và function pointer thay cho chuỗi `strcmp()` trong shell. Handler chỉ mô phỏng việc post message, không gọi driver trực tiếp.
