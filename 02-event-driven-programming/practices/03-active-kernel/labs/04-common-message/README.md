# Lab 04 - Common Message

## Mục tiêu

- Gửi struct player status trong payload cố định.
- Kiểm tra `len` trước khi đọc.
- Quan sát lỗi khi length không đúng.

## Build và chạy

Makefile của lab chỉ quản lý source và executable của chính lab.

```bash
cd labs/04-common-message
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

Payload hợp lệ được copy; payload thiếu một byte bị từ chối.

## Câu hỏi

1. Vì sao phải kiểm tra length?
2. Alignment và padding ảnh hưởng protocol thế nào?
3. Struct nội bộ có nên truyền trực tiếp qua network không?
4. Common payload nên có giới hạn bao nhiêu?
