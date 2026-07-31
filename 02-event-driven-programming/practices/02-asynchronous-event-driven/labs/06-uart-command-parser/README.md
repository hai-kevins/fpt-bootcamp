# Lab 06 - UART Command Parser

## Mục tiêu

- Tách command parser khỏi UART driver.
- Parse `led on`, `led off`, `blink <ms>` và `status`.
- Validate blink period từ 50 đến 5000 ms.
- Trả command object để tầng khác chuyển thành event.

## Build và chạy

```bash
cd labs/06-uart-command-parser
make
make test
make run
```

## Kết quả

Test phải báo:

```text
[PASS] command-parser
```

Input không hợp lệ phải trả `false` và `COMMAND_INVALID`.

## Câu hỏi

1. Vì sao parser không nên điều khiển GPIO trực tiếp?
2. Command object nên chứa pointer hay copy parameter?
3. Buffer command cần policy gì khi overflow?
4. Parser được nối với UART ISR qua event sequence nào?
