# Lab 03 - Run-to-Completion Scheduler

## Mục tiêu

- Đăng ký hai Active Object có priority khác nhau.
- Post event vào cả hai mailbox trước khi scheduler chạy.
- Xác nhận scheduler chọn priority cao trước.
- Quan sát nguyên tắc run-to-completion của từng handler.
- Liên hệ handler execution time với response latency.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/03-run-to-completion-scheduler
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
order=2,1
```

Active Object ID 2 có priority cao hơn nên được xử lý trước Active Object ID 1.

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đều đúng.

## Câu hỏi

1. Scheduler xác định Active Object ready bằng thông tin nào?
2. Run-to-completion khác preemption như thế nào?
3. Handler dài ảnh hưởng latency của Active Object khác ra sao?
4. Cần đo đại lượng nào để đặt deadline cho scheduler cooperative?
