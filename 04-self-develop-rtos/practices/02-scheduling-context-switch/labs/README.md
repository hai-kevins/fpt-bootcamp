# Labs — Independent Makefiles

Mỗi thư mục lab là một build unit độc lập.

- Không build lab từ Makefile gốc.
- Không dùng rule tổng hợp như `make lab01`, `make all-labs`, `make run-labs` hoặc `make run-host-labs`.
- Đi vào đúng thư mục lab rồi chạy Makefile của lab đó.
- Output được tạo trong `build/` bên trong chính lab.
- `make clean` chỉ xóa output của lab hiện tại.

## Host labs

```bash
cd labs/02-initial-task-stack
make test
make run

cd ../05-priority-scheduler-host
make test
make run
```

Các lab host dùng AddressSanitizer và UndefinedBehaviorSanitizer.

## Target labs

```bash
cd labs/01-exception-stack-inspection
make

cd ../03-svc-first-task
make

cd ../04-cooperative-pendsv
make

cd ../06-fixed-priority-target
make

cd ../07-systick-time-slice
make

cd ../08-isr-preemption
make

cd ../09-round-robin
make

cd ../10-context-switch-stress
make
```

Makefile ưu tiên GNU Arm Embedded Toolchain; nếu không tìm thấy `arm-none-eabi-gcc`, nó tự chuyển sang Clang/LLD.
