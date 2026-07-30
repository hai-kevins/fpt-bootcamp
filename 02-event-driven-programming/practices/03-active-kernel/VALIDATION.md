# Validation

Repository được kiểm tra trong môi trường Linux trước khi đóng gói.

## Root host tests

```text
[PASS] message_pool
[PASS] task_scheduler
[PASS] timer
[PASS] fsm_and_tsm
[PASS] event_record
[PASS] fatal_record
[PASS] application_flow
Summary: 7/7 passed
```

Lệnh:

```bash
make test
```

Host tests được build bằng GCC với:

```text
-Wall -Wextra -Werror
-fsanitize=address,undefined
```

## Target build

Firmware root đã compile và link thành công cho ARM Cortex-M3 bằng:

```bash
make TOOLCHAIN=clang
```

Memory summary tại thời điểm validation:

```text
text = 7556 bytes
data = 0 bytes
bss  = 3308 bytes
total reported = 10864 bytes
```

Các artifact `.elf`, `.bin`, `.hex`, `.map` và `.lst` đều được tạo thành công trong lần validation. Build output được xóa trước khi đóng gói ZIP.

GNU Arm Embedded Toolchain không có trong môi trường kiểm tra này, vì vậy target được xác nhận bằng Clang/LLD. Makefile vẫn mặc định hỗ trợ `arm-none-eabi-gcc`.

## Labs

Toàn bộ 13 lab đã build và chạy thành công bằng GCC host:

```text
PASS 01-ak-structure
PASS 02-two-tasks
PASS 03-pure-message
PASS 04-common-message
PASS 05-dynamic-message
PASS 06-one-shot-timer
PASS 07-periodic-timer
PASS 08-fsm
PASS 09-tsm
PASS 10-command-line
PASS 11-pool-stress-test
PASS 12-event-record
PASS 13-fatal-record
```

Lab 14 Mini Game không tồn tại theo yêu cầu.

## Chưa kiểm tra

Chưa thực hiện trong môi trường hiện tại:

- Flash lên STM32F103C8T6 thật.
- Kiểm tra EXTI0 bằng nút PA0 thật.
- Kiểm tra UART USART1 bằng USB-UART thật.
- Kiểm tra reset thật và khả năng giữ `.noinit` qua từng loại reset.
- Đo latency, jitter và WCET bằng logic analyzer hoặc cycle counter.
