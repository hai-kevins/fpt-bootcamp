# Lab 04 - Fixed-Block Message Pool

## Mục tiêu

- Cài pool block cố định, exhaustion, reuse, invalid free và double free.
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
build/04-message-pool
```

## Câu hỏi

1. Cơ chế của lab giải quyết vấn đề gì?
2. Ownership hoặc timing invariant nào phải luôn đúng?
3. Khi bỏ cơ chế này, lỗi nào xuất hiện?
4. Kết quả lab có đủ để chứng minh worst-case tuyệt đối không? Vì sao?
