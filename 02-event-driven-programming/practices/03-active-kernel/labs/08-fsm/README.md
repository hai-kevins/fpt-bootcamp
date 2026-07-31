# Lab 08 - FSM

## Mục tiêu

- Xây function-based FSM với IDLE/RUNNING/PAUSED/ERROR.
- Kiểm tra transition hợp lệ.
- Giữ state trong module owner.

## Build và chạy

Makefile của lab chỉ quản lý source và executable của chính lab.

```bash
cd labs/08-fsm
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

Chuỗi START, PAUSE, RESUME, ERROR, RESET đưa FSM qua đúng state và quay về IDLE.

## Câu hỏi

1. Unhandled signal nên được xử lý thế nào?
2. Entry/exit action đặt ở đâu?
3. Timer thuộc state phải được hủy khi nào?
4. Function-based FSM dễ mở rộng tới mức nào?
