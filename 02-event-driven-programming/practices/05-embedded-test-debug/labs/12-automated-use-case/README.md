# Lab 12 - Automated Use Case

## Mục tiêu

- Mô phỏng Boot, Button press, Mode change, Timeout, Error và Recovery.
- Chạy use case không cần thao tác tay.
- Assert state sau từng step.
- Xuất PASS/FAIL.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/12-automated-use-case
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
steps=6 final=IDLE PASS
```

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đúng.

## Câu hỏi

1. Use-case test khác Unit Test ở phạm vi nào?
2. Mỗi step cần timeout và expected state ra sao?
3. Sequence có thể tái sử dụng cho HIL như thế nào?
4. Cleanup cần reset subsystem nào?
