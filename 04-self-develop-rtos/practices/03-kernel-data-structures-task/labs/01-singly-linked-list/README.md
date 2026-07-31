# Lab 01 - Singly Linked List

## Mục tiêu

- Cài singly linked list với push front/back, pop front, find, count và cycle detection.
- Quan sát kết quả bằng output hoặc test thay vì chỉ đọc source code.
- Ghi lại một trường hợp lỗi có chủ đích và giải thích nguyên nhân.

## Build và chạy

Makefile của lab này chỉ quản lý source, test và executable của chính lab. Từ thư mục root:

```bash
cd labs/01-singly-linked-list
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

1. Head, tail và count phải thỏa những invariant nào?
2. Operation nào của singly linked list là O(1), operation nào là O(n)?
3. Một cycle trong list làm traversal bị ảnh hưởng như thế nào?
4. Singly linked list phù hợp với cấu trúc nào trong kernel?
