# Lab 05 - LED State Machine

## Mục tiêu

- Mô hình hóa LED bằng OFF, ON và BLINKING.
- Xử lý `ON`, `OFF`, `BLINK_START`, `BLINK_STOP`, `TIMEOUT`.
- Chứng minh timeout chỉ toggle khi state là BLINKING.
- Test State Machine không cần GPIO thật.

## Build và chạy

```bash
cd labs/05-led-state-machine
make
make test
make run
```

## Kết quả

Test phải báo:

```text
[PASS] led-state-machine
```

Demo in state, output level và blink period sau từng event.

## Câu hỏi

1. Vì sao blink period thuộc state object?
2. Khi rời BLINKING, timer owner phải làm gì?
3. Unhandled timeout ở OFF/ON nên bị bỏ hay báo lỗi?
4. GPIO driver nên nằm trong State Machine hay adapter riêng?
