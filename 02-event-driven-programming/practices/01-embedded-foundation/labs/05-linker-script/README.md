# Lab 05 - Linker Script

## Mục tiêu

- Định nghĩa vùng Flash và SRAM bằng lệnh `MEMORY`.
- Đặt `.isr_vector`, `.text`, `.rodata`, `.data`, `.bss` và `.noinit`.
- Hiểu sự khác nhau giữa VMA và LMA của `.data`.
- Tạo các linker symbol phục vụ Startup Code.
- Quan sát tác động của `KEEP()`, `NOLOAD` và `AT > FLASH`.

## Build và chạy

Makefile của lab này chỉ build firmware và báo cáo của chính lab. Từ thư mục root:

```bash
cd labs/05-linker-script
make
make report
```

Các lệnh thường dùng:

```bash
make              # Build ELF, BIN, HEX, MAP và LST
make size         # In kích thước firmware
make sections     # In section headers
make symbols      # In symbol
make report       # Tạo size/section/symbol report
make flash-stlink # Flash Lab 05 nếu cần quan sát bằng debugger
make clean        # Chỉ xóa build/ của Lab 05
```

## Kết quả

Artifact chính:

```text
build/lab05_linker_script.elf
```

Các object được thiết kế để xuất hiện ở:

| Object | Section |
|---|---|
| `g_firmware_version` | `.rodata` |
| `g_initialized_counter` | `.data` |
| `g_uninitialized_counter` | `.bss` |
| `g_work_buffer` | `.bss` |
| `g_reset_record` | `.noinit` |

Phân tích:

```bash
arm-none-eabi-objdump -h build/lab05_linker_script.elf
arm-none-eabi-nm -n build/lab05_linker_script.elf
less build/lab05_linker_script.map
```

## Câu hỏi

1. Vì sao `.data` có địa chỉ load trong Flash nhưng địa chỉ chạy trong SRAM?
2. `.noinit` khác `.bss` ở bước Startup Code như thế nào?
3. Điều gì có thể xảy ra nếu bỏ `KEEP(*(.isr_vector))` khi dùng `--gc-sections`?
4. Linker phải báo gì khi tổng section vượt quá kích thước Flash hoặc SRAM?
