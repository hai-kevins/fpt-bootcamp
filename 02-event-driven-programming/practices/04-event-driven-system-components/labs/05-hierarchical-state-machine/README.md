# Lab 05 - Hierarchical State Machine

## Mục tiêu

- Mô tả quan hệ parent-child giữa các state.
- Bắt đầu dispatch từ leaf state.
- Bubble event lên parent khi leaf không xử lý.
- Thực hiện transition do parent state quyết định.

## Build và test

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/05-hierarchical-state-machine
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

Output chứa state cuối bằng `1` và bubbled count khác 0. Chương trình trả exit code `0` khi parent xử lý signal 9 và chuyển state thành công.

## Câu hỏi

1. Event bubbling giúp giảm lặp behavior như thế nào?
2. Parent chain phải thỏa invariant nào để tránh loop vô hạn?
3. Khi nào HSM phức tạp hơn lợi ích nó mang lại?
4. Entry/exit action nên được bổ sung vào engine theo thứ tự nào?
