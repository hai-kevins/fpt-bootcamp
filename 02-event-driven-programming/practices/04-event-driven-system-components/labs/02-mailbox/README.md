# Lab 02 - Mailbox FIFO và Overflow

## Mục tiêu

- Khởi tạo mailbox capacity bằng 2.
- Kiểm tra thứ tự FIFO khi post/get event.
- Kiểm tra policy drop-newest khi mailbox đầy.
- Quan sát high-water mark và overflow counter.

## Build và test

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/02-mailbox
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

Output dự kiến: `hwm=2 overflow=1`. Hai event đầu được lấy theo đúng thứ tự signal 1 rồi signal 2; event thứ ba bị từ chối.

## Câu hỏi

1. FIFO là invariant nào của mailbox?
2. Drop-newest và drop-oldest phù hợp với loại signal nào?
3. High-water mark giúp sizing mailbox như thế nào?
4. Nếu producer chạy nhanh hơn consumer lâu dài, tăng capacity có giải quyết tận gốc không?
