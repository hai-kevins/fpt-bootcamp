# Lab 08 - Queue Stress Test

## Mục tiêu

- Tạo producer nhanh hơn consumer.
- Đo total post, dispatch, drop và high-water mark.
- Xác nhận accounting invariant.
- Quan sát queue saturation có kiểm soát.

## Build và chạy

```bash
cd labs/08-queue-stress-test
make
make run
```

## Kết quả

Các invariant cần đúng:

```text
posted + dropped = attempts
dispatched = posted
high-water mark = capacity
```

Lab chủ động tạo drop; drop không phải lỗi của test mà là điều kiện cần đo.

## Câu hỏi

1. Tăng capacity có chữa được event leak không?
2. Producer rate và consumer service time quyết định queue depth thế nào?
3. Drop counter cần được báo ra production bằng cách nào?
4. Coalescing phù hợp với loại event nào?
