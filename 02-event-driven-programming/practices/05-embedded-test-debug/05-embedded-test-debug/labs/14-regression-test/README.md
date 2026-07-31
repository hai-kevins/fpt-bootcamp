# Lab 14 - Regression Test

## Mục tiêu

- Mô phỏng một bug cũ về stale timeout.
- Tạo test tái hiện bug.
- Xác nhận fix loại bỏ timeout cũ.
- Giữ test trong regression suite.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/14-regression-test
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
bug_reproduced=1 fixed=1 stale_timeout=0 PASS
```

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đúng.

## Câu hỏi

1. Vì sao nên viết failing test trước khi fix?
2. Regression test cần tên gắn với behavior hay bug ID?
3. Test quá phụ thuộc implementation gây brittle ra sao?
4. Bug timing cần được tái hiện deterministic thế nào?
