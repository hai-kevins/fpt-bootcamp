# Lab 15 - Mailbox và Event Pool Stress Test

## Mục tiêu

- Tạo 1000 lần thử allocate và post Dynamic Event.
- Chạy scheduler định kỳ để tiêu thụ mailbox.
- Theo dõi posted count, handled count, mailbox high-water mark và pool failure.
- Xác nhận không leak event sau khi hệ thống idle.
- Phân biệt stress test ngắn hạn và soak test dài hạn.

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

Artifact chính nằm cục bộ trong lab:

```text
build/lab
```

Output dự kiến:

```text
posted=1000 handled=1000 hwm=16 failures=0
```

Toàn bộ 1000 event được xử lý, không có allocation failure và Event Pool trở về trạng thái rỗng.

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đều đúng.

## Câu hỏi

1. Vì sao stress test cần kiểm tra cả throughput và ownership invariant?
2. Mailbox high-water mark cho biết điều gì về consumer latency?
3. Allocation failure bằng 0 có luôn chứng minh pool đủ lớn không?
4. Soak test dài hạn cần bổ sung counter và điều kiện PASS nào?
