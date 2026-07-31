# Lab 16 - Soak Test

## Mục tiêu

- Mô phỏng chạy dài hạn bằng nhiều chu kỳ.
- Ghi statistics định kỳ.
- Phát hiện resource leak.
- Kiểm tra counter tăng ổn định.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/16-soak-test
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
hours=12 samples=12 leak=0 resets=0 PASS
```

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đúng.

## Câu hỏi

1. Soak test thật cần chạy bao lâu theo requirement?
2. Statistics nên sample theo chu kỳ nào?
3. Counter overflow sau nhiều ngày cần xử lý ra sao?
4. Power-cycle có nằm trong soak test hay test riêng?
