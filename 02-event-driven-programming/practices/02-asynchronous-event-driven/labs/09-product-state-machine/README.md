# Lab 09 - Product State Machine

## Mục tiêu

- Mô hình hóa BOOT, IDLE, ACTIVE và ERROR.
- Xử lý SYSTEM_START, BUTTON_PRESSED, SENSOR_READY, ERROR_DETECTED và RESET_REQUEST.
- Đếm transition thực sự.
- Test invalid/unhandled event không làm state đổi sai.

## Build và test

```bash
cd labs/09-product-state-machine
make
make test
make run
```

## Kết quả

Sequence mẫu kết thúc ở IDLE sau khi đi qua ACTIVE và ERROR.

Test phải báo:

```text
[PASS] product-state-machine
```

## Câu hỏi

1. SENSOR_READY được xử lý khác nhau ở IDLE và ACTIVE ra sao?
2. Vì sao ERROR chỉ thoát bằng RESET_REQUEST?
3. Entry/exit action nên được đặt ở đâu?
4. Làm sao tạo transition coverage matrix?
