# Lab 01 - Cortex-M3 Exception and Stack Inspection

## Mục tiêu

- Quan sát MSP, PSP, EXC_RETURN và 8 word hardware exception frame khi thực thi SVC.
- Phân biệt stack pointer được CPU sử dụng trong Thread mode và Handler mode.
- Ghi lại một trường hợp đọc sai stack frame có chủ đích và giải thích nguyên nhân.

## Build và chạy

Makefile của lab này chỉ build firmware của chính lab. Từ thư mục root:

```bash
cd labs/01-exception-stack-inspection
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
build/lab01_exception_stack.elf
```

Flash file `.bin`, mở UART 9600 baud và kiểm tra các giá trị `MSP frame`, `PSP`, `EXC_RETURN` cùng `frame[0]` đến `frame[7]`.

## Câu hỏi

1. Vì sao SVC frame của lab này được tạo trên MSP thay vì PSP?
2. Các vị trí `frame[0]` đến `frame[7]` tương ứng những register nào?
3. Giá trị `0xFFFFFFF9` của EXC_RETURN biểu diễn điều gì?
4. Nếu chọn sai stack pointer để đọc frame thì kết quả sẽ sai như thế nào?
