# Lab 04 - Button Debounce

## Mục tiêu

- Dùng PA0 active-low với pull-up nội và EXTI0.
- Giữ ISR ngắn: clear cờ và post `BUTTON_EDGE`.
- Restart software timer 20 ms khi có cạnh.
- Chỉ phát hiện stable pressed/released sau timeout.
- Dùng LED PC13 để quan sát stable button state.

## Build

```bash
cd labs/04-button-debounce
make
```

Hoặc khi chỉ có Clang/LLD:

```bash
make TOOLCHAIN=clang
```

Flash:

```bash
make flash-stlink
```

Kết nối:

```text
PA0 ---- Button ---- GND
```

## Kết quả

- Nhấn button: sau debounce, LED PC13 sáng.
- Thả button: sau debounce, LED PC13 tắt.
- Bounce trong cửa sổ 20 ms chỉ restart timer, không đổi state ngay.
- Artifact nằm trong `build/` của Lab 04.

## Câu hỏi

1. Vì sao ISR không nên delay 20 ms?
2. Vì sao mỗi cạnh cần restart timer thay vì tạo nhiều timer mới?
3. Stable state khác raw GPIO state ở đâu?
4. Queue overflow trong ISR nên được xử lý theo policy nào?
