# Lab 09 - Equal-Priority Round-Robin

## Mục tiêu

- Luân phiên ba worker cùng priority bằng FIFO ready queue và SysTick time slice.
- Quan sát tất cả counter đều tăng để chứng minh không task nào bị starvation.
- Bỏ queue rotation có chủ đích rồi quan sát một worker chiếm CPU.

## Build và chạy

Makefile của lab này chỉ build firmware của chính lab. Từ thư mục root:

```bash
cd labs/09-round-robin
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
build/lab09_round_robin.elf
```

Flash firmware và theo dõi `ca`, `cb`, `cc` cùng `g_context_switch_count` bằng GDB. Cả ba worker phải tiếp tục chạy.

## Câu hỏi

1. Round-robin khác fixed-priority selection ở điểm nào?
2. Vì sao chỉ xoay các task trong cùng một priority?
3. FIFO ready queue hỗ trợ fairness như thế nào?
4. Nếu time slice quá nhỏ hoặc quá lớn thì hệ thống bị ảnh hưởng ra sao?
