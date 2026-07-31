# Lab 01 - Static Ring-Buffer Queue

## Mục tiêu

- Cài queue FIFO tĩnh, kiểm tra wrap, full/empty và item ordering.
- Quan sát output hoặc target trace thay vì chỉ đọc source.
- Tạo một lỗi có chủ đích và giải thích invariant bị phá.

## Build và chạy

```bash
make test
make run
```

## Kết quả

Artifact chính:

```text
build/01-static-ring-buffer-queue
```

## Câu hỏi

1. Cơ chế của lab giải quyết vấn đề gì?
2. Ownership hoặc timing invariant nào phải luôn đúng?
3. Khi bỏ cơ chế này, lỗi nào xuất hiện?
4. Kết quả lab có đủ để chứng minh worst-case tuyệt đối không? Vì sao?
