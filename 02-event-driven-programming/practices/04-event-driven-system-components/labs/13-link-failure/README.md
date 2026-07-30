# Lab 13 - Link Failure, Timeout và Retry

## Mục tiêu

- Gửi frame yêu cầu ACK.
- Mô phỏng ACK không đến trước timeout.
- Retry đúng số lần giới hạn.
- Chuyển trạng thái link sang `ED_LINK_DOWN` khi hết retry.

## Build và test

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/13-link-failure
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

Output cho thấy tổng số lần send bằng 3: một lần gửi đầu và hai lần retry. Link kết thúc ở trạng thái DOWN và chương trình trả exit code `0`.

## Câu hỏi

1. Retry limit ngăn event storm như thế nào?
2. ACK cần đối chiếu sequence để tránh lỗi gì?
3. Timeout phải dùng monotonic time hay wall-clock time?
4. Sau khi link DOWN, điều kiện nào nên cho phép recovery?
