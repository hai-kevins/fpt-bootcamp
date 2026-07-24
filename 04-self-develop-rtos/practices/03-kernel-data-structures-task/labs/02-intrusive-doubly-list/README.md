# Lab 02 - Intrusive Doubly Linked List

## Mục tiêu

- Cài intrusive doubly list với insert/remove O(1), duyệt hai chiều và `container_of`.
- Quan sát kết quả bằng output hoặc test thay vì chỉ đọc source code.
- Ghi lại một trường hợp lỗi có chủ đích và giải thích nguyên nhân.

## Build và test

Makefile của lab này chỉ quản lý source, test và executable của chính lab. Từ thư mục root:

```bash
cd labs/02-intrusive-doubly-list
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

1. Intrusive list khác non-intrusive list ở đâu?
2. Tại sao remove một known node có thể thực hiện O(1)?
3. `container_of` lấy object chứa node bằng cách nào?
4. Nếu quên cập nhật backward link thì validator phát hiện ra sao?
