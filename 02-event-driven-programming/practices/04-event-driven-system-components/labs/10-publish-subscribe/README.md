# Lab 10 - Publish-Subscribe

## Mục tiêu

- Đăng ký hai Active Object subscribe cùng một signal.
- Publish một static event mà publisher không biết subscriber cụ thể.
- Tạo bản sao dynamic riêng cho từng subscriber.
- Dispatch đủ event và release toàn bộ pool block.

## Build và test

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/10-publish-subscribe
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

Output dự kiến: `delivered=2 hits=2`. Hai subscriber đều nhận event và Event Pool kết thúc với `used_count == 0`.

## Câu hỏi

1. Publish-Subscribe giảm coupling giữa producer và consumer như thế nào?
2. Copy-per-subscriber có ưu điểm ownership gì?
3. Fan-out lớn ảnh hưởng Event Pool high-water mark ra sao?
4. Khi một subscriber mailbox đầy, publish nên trả kết quả thế nào?
