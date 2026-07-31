# Lab 08 - Map File Analysis

## Mục tiêu

- Đọc map file do linker tạo.
- Phân biệt Flash usage và static SRAM usage.
- Tìm section, symbol, VMA và LMA.
- Xác định object chiếm nhiều bộ nhớ nhất.
- Hiểu giới hạn của số liệu từ `arm-none-eabi-size`.
- Tạo báo cáo có thể dùng cho memory budget.

## Build và chạy

Makefile của lab này chỉ build firmware và báo cáo của chính lab. Từ thư mục root:

```bash
cd labs/08-map-analysis
make
make report
./analyze.sh
```

Các lệnh thường dùng:

```bash
make              # Build ELF, BIN, HEX, MAP và LST
make size         # In text/data/bss
make sections     # In section headers
make symbols      # In symbol theo kích thước
make report       # Tạo báo cáo trong build/
./analyze.sh      # Tổng hợp thông tin phân tích
make clean        # Chỉ xóa build/ của Lab 08
```

## Kết quả

Artifact chính:

```text
build/lab08_map_analysis.elf
```

Các object được tạo với kích thước dễ quan sát:

| Object | Section dự kiến | Kích thước |
|---|---|---:|
| `g_lookup_table` | `.rodata` | 256 bytes |
| `g_initialized_statistics` | `.data` | 64 bytes |
| `g_rx_buffer` | `.bss` | 512 bytes |
| `g_framebuffer` | `.bss` | 1024 bytes |

Các file phân tích:

```text
build/lab08_map_analysis.map
build/lab08_map_analysis.lst
build/size-report.txt
build/section-report.txt
build/symbol-report.txt
```

## Câu hỏi

1. Vì sao `g_initialized_statistics` làm tăng cả Flash và SRAM?
2. Vì sao `g_framebuffer` làm tăng `.bss` nhưng không làm file `.bin` tăng tương ứng 1024 byte?
3. Symbol nào chiếm nhiều bộ nhớ nhất và làm sao xác nhận bằng `nm`?
4. Vì sao `arm-none-eabi-size` chưa phản ánh đầy đủ stack usage tại runtime?
