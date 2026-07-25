# Labs — Independent Makefiles

Mỗi thư mục lab là một build unit độc lập.

- Không build lab từ Makefile gốc.
- Không dùng rule tổng hợp như `make lab01`, `make all-labs`, `make run-labs` hoặc `make run-host-labs`.
- Đi vào đúng thư mục lab rồi chạy Makefile của lab đó.
- Output được tạo trong `build/` bên trong chính lab.
- `make clean` chỉ xóa output của lab hiện tại.

## Host labs

```bash
cd labs/03-stack-high-water
make test
make run

cd ../04-static-memory-budget
make test
make run

cd ../05-first-fit-basic
make test
make run

cd ../06-block-splitting
make test
make run

cd ../07-block-coalescing
make test
make run

cd ../08-invalid-free
make test
make run

cd ../09-fragmentation
make test
make run
```

Các lab host dùng AddressSanitizer và UndefinedBehaviorSanitizer.

## Target labs

```bash
cd labs/01-memory-layout
make

cd ../02-startup-sections
make

cd ../10-target-demo
make
```

Makefile mặc định dùng GNU Arm Embedded Toolchain. Khi muốn dùng Clang/LLD, chạy `make TOOLCHAIN=clang`.
