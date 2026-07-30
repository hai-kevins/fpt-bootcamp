# Lab 13 - Link Failure, Timeout và Retry

## Mục tiêu

- Gửi frame yêu cầu ACK.
- Mô phỏng ACK không đến trước timeout.
- Retry đúng số lần giới hạn.
- Chuyển trạng thái link sang `ED_LINK_DOWN` khi hết retry.
- Theo dõi send count, retry count và state cuối.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/13-link-failure
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
state=2 sends=3 retries=2
```

Có một lần gửi đầu và hai lần retry; sau đó link chuyển sang trạng thái DOWN.

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đều đúng.

## Câu hỏi

1. Retry limit ngăn event storm như thế nào?
2. ACK cần đối chiếu sequence để tránh lỗi gì?
3. Timeout phải dùng monotonic time hay wall-clock time?
4. Sau khi link DOWN, điều kiện nào nên cho phép recovery?
