# Lab 14 - Event Trace Ring Buffer

## Mục tiêu

- Ghi nhiều trace record hơn capacity của ring buffer.
- Giữ lại đúng các record mới nhất.
- Theo dõi overwritten count.
- Đọc record cũ nhất còn tồn tại sau wraparound.
- Liên hệ trace ring buffer với crash investigation.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/14-event-trace
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
count=128 overwritten=5 oldest=5
```

Sau khi ghi `capacity + 5` record, buffer vẫn giữ 128 record mới nhất và ghi đè đúng 5 record cũ.

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đều đúng.

## Câu hỏi

1. Vì sao trace ring buffer phù hợp với crash investigation?
2. Overwrite-oldest khác stop-when-full ở mục tiêu debug nào?
3. Binary trace tiết kiệm tài nguyên hơn text log thế nào?
4. Trace write từ ISR cần critical section hoặc atomic operation nào?
