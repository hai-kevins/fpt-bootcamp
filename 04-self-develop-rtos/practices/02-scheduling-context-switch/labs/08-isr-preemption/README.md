# Lab 08 - Preemption from EXTI0 Interrupt

## Mục tiêu

- Wake high-priority task từ EXTI0 ISR trên chân PA0.
- Chứng minh ISR chỉ pend PendSV và task high chạy sau khi ISR return.
- Context-switch trực tiếp trong ISR có chủ đích trên bản thử nghiệm rồi phân tích rủi ro.

## Build và chạy

Makefile của lab này chỉ build firmware của chính lab. Từ thư mục root:

```bash
cd labs/08-isr-preemption
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
build/lab08_isr_preemption.elf
```

Flash firmware, tạo rising edge sạch trên PA0 và quan sát `irq_count`, `high_runs`, `low_runs` cùng LED PC13.

## Câu hỏi

1. Vì sao high task phải block trước khi chờ interrupt?
2. ISR-safe wake khác task-context wake ở điểm nào?
3. Tại sao context switch được defer tới PendSV?
4. Button bounce có thể làm kết quả `irq_count` thay đổi như thế nào?
