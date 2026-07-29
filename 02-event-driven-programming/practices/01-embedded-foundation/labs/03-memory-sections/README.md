# Lab 03 - Memory Sections

## Mục tiêu

- Khảo sát `.text`, `.rodata`, `.data` và `.bss` trong executable host.
- Liên hệ loại biến với section dự kiến.
- Sử dụng `nm`, `objdump` và `readelf` để quan sát symbol và section.
- So sánh kết quả host với firmware ARM ở project root.
- Ghi lại một thay đổi có chủ đích và giải thích vì sao symbol chuyển section.

## Build và chạy

Makefile của lab này chỉ quản lý source, executable và báo cáo của chính lab. Từ thư mục root:

```bash
cd labs/03-memory-sections
make
make run
make report
```

Có thể dùng:

```bash
make              # Build executable
make run          # Build rồi chạy
make symbols      # In symbol
make sections     # In section bằng objdump
make elf-sections # In section bằng readelf
make report       # Ghi báo cáo vào build/
make clean        # Chỉ xóa build/ của Lab 03
```

## Kết quả

Artifact chính:

```text
build/lab03_memory_sections
```

Các object dự kiến:

| Object | Section dự kiến |
|---|---|
| `g_firmware_version` | `.rodata` |
| `g_initialized_counter` | `.data` |
| `g_uninitialized_counter` | `.bss` |
| `g_static_buffer` | `.bss` |
| `g_private_value` | `.data` |

Tên section và cách tối ưu có thể khác đôi chút tùy compiler/linker. Kết luận phải dựa trên output thật của `nm`, `objdump` và `readelf`.

## Câu hỏi

1. Vì sao biến đã khởi tạo khác 0 thường nằm trong `.data`?
2. Vì sao mảng zero-initialized lớn không cần lưu toàn bộ byte 0 trong file executable?
3. Khi thêm `const` vào một biến global, section và khả năng sửa ở runtime thay đổi thế nào?
4. `static` quyết định linkage/lifetime hay trực tiếp quyết định `.data` và `.bss`?
