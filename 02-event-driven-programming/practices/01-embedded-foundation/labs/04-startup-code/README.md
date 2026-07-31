# Lab 04 - Startup Code

## Mục tiêu

- Tạo Vector Table cho Cortex-M3.
- Thiết lập Initial Main Stack Pointer bằng symbol `_estack`.
- Đi từ reset vào `Reset_Handler`.
- Copy `.data` từ Flash sang SRAM.
- Clear `.bss` trước khi gọi `main()`.
- Kiểm tra kết quả khởi tạo bằng biến probe và LED PC13.

## Build và chạy

Makefile của lab này chỉ build firmware của chính lab. Từ thư mục root:

```bash
cd labs/04-startup-code
make
```

Các lệnh thường dùng:

```bash
make              # Build ELF, BIN, HEX, MAP và LST
make size         # In kích thước firmware
make sections     # In section headers
make symbols      # In symbol theo kích thước
make report       # Ghi các báo cáo vào build/
make flash-stlink # Flash Lab 04 bằng ST-Link
make clean        # Chỉ xóa build/ của Lab 04
```

## Kết quả

Artifact chính nằm cục bộ trong lab:

```text
build/lab04_startup_code.elf
```

Sau khi flash:

- LED PC13 nháy chậm khi `.data` giữ đúng `0x12345678` và toàn bộ `.bss` ban đầu bằng 0.
- LED PC13 nháy nhanh khi kiểm tra Startup Code thất bại.

Có thể đặt breakpoint tại:

```text
Reset_Handler
main
Default_Handler
```

và quan sát:

```text
_sidata
_sdata
_edata
_sbss
_ebss
_estack
```

## Câu hỏi

1. CPU lấy Initial MSP và địa chỉ `Reset_Handler` từ đâu?
2. Vì sao `.data` cần được copy nhưng `.bss` chỉ cần được clear?
3. Invariant nào phải đúng trước khi `main()` được gọi?
4. Nếu bỏ vòng clear `.bss`, lỗi nào có thể xuất hiện trong application?
