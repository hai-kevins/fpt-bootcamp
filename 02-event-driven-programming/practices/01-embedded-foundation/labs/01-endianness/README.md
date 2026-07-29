# Lab 01 - Endianness

## Mục tiêu

- Quan sát cách giá trị `0x12345678` được chia thành bốn byte trong bộ nhớ.
- Hiểu quan hệ giữa biến, địa chỉ và con trỏ `uint8_t`.
- Phân biệt little-endian và big-endian.
- Quan sát kết quả bằng output thay vì chỉ suy luận từ source code.
- Thay đổi giá trị đầu vào và giải thích vì sao thứ tự byte thay đổi hoặc không thay đổi.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable host của chính lab. Từ thư mục root:

```bash
cd labs/01-endianness
make
make run
```

Có thể dùng:

```bash
make         # Build executable
make run     # Build rồi chạy
make rebuild # Clean rồi build lại
make clean   # Chỉ xóa build/ của Lab 01
```

## Kết quả

Artifact chính nằm cục bộ trong lab:

```text
build/lab01_endianness
```

Trên hệ thống little-endian, phần byte dự kiến có dạng:

```text
byte[0] = 0x78
byte[1] = 0x56
byte[2] = 0x34
byte[3] = 0x12
Detected byte order: little-endian
```

Địa chỉ in ra thay đổi giữa các lần chạy. Điều cần quan sát là byte có trọng số thấp nhất `0x78` nằm tại địa chỉ thấp nhất.

## Câu hỏi

1. Endianness có thay đổi giá trị logic của biến `value` không?
2. Vì sao `uint8_t *` có thể được dùng để quan sát từng byte của một object?
3. Nếu đổi `value` thành `0xA1B2C3D4`, bốn byte được in theo thứ tự nào trên little-endian?
4. Khi truyền một số 32-bit giữa hai thiết bị, protocol phải quy định byte order như thế nào?
