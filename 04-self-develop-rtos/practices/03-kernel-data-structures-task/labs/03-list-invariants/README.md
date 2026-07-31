# Lab 03 - List Invariants and Ownership

## Mục tiêu

- Kiểm tra owner, duplicate insertion, wrong-list removal, count và pointer corruption.
- Quan sát kết quả bằng output hoặc test thay vì chỉ đọc source code.
- Ghi lại một trường hợp lỗi có chủ đích và giải thích nguyên nhân.

## Build và chạy

Makefile của lab này chỉ quản lý source, test và executable của chính lab. Từ thư mục root:

```bash
cd labs/03-list-invariants
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

1. Field `owner` ngăn một node thuộc hai list như thế nào?
2. Duplicate insertion có thể làm hỏng list ra sao?
3. Validator cần kiểm tra những quan hệ nào giữa head, tail và count?
4. Full list validation có phù hợp để chạy trong PendSV không? Vì sao?
