# Lab 01 - Active Object

## Mục tiêu

- Khởi tạo Active Object có ID, priority, mailbox, handler và context.
- Post một static event vào mailbox của Active Object.
- Dùng scheduler dispatch event theo run-to-completion.
- Xác nhận handler được gọi đúng một lần.
- Hiểu ranh giới ownership giữa Active Object, mailbox và scheduler.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/01-active-object
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
handled=1
```

Event được post thành công, scheduler dispatch đúng một lần và handler tăng bộ đếm lên 1.

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đều đúng.

## Câu hỏi

1. Active Object khác một hàm callback thông thường ở những thành phần sở hữu nào?
2. Invariant nào phải đúng giữa mailbox count và số event đã dispatch?
3. Điều gì xảy ra nếu handler block vô hạn?
4. Khi chuyển lên MCU, ISR nên post event hay gọi trực tiếp handler? Vì sao?
