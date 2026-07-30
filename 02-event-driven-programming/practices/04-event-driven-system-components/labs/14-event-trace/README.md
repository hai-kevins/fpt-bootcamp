# Lab 14 - Event Trace Ring Buffer

## Mục tiêu

- Ghi nhiều trace record hơn capacity của ring buffer.
- Giữ lại đúng các record mới nhất.
- Theo dõi overwritten count.
- Đọc record cũ nhất còn tồn tại sau wraparound.

## Build và test

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/14-event-trace
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

Sau khi ghi `capacity + 5` record, count vẫn bằng capacity, overwritten count bằng 5 và timestamp cũ nhất còn lại bằng 5.

## Câu hỏi

1. Vì sao trace ring buffer phù hợp với crash investigation?
2. Overwrite-oldest khác stop-when-full ở mục tiêu debug nào?
3. Binary trace tiết kiệm tài nguyên hơn text log thế nào?
4. Trace write từ ISR cần critical section hoặc atomic operation nào?
