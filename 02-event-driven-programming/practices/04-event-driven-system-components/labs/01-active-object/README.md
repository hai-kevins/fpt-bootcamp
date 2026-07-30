# Lab 01 - Active Object

## Mục tiêu

- Khởi tạo Active Object có ID, priority, mailbox, handler và context.
- Post một static event vào mailbox của Active Object.
- Dùng scheduler dispatch event theo run-to-completion.
- Xác nhận handler được gọi đúng một lần.

## Build và test

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/01-active-object
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

Output dự kiến chứa `handled=1`; chương trình trả exit code `0` khi event được post, dispatch và xử lý đúng một lần.

## Câu hỏi

1. Active Object khác một hàm callback thông thường ở những thành phần sở hữu nào?
2. Invariant nào phải đúng giữa mailbox count và số event đã dispatch?
3. Điều gì xảy ra nếu handler block vô hạn?
4. Khi chuyển lên MCU, ISR nên post event hay gọi trực tiếp handler? Vì sao?
