# Lab 09 - TSM

## Mục tiêu

- Viết lại FSM bằng transition table.
- Tách dữ liệu transition khỏi engine.
- So sánh readability với switch-case.

## Build và chạy

Makefile của lab chỉ quản lý source và executable của chính lab.

```bash
cd labs/09-tsm
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

Transition table xử lý cùng chuỗi sự kiện như Lab 08 và kết thúc ở IDLE.

## Câu hỏi

1. Table giúp review transition coverage thế nào?
2. Action và guard được thêm vào row ra sao?
3. TSM có phù hợp với hierarchical state không?
4. Khi số state lớn, cách nào dễ bảo trì hơn?
