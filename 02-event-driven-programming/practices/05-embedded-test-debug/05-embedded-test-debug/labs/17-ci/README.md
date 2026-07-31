# Lab 17 - Continuous Integration

## Mục tiêu

- Mô phỏng pipeline Build, Unit Test, Sanitizer, Firmware Build, Size Check và Artifact Report.
- Dừng pipeline khi stage fail.
- Đếm stage PASS.
- Trả exit code cho CI.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/17-ci
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
stages=6 passed=6 failed=0 PASS
```

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đúng.

## Câu hỏi

1. CI cần fail khi có warning không?
2. Memory budget nên kiểm tra bằng artifact nào?
3. Sanitizer chỉ chạy host có giới hạn gì?
4. Artifact khi fail nên gồm log, binary và report nào?
