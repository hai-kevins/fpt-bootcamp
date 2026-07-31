# Lab 07 - Binary Event Trace

## Mục tiêu

- Tạo ring buffer 128 record.
- Cài trace dump, clear và stats ở mức logic.
- Theo dõi overwritten count.
- Đọc record theo thứ tự thời gian.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/07-binary-event-trace
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
count=128 overwritten=5 oldest=5 clear=PASS
```

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đúng.

## Câu hỏi

1. Binary trace tiết kiệm tài nguyên hơn text log thế nào?
2. Record nên lưu ID hay pointer?
3. Trace clear nên được phép trong production không?
4. Dump trace không blocking nên thực hiện bằng cách nào?
