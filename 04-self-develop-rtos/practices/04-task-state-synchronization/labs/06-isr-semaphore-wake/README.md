# Lab 06 - ISR-to-Task Semaphore Wake

## Mục tiêu

- Dùng EXTI0 PA0 để give binary semaphore và wake high-priority task sau ISR return.
- Quan sát kết quả qua UART/GDB.
- Ghi lại một fault injection và nguyên nhân.

## Build và chạy

```bash
cd labs/06-isr-semaphore-wake
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
