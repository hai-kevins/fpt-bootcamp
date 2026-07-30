# Lab 02 - Mailbox FIFO và Overflow

## Mục tiêu

- Khởi tạo mailbox có capacity bằng 2.
- Kiểm tra thứ tự FIFO khi post và get event.
- Kiểm tra policy drop-newest khi mailbox đầy.
- Quan sát high-water mark và overflow counter.
- Phân biệt capacity tạm thời với throughput dài hạn.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/02-mailbox
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
hwm=2 overflow=1
```

Hai event đầu được lấy đúng thứ tự; event thứ ba bị từ chối khi mailbox đầy.

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đều đúng.

## Câu hỏi

1. FIFO là invariant nào của mailbox?
2. Drop-newest và drop-oldest phù hợp với loại signal nào?
3. High-water mark giúp sizing mailbox như thế nào?
4. Nếu producer chạy nhanh hơn consumer lâu dài, tăng capacity có giải quyết tận gốc không?
