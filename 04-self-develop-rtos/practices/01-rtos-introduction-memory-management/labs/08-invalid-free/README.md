# Lab 08 - Invalid Free Detection

## Mục tiêu

- Phát hiện pointer ngoài heap, pointer giữa payload và double free.
- Quan sát kết quả bằng output hoặc test thay vì chỉ đọc source code.
- Ghi lại một trường hợp lỗi có chủ đích và giải thích nguyên nhân.

## Build và chạy

Makefile của lab này chỉ quản lý source, test và executable của chính lab. Từ thư mục root:

```bash
cd labs/08-invalid-free
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
build/lab08-invalid-free
```

Chạy trực tiếp bằng `./build/lab08-invalid-free` hoặc `make run`.

## Câu hỏi

1. Cơ chế mới của lab này giải quyết vấn đề gì?
2. Invariant nào phải luôn đúng?
3. Khi bỏ cơ chế này, lỗi nào có thể xuất hiện?
4. Cơ chế này có phù hợp cho hard real-time path không? Vì sao?
