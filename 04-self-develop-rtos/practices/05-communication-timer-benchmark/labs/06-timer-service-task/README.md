# Lab 06 - Timer Service Task

## Mục tiêu

- SysTick signal timer service; callback chạy trong task context.
- Quan sát output hoặc target trace thay vì chỉ đọc source.
- Tạo một lỗi có chủ đích và giải thích invariant bị phá.

## Build và chạy

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
