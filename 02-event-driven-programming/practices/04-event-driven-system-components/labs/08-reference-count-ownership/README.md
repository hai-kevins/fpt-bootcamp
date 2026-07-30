# Lab 08 - Reference Count và Ownership

## Mục tiêu

- Allocate một Dynamic Event từ Event Pool.
- Retain event để tạo owner thứ hai.
- Release từng reference theo đúng thứ tự.
- Xác nhận block chỉ trở về pool khi reference count về 0.
- Phân biệt owner, borrower và transfer ownership.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/08-reference-count-ownership
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
refs=2->1 used=0
```

Reference count giảm đúng thứ tự; sau release cuối, block được trả về pool.

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đều đúng.

## Câu hỏi

1. Ai sở hữu event ngay sau allocate?
2. Double release có thể gây lỗi gì?
3. Reference count có giải quyết được ownership cycle không?
4. Khi fan-out lớn, copy event và retain cùng event khác nhau về RAM và complexity thế nào?
