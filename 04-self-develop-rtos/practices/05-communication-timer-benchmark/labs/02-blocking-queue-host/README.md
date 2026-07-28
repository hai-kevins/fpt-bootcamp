# Lab 02 - Blocking Queue on Host

## Mục tiêu

- Mô phỏng sender/receiver waiters, no-wait, finite timeout và wake ordering.
- Quan sát output hoặc target trace thay vì chỉ đọc source.
- Tạo một lỗi có chủ đích và giải thích invariant bị phá.

## Build và test

```bash
make test
make run
```

## Kết quả

Artifact chính:

```text
build/02-blocking-queue-host
```

## Câu hỏi

1. Cơ chế của lab giải quyết vấn đề gì?
2. Ownership hoặc timing invariant nào phải luôn đúng?
3. Khi bỏ cơ chế này, lỗi nào xuất hiện?
4. Kết quả lab có đủ để chứng minh worst-case tuyệt đối không? Vì sao?
