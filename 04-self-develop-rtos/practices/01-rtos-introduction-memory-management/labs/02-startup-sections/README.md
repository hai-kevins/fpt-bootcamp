# Lab 02 - Startup Sections

## Mục tiêu

- Kiểm tra Reset_Handler đã copy `.data` và clear `.bss` đúng.
- Quan sát kết quả bằng output hoặc test thay vì chỉ đọc source code.
- Ghi lại một trường hợp lỗi có chủ đích và giải thích nguyên nhân.

## Build và chạy

Makefile của lab này chỉ build firmware của chính lab. Từ thư mục root:

```bash
cd labs/02-startup-sections
make
# Hoặc khi chỉ có Clang/LLD:
make TOOLCHAIN=clang
```

Các lệnh thường dùng:

```bash
make size
make flash-stlink
make clean
```

## Kết quả

Artifact chính nằm cục bộ trong lab:

```text
build/02_startup_sections.elf
```

Flash file `.bin` bằng ST-Link rồi mở UART 9600 baud.

## Câu hỏi

1. Cơ chế mới của lab này giải quyết vấn đề gì?
2. Invariant nào phải luôn đúng?
3. Khi bỏ cơ chế này, lỗi nào có thể xuất hiện?
4. Cơ chế này có phù hợp cho hard real-time path không? Vì sao?
