# Lab 08 - Reference Count và Ownership

## Mục tiêu

- Allocate một dynamic event từ Event Pool.
- Retain event để tạo owner thứ hai.
- Release từng reference theo đúng thứ tự.
- Xác nhận block chỉ trở về pool khi ref-count về 0.

## Build và test

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/08-reference-count-ownership
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

Output dự kiến: `refs=2->1 used=0`. Sau release cuối cùng, event được trả về pool và không còn block đang sử dụng.

## Câu hỏi

1. Ai sở hữu event ngay sau allocate?
2. Double release có thể gây lỗi gì?
3. Reference count có giải quyết được ownership cycle không?
4. Khi fan-out lớn, copy event và retain cùng event khác nhau về RAM và complexity thế nào?
