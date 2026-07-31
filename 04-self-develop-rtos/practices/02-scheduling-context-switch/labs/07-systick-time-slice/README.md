# Lab 07 - SysTick Kernel Tick and Time Slice

## Mục tiêu

- Dùng SysTick 1 kHz để giảm time slice và yêu cầu PendSV.
- Cho hai worker cùng priority luân phiên mà không gọi `yield()`.
- Tắt time-slice rotation có chủ đích rồi quan sát một task bị giữ CPU.

## Build và chạy

Makefile của lab này chỉ build firmware của chính lab. Từ thư mục root:

```bash
cd labs/07-systick-time-slice
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
build/lab07_systick_time_slice.elf
```

Flash firmware và theo dõi `worker_a_count`, `worker_b_count`, `g_kernel_tick` và `g_context_switch_count` bằng GDB.

## Câu hỏi

1. Tại sao SysTick chỉ nên yêu cầu PendSV thay vì tự restore task khác?
2. Time slice được áp dụng cho task cùng priority như thế nào?
3. Nếu chỉ có một task tại priority hiện tại thì có cần rotation không?
4. Vì sao PendSV phải có priority thấp hơn SysTick?
