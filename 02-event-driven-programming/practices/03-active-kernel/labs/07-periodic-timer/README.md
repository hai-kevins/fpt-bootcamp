# Lab 07 - Periodic Timer

## Mục tiêu

- Tạo LED heartbeat bằng periodic timer.
- Thay đổi chu kỳ 100/500/1000 ms.
- Bù nhiều expiration khi scheduler xử lý trễ.

## Build và chạy

Makefile của lab chỉ quản lý source và executable của chính lab.

```bash
cd labs/07-periodic-timer
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

Counter tăng đúng theo số deadline đã đi qua và chu kỳ mới có hiệu lực.

## Câu hỏi

1. Periodic deadline nên cộng period hay gán now+period?
2. Cách nào giảm drift?
3. Nếu handler chậm hơn period thì hệ thống nên làm gì?
4. Shell thay chu kỳ có cần restart timer không?
