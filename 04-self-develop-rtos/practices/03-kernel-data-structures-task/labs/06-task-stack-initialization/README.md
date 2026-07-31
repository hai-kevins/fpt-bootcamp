# Lab 06 - Task Stack Initialization

## Mục tiêu

- Tạo initial Cortex-M frame, stack fill pattern, guard word và high-water report.
- Quan sát kết quả bằng output hoặc test thay vì chỉ đọc source code.
- Ghi lại một trường hợp lỗi có chủ đích và giải thích nguyên nhân.

## Build và chạy

Makefile của lab này chỉ quản lý source, test và executable của chính lab. Từ thư mục root:

```bash
cd labs/06-task-stack-initialization
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

1. Initial frame mô phỏng trạng thái nào của một task chưa từng chạy?
2. Tại sao xPSR phải có Thumb bit?
3. Guard word và fill pattern phát hiện hai loại vấn đề khác nhau như thế nào?
4. `saved_sp` phải thỏa điều kiện range và alignment nào?
