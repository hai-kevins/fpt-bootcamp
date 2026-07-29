# Validation

Repository được kiểm tra trong môi trường Linux với:

```text
Host compiler : GCC 14.2.0
Target compiler: Clang 17.0.0 + LLD
Target         : arm-none-eabi, Cortex-M3
```

## Root host unit tests

Lệnh:

```bash
make test
```

Kết quả:

```text
[PASS] event_queue_fifo
[PASS] event_queue_wrap_and_overflow
[PASS] timer_one_shot
[PASS] timer_periodic
[PASS] button_debounce_press
[PASS] led_state_machine
[PASS] app_state_machine

Summary: passed=7 failed=0
```

Host tests được build với:

```text
-Wall -Wextra -Werror
-Wshadow -Wundef
-Wconversion -Wsign-conversion
-fsanitize=address,undefined
```

## Root target build

Lệnh:

```bash
make TOOLCHAIN=clang
```

Kết quả:

```text
text = 4688 bytes
data = 0 bytes
bss  = 2616 bytes
```

Build đã tạo thành công ELF, BIN, HEX, MAP và LST cho Cortex-M3.

## Lab validation

Các host lab sau đã build và chạy thành công:

```text
01-blocking-to-nonblocking
02-event-queue
03-dispatcher
05-led-state-machine
06-uart-command-parser
07-event-trace
08-queue-stress-test
09-product-state-machine
10-superloop-vs-event-driven
```

Lab 04 đã build thành công bằng:

```bash
cd labs/04-button-debounce
make TOOLCHAIN=clang
```

Kích thước tại lần kiểm tra:

```text
text = 1172 bytes
data = 0 bytes
bss  = 92 bytes
```

## Chưa xác nhận

Các mục sau chưa được kiểm thử trực tiếp trong môi trường tạo repository:

- Flash bằng ST-Link hoặc OpenOCD.
- Button PA0 trên board thật.
- LED PC13 trên board thật.
- USART1 PA9/PA10 trên board thật.
- Timing debounce dưới nhiễu/bounce thực tế.
- Stress test đồng thời giữa EXTI0, USART1 và SysTick trên target.

Sau khi chạy trên board, nên bổ sung kết quả thực nghiệm, ảnh wiring, UART log và commit ID vào file này.
