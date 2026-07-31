# Lab 18 - Hardware-in-the-Loop

## Mục tiêu

- Mô phỏng PC script reset board.
- Gửi shell command.
- Đọc response và so khớp expected result.
- Xuất report PASS/FAIL.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/18-hardware-in-the-loop
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
reset=PASS commands=4 passed=4 failed=0 HIL=PASS
```

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đúng.

## Câu hỏi

1. HIL runner cần timeout cho từng command như thế nào?
2. Reset board bằng ST-Link, relay hay reset controller có trade-off gì?
3. Serial transcript cần timestamp để debug ra sao?
4. HIL case phải cleanup DUT thế nào trước case tiếp theo?
