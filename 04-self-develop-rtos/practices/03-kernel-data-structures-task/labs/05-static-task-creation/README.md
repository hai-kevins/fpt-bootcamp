# Lab 05 - Static Task Creation

## Mục tiêu

- Tạo task từ TCB và stack tĩnh, validate input và ngăn duplicate creation.
- Quan sát kết quả bằng output hoặc test thay vì chỉ đọc source code.
- Ghi lại một trường hợp lỗi có chủ đích và giải thích nguyên nhân.

## Build và chạy

Makefile của lab này chỉ quản lý source, test và executable của chính lab. Từ thư mục root:

```bash
cd labs/05-static-task-creation
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

1. Static task creation cần validate những input nào?
2. Vì sao task creation không nên cấp phát động ngầm?
3. TCB phải ở trạng thái nào nếu creation thất bại?
4. Task magic giúp phát hiện lỗi gì?
