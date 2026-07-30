# Lab 10 - Publish-Subscribe

## Mục tiêu

- Đăng ký hai Active Object subscribe cùng một signal.
- Publish một static event mà publisher không biết subscriber cụ thể.
- Tạo bản sao Dynamic Event riêng cho từng subscriber.
- Dispatch đủ event và release toàn bộ pool block.
- Quan sát tác động của fan-out tới Event Pool high-water mark.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/10-publish-subscribe
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
delivered=2 hits=2
```

Hai subscriber đều nhận event và Event Pool kết thúc với `used_count == 0`.

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đều đúng.

## Câu hỏi

1. Publish-Subscribe giảm coupling giữa producer và consumer như thế nào?
2. Copy-per-subscriber có ưu điểm ownership gì?
3. Fan-out lớn ảnh hưởng Event Pool high-water mark ra sao?
4. Khi một subscriber mailbox đầy, publish nên trả kết quả thế nào?
