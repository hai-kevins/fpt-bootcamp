# Lab 05 - Hierarchical State Machine

## Mục tiêu

- Mô tả quan hệ parent-child giữa các state.
- Bắt đầu dispatch từ leaf state.
- Bubble event lên parent khi leaf không xử lý.
- Thực hiện transition do parent state quyết định.
- Hiểu cách HSM giảm lặp behavior dùng chung.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/05-hierarchical-state-machine
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
state=1 bubbled=1
```

Leaf không xử lý signal 9; event được bubble lên parent và parent thực hiện transition thành công.

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đều đúng.

## Câu hỏi

1. Event bubbling giúp giảm lặp behavior như thế nào?
2. Parent chain phải thỏa invariant nào để tránh loop vô hạn?
3. Khi nào HSM phức tạp hơn lợi ích nó mang lại?
4. Entry/exit action nên được bổ sung vào engine theo thứ tự nào?
