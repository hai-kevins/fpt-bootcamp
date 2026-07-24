# Labs — independent Makefiles

Mỗi thư mục lab là một build unit độc lập.

- Không build lab từ Makefile gốc.
- Không dùng rule tổng hợp như `make lab01`, `make all-labs` hoặc `make run-host-labs`.
- Đi vào đúng thư mục lab rồi chạy Makefile của lab đó.
- Output được tạo trong `build/` bên trong chính lab.
- `make clean` chỉ xóa output của lab hiện tại.

## Host labs

```bash
cd labs/01-singly-linked-list
make test
make run

cd ../02-intrusive-doubly-list
make test
make run

cd ../03-list-invariants
make test
make run

cd ../04-tcb-layout
make test
make run

cd ../05-static-task-creation
make test
make run

cd ../06-task-stack-initialization
make test
make run

cd ../07-all-task-registry
make test
make run

cd ../08-priority-ready-queues
make test
make run

cd ../09-tcb-multiple-lists
make test
make run
```

Các lab host dùng AddressSanitizer và UndefinedBehaviorSanitizer.

## Target lab

```bash
cd labs/10-target-task-inspector
make
make flash-stlink
```

Makefile của lab target tự chuyển sang Clang/LLD khi không tìm thấy GNU Arm Embedded Toolchain.
