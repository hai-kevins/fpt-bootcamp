# Validation

Repository được kiểm tra trong môi trường Linux bằng Clang/LLD cho target
`arm-none-eabi`, Cortex-M3 và compiler host có AddressSanitizer/
UndefinedBehaviorSanitizer.

## Kết quả

```text
Structure check                              PASS
Lab 01 static ring-buffer queue             PASS
Lab 02 blocking queue model                 PASS
Lab 04 fixed-block message pool             PASS
Lab 05 software timer list                  PASS
Root Cortex-M3 cross-build                  PASS
Lab 03 queue from ISR cross-build           PASS
Lab 06 timer service task cross-build       PASS
Lab 07 timestamp backends cross-build       PASS
Lab 08 context-switch benchmark cross-build PASS
Lab 09 event/message latency cross-build    PASS
Lab 10 target console cross-build           PASS
AddressSanitizer                            PASS
UndefinedBehaviorSanitizer                  PASS
```

## Kích thước cross-build

| Firmware | `.text` | `.data` | `.bss` |
|---|---:|---:|---:|
| Root Communication and Benchmark Console | 11036 B | 0 B | 6992 B |
| Lab 03 Queue from ISR | 5104 B | 0 B | 1536 B |
| Lab 06 Timer Service Task | 5264 B | 0 B | 1688 B |
| Lab 07 Timestamp Backends | 3172 B | 0 B | 104 B |
| Lab 08 Context-Switch Benchmark | 5252 B | 0 B | 3216 B |
| Lab 09 Event/Message Latency | 5892 B | 0 B | 2816 B |
| Lab 10 Target Console | 11036 B | 0 B | 6992 B |

Kích thước có thể thay đổi theo compiler, phiên bản linker và optimization flags.

## Giới hạn validation

Firmware chưa được flash lên STM32F103 vật lý trong môi trường tạo repository.
Các mục sau vẫn cần được xác nhận trên board thật:

- SVC, PendSV, PSP/MSP và SysTick 1 kHz.
- EXTI0 trên PA0.
- UART1 PA9/PA10 ở 9600 baud.
- DWT `CYCCNT` có hoạt động trong cấu hình debug/target đang dùng hay không.
- ISR-to-task latency bằng GPIO và logic analyzer.
- Software timer callback latency và periodic drift.
- Context-switch benchmark trong nhiều mức interrupt load.
- Stack guard, long-run stress test và queue timeout race trên target.

Maximum quan sát trong benchmark không được xem là WCET đã được chứng minh.
