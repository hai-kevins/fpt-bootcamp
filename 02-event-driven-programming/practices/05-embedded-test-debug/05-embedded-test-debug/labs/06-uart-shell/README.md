# Lab 06 - UART Shell

## Mục tiêu

- Cài command help, state, pool, mailbox, event và test.
- Tách command parser khỏi UART driver thật.
- Inject event qua shell.
- Trả lỗi cho unknown command.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/06-uart-shell
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
commands=6 known=6 unknown=1 PASS
```

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đúng.

## Câu hỏi

1. Shell nên post event hay điều khiển hardware trực tiếp?
2. Line buffer cần giới hạn và timeout như thế nào?
3. Diagnostic command production cần bảo vệ ra sao?
4. Command `test run all` nên trả report theo format nào?
