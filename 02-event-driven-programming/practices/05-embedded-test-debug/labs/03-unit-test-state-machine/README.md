# Lab 03 - Unit Test State Machine

## Mục tiêu

- Kiểm thử các state IDLE, RUNNING, PAUSED và ERROR.
- Test mọi transition hợp lệ.
- Kiểm tra event không hợp lệ không làm đổi state.
- Đếm transition coverage.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/03-unit-test-state-machine
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
final=IDLE transitions=7 unhandled=1 PASS
```

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đúng.

## Câu hỏi

1. State-signal matrix hỗ trợ coverage như thế nào?
2. Unhandled event nên giữ state hay chuyển ERROR?
3. Entry/exit action cần được test riêng ra sao?
4. Làm sao kiểm tra output event của transition?
