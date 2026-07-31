# Lab 03 - Start First Task Using SVC

## Mục tiêu

- Khởi động task đầu tiên từ `main()` bằng SVC và exception return.
- Xác nhận task chạy ở Thread mode bằng PSP còn handler dùng MSP.
- Ghi lại một lỗi initial frame hoặc EXC_RETURN có chủ đích và giải thích HardFault.

## Build và chạy

Makefile của lab này chỉ build firmware của chính lab. Từ thư mục root:

```bash
cd labs/03-svc-first-task
make
```

Các lệnh thường dùng:

```bash
make size
make disasm
make flash-stlink
make clean
```

Nếu không có GNU Arm Embedded Toolchain, Makefile tự chuyển sang Clang/LLD.

## Kết quả

Artifact chính nằm cục bộ trong lab:

```text
build/lab03_svc_first_task.elf
```

Flash firmware, mở UART 9600 baud và kiểm tra argument, PSP và MSP được in từ task đầu tiên. LED PC13 tiếp tục toggle trong task.

## Câu hỏi

1. Vì sao không gọi trực tiếp task entry từ `main()`?
2. SVC Handler phải restore phần context nào trước khi exception return?
3. Tại sao EXC_RETURN `0xFFFFFFFD` được dùng để vào task?
4. Làm cách nào dùng GDB để chứng minh task đang dùng PSP?
