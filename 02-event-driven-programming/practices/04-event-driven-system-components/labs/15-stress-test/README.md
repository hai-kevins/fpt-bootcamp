# Lab 15 - Mailbox và Event Pool Stress Test

## Mục tiêu

- Tạo 1000 lần thử allocate/post dynamic event.
- Chạy scheduler định kỳ để tiêu thụ mailbox.
- Theo dõi posted count, handled count, mailbox high-water và pool failure.
- Xác nhận không leak event sau khi hệ thống idle.

## Build và test

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/15-stress-test
make test
make run
```

Có thể dùng:

```bash
make          # Build executable
make test     # Build rồi chạy bài kiểm tra
make run      # Build rồi chạy demo/test
make sanitize # Chạy với ASan/UBSan
make clean    # Chỉ xóa build/ của lab này
```

## Kết quả

Artifact chính nằm cục bộ trong lab:

```text
build/lab
```

Chương trình PASS khi `posted == handled` và `event_pool.used_count == 0`. Output in thêm mailbox high-water mark và allocation failure count để đánh giá tải.

## Câu hỏi

1. Vì sao stress test cần kiểm tra cả throughput và ownership invariant?
2. Mailbox high-water mark cho biết điều gì về consumer latency?
3. Allocation failure bằng 0 có luôn chứng minh pool đủ lớn không?
4. Soak test dài hạn cần bổ sung counter và điều kiện PASS nào?
