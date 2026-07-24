# Lab 08 - Per-Priority Ready Queues

## Mục tiêu

- Quản lý FIFO ready queue theo priority, queue rotation và ready bitmap.
- Quan sát kết quả bằng output hoặc test thay vì chỉ đọc source code.
- Ghi lại một trường hợp lỗi có chủ đích và giải thích nguyên nhân.

## Build và test

Makefile của lab này chỉ quản lý source, test và executable của chính lab. Từ thư mục root:

```bash
cd labs/08-priority-ready-queues
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

1. Ready bitmap phải nhất quán với các queue như thế nào?
2. Vì sao task mới READY được thêm vào tail?
3. Scheduler chọn task priority cao nhất theo quy ước nào?
4. Điều gì xảy ra nếu bitmap có bit set nhưng queue tương ứng rỗng?
