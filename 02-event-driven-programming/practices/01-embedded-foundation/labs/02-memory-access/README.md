# Lab 02 - Memory Access

## Mục tiêu

- Ép một địa chỉ kiểu `uintptr_t` thành con trỏ 32-bit.
- Đọc và ghi dữ liệu thông qua phép dereference.
- Hiểu vai trò của `volatile` trong memory access.
- Phân biệt địa chỉ hợp lệ và địa chỉ tùy ý.
- Liên hệ thao tác con trỏ với Memory-Mapped I/O trên MCU.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable host của chính lab. Từ thư mục root:

```bash
cd labs/02-memory-access
make
make run
```

Có thể dùng:

```bash
make         # Build executable
make run     # Build rồi chạy
make rebuild # Clean rồi build lại
make clean   # Chỉ xóa build/ của Lab 02
```

## Kết quả

Artifact chính nằm cục bộ trong lab:

```text
build/lab02_memory_access
```

Kết quả chính:

```text
initial value = 0x12345678
after write = 0xABCDEF01
```

Lab chỉ truy cập địa chỉ của biến `test_value` hợp lệ. Không thay địa chỉ bằng một số ngẫu nhiên vì executable host có thể gặp segmentation fault; trên Cortex-M, truy cập sai có thể gây HardFault hoặc BusFault.

## Câu hỏi

1. Vì sao lab dùng `uintptr_t` thay vì `uint32_t` để chứa địa chỉ trên host?
2. `volatile` ngăn compiler thực hiện những tối ưu nào đối với memory access?
3. Điều gì có thể xảy ra nếu truy cập một địa chỉ không căn chỉnh theo 4 byte?
4. Memory-Mapped I/O khác SRAM thông thường ở đâu dù cú pháp dereference giống nhau?
