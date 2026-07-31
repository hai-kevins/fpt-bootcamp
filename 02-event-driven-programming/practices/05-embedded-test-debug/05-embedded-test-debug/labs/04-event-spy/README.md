# Lab 04 - Event Spy

## Mục tiêu

- Ghi lại signal được handler post.
- Kiểm tra số lần post.
- Kiểm tra signal và argument cuối.
- Dùng Spy thay cho Event Queue thật trong Unit Test.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/04-event-spy
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
calls=2 last_signal=20 last_argument=7 PASS
```

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đúng.

## Câu hỏi

1. Spy khác Mock ở mức kiểm tra expectation nào?
2. Event Spy nên lưu signal cuối hay toàn bộ sequence?
3. Khi sequence dài, capacity của Spy cần policy gì?
4. Spy có nên sở hữu payload động không?
