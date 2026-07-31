# Lab 06 - GPIO Register-Level

## Mục tiêu

- Bật clock GPIOC qua `RCC_APB2ENR`.
- Cấu hình PC13 thành output push-pull 2 MHz trong `GPIOC_CRH`.
- Set/reset output bằng `GPIOC_BSRR`.
- Hiểu LED PC13 active-low trên Blue Pill.
- Tách driver GPIO khỏi `main.c`.

## Build và chạy

Makefile của lab này chỉ build firmware của chính lab. Từ thư mục root:

```bash
cd labs/06-gpio-register
make
```

Các lệnh thường dùng:

```bash
make              # Build ELF, BIN, HEX, MAP và LST
make size         # In kích thước firmware
make report       # Tạo báo cáo
make flash-stlink # Flash đúng firmware Lab 06
make clean        # Chỉ xóa build/ của Lab 06
```

## Kết quả

Artifact chính:

```text
build/lab06_gpio_register.elf
```

Sau khi flash, LED PC13 nháy liên tục.

```text
PC13 = 0 -> LED sáng
PC13 = 1 -> LED tắt
```

Driver sử dụng `GPIOC_BSRR` để set/reset bit mà không cần read-modify-write toàn bộ `GPIOC_ODR`.

## Câu hỏi

1. Vì sao phải bật peripheral clock trước khi cấu hình GPIO?
2. Nibble cấu hình PC13 nằm ở vị trí nào trong `GPIOC_CRH`?
3. Vì sao `BSRR` an toàn hơn read-modify-write `ODR` trong một số tình huống?
4. Nếu chọn sai MODE/CNF, LED hoặc chân GPIO có thể hoạt động thế nào?
