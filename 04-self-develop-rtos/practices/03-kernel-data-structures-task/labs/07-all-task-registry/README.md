# Lab 07 - All-Task Registry

## Mục tiêu

- Lưu, duyệt, đếm và tìm task bằng intrusive all-task list.
- Quan sát kết quả bằng output hoặc test thay vì chỉ đọc source code.
- Ghi lại một trường hợp lỗi có chủ đích và giải thích nguyên nhân.

## Build và test

Makefile của lab này chỉ quản lý source, test và executable của chính lab. Từ thư mục root:

```bash
cd labs/07-all-task-registry
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

1. All-task registry khác ready queue ở mục đích nào?
2. Vì sao registry phải dùng `all_task_node` thay vì `ready_node`?
3. Lookup theo name có độ phức tạp bao nhiêu với implementation hiện tại?
4. Task name phải có lifetime như thế nào?
