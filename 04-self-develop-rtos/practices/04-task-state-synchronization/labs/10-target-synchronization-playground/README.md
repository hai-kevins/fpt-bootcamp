# Lab 10 - Target Synchronization Playground

## Mục tiêu

- Chạy toàn bộ delay, semaphore, mutex, inheritance, suspend/resume và UART diagnostics trên STM32F103.
- Quan sát kết quả qua UART/GDB.
- Ghi lại một fault injection và nguyên nhân.

## Build

```bash
cd labs/10-target-synchronization-playground
make
make size
make disasm
make flash-stlink
make clean
```

## Kết quả

Firmware tạo `.elf`, `.bin`, `.hex`, `.map` và `.lst` trong `build/` của lab. Xác nhận trên STM32F103 thật trước khi đánh dấu target validation PASS.

## Câu hỏi

1. Vì sao ISR không được gọi blocking API?
2. PendSV phải chạy trước hay sau EXTI handler return?
3. Task state/list membership nào cần kiểm tra bằng GDB?
4. Stack guard và scheduler validation phát hiện lỗi gì?
