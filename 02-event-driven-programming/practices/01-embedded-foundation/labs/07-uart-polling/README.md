# Lab 07 - UART Polling

## Mục tiêu

- Cấu hình PA9 thành USART1 TX.
- Cấu hình PA10 thành USART1 RX.
- Cấu hình USART1 ở 9600 baud, 8-N-1.
- Poll cờ `TXE` khi gửi và `RXNE` khi nhận.
- Echo byte nhận được và toggle LED PC13.
- Hiểu giới hạn của polling trong firmware có nhiều tác vụ.

## Build

Makefile của lab này chỉ build firmware của chính lab. Từ thư mục root:

```bash
cd labs/07-uart-polling
make
```

Các lệnh thường dùng:

```bash
make              # Build ELF, BIN, HEX, MAP và LST
make size         # In kích thước firmware
make report       # Tạo báo cáo
make flash-stlink # Flash đúng firmware Lab 07
make clean        # Chỉ xóa build/ của Lab 07
```

Kết nối:

```text
STM32F103       USB-UART
PA9  TX   --->  RX
PA10 RX   <---  TX
GND        ---  GND
```

## Kết quả

Artifact chính:

```text
build/lab07_uart_polling.elf
```

Mở UART:

```bash
picocom -b 9600 /dev/ttyUSB0
```

Banner dự kiến:

```text
Lab 07 - USART1 polling
PA9=TX, PA10=RX, 9600 8-N-1
Every received byte is echoed.
```

Khi gửi một ký tự, firmware trả về:

```text
RX: <ký tự>
```

và LED PC13 đổi trạng thái.

## Câu hỏi

1. `TXE` và `RXNE` biểu diễn điều kiện gì?
2. Baud-rate register phụ thuộc vào clock peripheral như thế nào?
3. Polling có thể làm mất dữ liệu khi application thực hiện công việc dài ra sao?
4. Khi chuyển sang Event-Driven, ISR, ring buffer và UART service nên chia trách nhiệm như thế nào?
