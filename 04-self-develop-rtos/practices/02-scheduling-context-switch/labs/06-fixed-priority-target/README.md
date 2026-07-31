# Lab 06 - Fixed-Priority Scheduler on Target

## Mục tiêu

- Chạy fixed-priority scheduler với high, mid, low và idle task trên STM32.
- Quan sát task priority cao preempt task thấp khi hết thời gian delay.
- Đổi priority hoặc bỏ wake có chủ đích rồi phân tích thứ tự chạy sai.

## Build và chạy

Makefile của lab này chỉ build firmware của chính lab. Từ thư mục root:

```bash
cd labs/06-fixed-priority-target
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
build/lab06_fixed_priority.elf
```

Flash firmware, quan sát `high_count`, `mid_count`, `low_count`, `idle_count` bằng GDB và LED PC13 từ high task.

## Câu hỏi

1. Vì sao task priority thấp chỉ chạy khi task cao hơn không READY?
2. Delay làm thay đổi ready queue và task state như thế nào?
3. Idle task giải quyết trường hợp ready set rỗng ra sao?
4. Nếu quy ước priority bị đảo ngược nhưng selector không đổi thì lỗi gì xảy ra?
