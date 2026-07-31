# Lab 10 - Context-Switch Stress and Diagnostics

## Mục tiêu

- Chạy nhiều task và context switch trong thời gian dài trên STM32.
- Kiểm tra local counter, stack guard và scheduler invariants mà không in mỗi switch.
- Phá guard word có chủ đích rồi xác nhận panic path phát hiện lỗi.

## Build và chạy

Makefile của lab này chỉ build firmware của chính lab. Từ thư mục root:

```bash
cd labs/10-context-switch-stress
make
```

Các lệnh thường dùng:

```bash
make size
make disasm
make flash-stlink
make clean
```

Nếu không có GNU Arm Embedded Toolchain, Makefile tự chuyển sang Clang/LLD.

## Kết quả

Artifact chính nằm cục bộ trong lab:

```text
build/lab10_context_switch_stress.elf
```

Flash firmware, theo dõi `vh`, `va`, `vb`, `vc`, `guard_failures` và switch count. Ghi kết quả vào `report/target-validation.md`.

## Câu hỏi

1. Vì sao không nên in UART trong mỗi lần context switch?
2. Stack guard phát hiện loại lỗi nào và không phát hiện loại lỗi nào?
3. Local counter riêng giúp chứng minh context preservation ra sao?
4. Stress test pass có đủ để chứng minh kernel đúng hoàn toàn không? Vì sao?
