# Lab 11 - Pool Stress Test

## Mục tiêu

- Tạo burst message vượt tốc độ dispatch.
- Đo posted, post failure, allocation failure.
- Theo dõi pool và queue high-water mark.

## Build và test

Makefile của lab chỉ quản lý source và executable của chính lab.

```bash
cd labs/11-pool-stress-test
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

Stress test chạy 1000 vòng, kết thúc không leak và in đầy đủ counter.

## Câu hỏi

1. Queue full và pool exhausted khác nhau thế nào?
2. High-water mark dùng để sizing ra sao?
3. Tăng pool có luôn sửa đúng lỗi không?
4. Handler latency cần đo ở đâu?
