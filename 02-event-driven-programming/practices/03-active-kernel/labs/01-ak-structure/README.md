# Lab 01 - AK Structure

## Mục tiêu

- Đọc cấu trúc task, message, timer, FSM và TSM.
- Quan sát kích thước các cấu trúc tối thiểu.
- Giải thích priority và run-to-completion.

## Build và chạy

Makefile của lab chỉ quản lý source và executable của chính lab.

```bash
cd labs/01-ak-structure
make test
make run
```

Có thể dùng:

```bash
make       # Build và chạy test/demo
make test  # Chạy kiểm tra
make run   # Chạy demo
make clean # Chỉ xóa build/ của lab hiện tại
```

## Kết quả

Executable in ra kích thước cấu trúc, ba loại message và hai loại timer.

## Câu hỏi

1. Task cần lưu những trường tối thiểu nào?
2. Priority cao hơn được chọn trước có hệ quả gì?
3. FSM function-based và TSM khác nhau ở đâu?
4. Cấu trúc nào cần static allocation trong kernel?
