# Lab 13 - Fatal Record

## Mục tiêu

- Lưu error code, task ID, signal, timestamp và restart count.
- Kiểm tra integrity bằng checksum.
- Mô phỏng record còn lại qua reset.

## Build và chạy

Makefile của lab chỉ quản lý source và executable của chính lab.

```bash
cd labs/13-fatal-record
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

Fatal record hợp lệ sau capture; boot tăng restart count và checksum vẫn đúng.

## Câu hỏi

1. Vùng `.noinit` có đảm bảo giữ dữ liệu khi mất nguồn không?
2. Fatal handler được phép gọi những API nào?
3. Vì sao cần build ID trong production record?
4. Khi nào record nên được clear?
