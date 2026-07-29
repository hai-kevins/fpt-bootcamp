# Labs — Independent Makefiles

Mỗi lab là một build unit độc lập.

- Không build lab từ Makefile root.
- Đi vào đúng thư mục lab rồi chạy Makefile của lab đó.
- Output nằm trong `build/` của chính lab.
- `make clean` chỉ xóa output của lab hiện tại.
- Host lab tạo executable Linux.
- Target lab tạo firmware ARM cho STM32F103.
- `make flash-stlink` phải chạy trong đúng target lab cần nạp.

## Host labs

| Lab | Lệnh chính | Nội dung |
|---:|---|---|
| 01 | `make run` | Blocking và non-blocking |
| 02 | `make test && make run` | Circular Event Queue |
| 03 | `make run` | Dispatcher |
| 05 | `make test && make run` | LED State Machine |
| 06 | `make test && make run` | UART command parser |
| 07 | `make run` | Event Trace |
| 08 | `make run` | Queue stress test |
| 09 | `make test && make run` | Product State Machine |
| 10 | `make run` | So sánh kiến trúc |

Ví dụ:

```bash
cd labs/05-led-state-machine
make test
make run
```

## Target labs

Lab 04 chạy trên STM32F103:

```bash
cd labs/04-button-debounce
make
make flash-stlink
```

Hoặc:

```bash
make TOOLCHAIN=clang
```

Kết nối button ngoài:

```text
PA0 ---- Button ---- GND
```

Không chạy `make flash-stlink` ở root khi mục tiêu là nạp Lab 04.
