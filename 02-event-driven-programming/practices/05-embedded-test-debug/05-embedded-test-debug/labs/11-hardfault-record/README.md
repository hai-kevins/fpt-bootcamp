# Lab 11 - HardFault Record

## Mục tiêu

- Mô hình hóa PC, LR, xPSR và Fault Status Register.
- Kiểm tra record có đầy đủ register bắt buộc.
- Tạo địa chỉ PC để dùng với addr2line.
- Hiểu giới hạn của mô phỏng host.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/11-hardfault-record
make
make run
```

Có thể dùng:

```bash
make          # Build executable
make run      # Build rồi chạy lab
make test     # Alias của make run
make sanitize # Chạy với ASan/UBSan
make clean    # Chỉ xóa build/ của lab hiện tại
```

## Kết quả

Artifact chính:

```text
build/lab
```

Output dự kiến:

```text
pc=0x08001234 lr=0xFFFFFFF9 xpsr=0x21000000 cfsr=0x00008200 PASS
```

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đúng.

## Câu hỏi

1. Stacked register được lấy từ MSP hay PSP như thế nào?
2. CFSR, HFSR, MMFAR và BFAR hỗ trợ phân tích gì?
3. Vì sao không nên printf dài trong HardFault handler?
4. Dùng `addr2line` với PC như thế nào?
