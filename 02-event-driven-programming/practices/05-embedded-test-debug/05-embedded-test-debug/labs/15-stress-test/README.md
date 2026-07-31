# Lab 15 - Stress Test

## Mục tiêu

- Tạo event burst.
- Đo drop count.
- Đo queue max và pool max.
- Theo dõi handler max time.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/15-stress-test
make
make run
```

Có thể dùng:

```bash
make          # Build executable
make run      # Build rồi chạy lab
make test     # Alias của make run
make sanitize # Chạy với ASan/UBSan
make clean    # Chỉ xóa build/ của lab hiện tại
```

## Kết quả

Artifact chính:

```text
build/lab
```

Output dự kiến:

```text
events=10000 drop=0 queue_max=16 pool_max=16 handler_max=900 PASS
```

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đúng.

## Câu hỏi

1. Stress test khác benchmark ở mục tiêu nào?
2. Producer rate ảnh hưởng queue max ra sao?
3. Drop bằng 0 có chứng minh hệ thống đủ tải không?
4. Handler max cần so với deadline như thế nào?
