# Lab 03 - Dispatcher

## Mục tiêu

- Route event tới tối thiểu ba handler.
- Dùng destination ID thay cho lời gọi module cứng.
- Có invalid-event policy.
- In trace để thấy destination, signal và parameter.

## Build và chạy

```bash
cd labs/03-dispatcher
make
make run
```

## Kết quả

Ba event hợp lệ phải tới A, B và C. Event destination `99` phải tăng `invalid_count` mà không gọi nhầm handler.

```text
counts=1,1,1 invalid=1
```

## Câu hỏi

1. Routing table khác `switch` lớn ở đâu?
2. Dispatcher có nên chứa logic nghiệp vụ không?
3. Invalid destination nên drop, assert hay fatal trong những trường hợp nào?
4. Trace nên được ghi trước hay sau handler?
