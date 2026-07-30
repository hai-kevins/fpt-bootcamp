# Lab 06 - One-shot Timer

## Mục tiêu

- Debounce button 20 ms không dùng delay.
- Tách edge event khỏi kiểm tra trạng thái ổn định.
- Dùng phép so sánh deadline an toàn với wraparound.

## Build và test

Makefile của lab chỉ quản lý source và executable của chính lab.

```bash
cd labs/06-one-shot-timer
make test
make run
```

Có thể dùng:

```bash
make       # Build và chạy test/demo
make test  # Chạy kiểm tra
make run   # Chạy demo
make clean # Chỉ xóa build/ của lab hiện tại
```

## Kết quả

Trước 20 ms chưa phát button pressed; đúng deadline thì one-shot hết hạn một lần.

## Câu hỏi

1. Vì sao delay trong ISR là sai?
2. One-shot timer phải bị hủy khi nào?
3. Nếu có edge mới trước deadline thì xử lý ra sao?
4. So sánh deadline bằng `now >= deadline` có rủi ro wraparound gì?
