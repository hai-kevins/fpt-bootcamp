# Lab 04 - TCB Layout and Static Assertions

## Mục tiêu

- Thiết kế TCB, quan sát `sizeof`/`offsetof` và khóa `saved_sp` tại offset 0.
- Quan sát kết quả bằng output hoặc test thay vì chỉ đọc source code.
- Ghi lại một trường hợp lỗi có chủ đích và giải thích nguyên nhân.

## Build và test

Makefile của lab này chỉ quản lý source, test và executable của chính lab. Từ thư mục root:

```bash
cd labs/04-tcb-layout
make test
make run
```

Có thể dùng:

```bash
make       # Build demo và chạy test
make test  # Chỉ build/chạy unit test
make run   # Build rồi chạy demo
make clean # Chỉ xóa build/ của lab này
```

## Kết quả

Artifact chính nằm cục bộ trong lab:

```text
build/demo
```

Chạy trực tiếp bằng `./build/demo` hoặc `make run`.

## Câu hỏi

1. Tại sao `saved_sp` phải giữ contract với context-switch assembly?
2. `offsetof` giúp phát hiện thay đổi layout như thế nào?
3. Những field nào thuộc architecture context và field nào thuộc scheduler?
4. Điều gì xảy ra nếu thêm một field trước `saved_sp`?
