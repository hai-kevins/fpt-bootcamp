# Labs — Independent Makefiles

Mỗi thư mục lab là một build unit độc lập.

- Không build lab từ Makefile gốc.
- Đi vào đúng thư mục lab rồi chạy Makefile của lab đó.
- Output được tạo trong `build/` bên trong chính lab.
- `make clean` chỉ xóa output của lab hiện tại.
- Host lab tạo executable chạy trên Linux.
- Target lab tạo firmware ARM để flash lên STM32F103.
- `make flash-stlink` phải được chạy trong đúng target lab cần nạp.

## Host labs

```bash
cd labs/01-endianness
make
make run

cd ../02-memory-access
make
make run

cd ../03-memory-sections
make
make run
make report
```

Host labs:

| Lab | Artifact chính | Quan sát |
|---:|---|---|
| 01 | `build/lab01_endianness` | Byte order |
| 02 | `build/lab02_memory_access` | Đọc/ghi qua địa chỉ |
| 03 | `build/lab03_memory_sections` | Section và symbol của executable host |

Các executable host không thể flash lên STM32.

## Target labs

```bash
cd labs/04-startup-code
make
make flash-stlink

cd ../05-linker-script
make
make report

cd ../06-gpio-register
make
make flash-stlink

cd ../07-uart-polling
make
make flash-stlink

cd ../08-map-analysis
make
make report
./analyze.sh
```

Target labs:

| Lab | Artifact chính | Quan sát |
|---:|---|---|
| 04 | `build/lab04_startup_code.elf` | LED báo kết quả khởi tạo `.data`/`.bss` |
| 05 | `build/lab05_linker_script.elf` | Map file, VMA/LMA và `.noinit` |
| 06 | `build/lab06_gpio_register.elf` | LED PC13 nháy |
| 07 | `build/lab07_uart_polling.elf` | UART echo và LED toggle |
| 08 | `build/lab08_map_analysis.elf` | Size, section, symbol và map file |

Mỗi target lab có Startup Code, Linker Script và Makefile riêng. Vì vậy, lệnh flash phải được thực hiện trong thư mục của lab tương ứng.
