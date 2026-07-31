# Lab 10 - Crash Record

## Mục tiêu

- Lưu Fatal code, task, signal, state, uptime và reset reason.
- Thêm magic và checksum.
- Validate record sau reset.
- Phát hiện corruption.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/10-crash-record
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
valid=1 fatal=0xE001 task=3 signal=42 state=2 uptime=12345 reset=1
```

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đúng.

## Câu hỏi

1. Crash Record cần build ID để làm gì?
2. Checksum khác CRC ở mục tiêu nào?
3. Record nên được lưu ở retention RAM hay Flash khi nào?
4. Raw pointer có giá trị gì sau reset?
