# Lab 04 - Delay and Delay-Until

## Mục tiêu

- So sánh relative delay và absolute delay-until.
- Đo drift do execution time.
- Phát hiện deadline miss.

## Build và chạy

Makefile chỉ quản lý lab này:

```bash
cd labs/04-delay-until
make test
make run
make clean
```

## Kết quả

Unit test chạy với ASan/UBSan. Demo được tạo tại `build/demo`. Hãy chủ động sửa một invariant, xác nhận test fail, rồi khôi phục implementation.

## Câu hỏi

1. Relative delay tạo drift như thế nào?
2. Delay-until cập nhật deadline từ now hay previous deadline?
3. Khi deadline miss, API nên làm gì?
4. Tick wrap ảnh hưởng periodic release ra sao?
