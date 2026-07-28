# Lab 09 - Event and Message Latency

## Mục tiêu

- Đo ISR-to-task và queue send-to-receive latency.
- Quan sát output hoặc target trace thay vì chỉ đọc source.
- Tạo một lỗi có chủ đích và giải thích invariant bị phá.

## Build

```bash
make
make size
make disasm
make flash-stlink
```

## Kết quả

Artifact chính:

```text
build/firmware.elf
```

## Câu hỏi

1. Cơ chế của lab giải quyết vấn đề gì?
2. Ownership hoặc timing invariant nào phải luôn đúng?
3. Khi bỏ cơ chế này, lỗi nào xuất hiện?
4. Kết quả lab có đủ để chứng minh worst-case tuyệt đối không? Vì sao?
