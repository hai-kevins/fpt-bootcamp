# Labs — independent Makefiles

Mỗi thư mục lab là một build unit độc lập.

- Không build lab từ Makefile gốc.
- Không dùng rule tổng hợp như `make lab01` hoặc `make all-labs`.
- Đi vào đúng thư mục lab rồi chạy Makefile của lab đó.
- Output được tạo trong `build/` của chính lab.
- `make clean` chỉ xóa output của lab hiện tại.

## Host labs

```bash
cd labs/01-task-state-machine && make test && make run
cd ../02-efficient-blocking && make test && make run
cd ../03-delayed-list-tick-wrap && make test && make run
cd ../04-delay-until && make test && make run
cd ../05-semaphore-host && make test && make run
cd ../07-mutex-ownership && make test && make run
cd ../08-priority-inheritance && make test && make run
cd ../09-suspend-resume && make test && make run
```

## Target labs

```bash
cd labs/06-isr-semaphore-wake
make
make flash-stlink

cd ../10-target-synchronization-playground
make
make flash-stlink
```

Host labs dùng AddressSanitizer và UndefinedBehaviorSanitizer. Target labs tự chuyển sang Clang/LLD khi không tìm thấy GNU Arm Embedded Toolchain.
