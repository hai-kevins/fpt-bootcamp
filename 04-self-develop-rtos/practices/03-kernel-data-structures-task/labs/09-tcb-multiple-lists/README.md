# Lab 09 - TCB in Multiple Lists

## Mục tiêu

- Cho cùng một TCB nằm trong registry và ready queue bằng hai intrusive node khác nhau.
- Quan sát kết quả bằng output hoặc test thay vì chỉ đọc source code.
- Ghi lại một trường hợp lỗi có chủ đích và giải thích nguyên nhân.

## Build và test

Makefile của lab này chỉ quản lý source, test và executable của chính lab. Từ thư mục root:

```bash
cd labs/09-tcb-multiple-lists
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

1. Tại sao một TCB cần nhiều list node?
2. Một node intrusive có thể thuộc bao nhiêu list tại một thời điểm?
3. Nếu dùng `ready_node` cho cả registry và ready queue thì lỗi gì xảy ra?
4. `container_of` phải dùng đúng member như thế nào?
