# 01 - Embedded Foundation

Repository thực hành cho **Chủ đề 1: Kiến thức nền tảng trong Embedded System Programming**:

- Cấu trúc máy tính, địa chỉ và dữ liệu trong bộ nhớ.
- Con trỏ, endianness và Memory-Mapped I/O.
- Các section `.isr_vector`, `.text`, `.rodata`, `.data` và `.bss`.
- Bare-metal ARM với GNU Arm Embedded Toolchain.
- Makefile, Startup Code và Linker Script.
- GPIO register-level, UART polling và phân tích map file.

Project được tổ chức theo cùng một bố cục:

- Phần root là firmware tổng kết cuối chủ đề.
- `labs/` chứa từng bài thực hành độc lập.
- `docs/` chứa phần giải thích sâu hơn.
- `build/` chỉ chứa artifact sinh tự động.
- Makefile gốc chỉ quản lý firmware tổng kết.
- Mỗi lab có Makefile riêng và không được build từ root.

Cấu hình mặc định:

- Board: STM32F103C8T6 Blue Pill.
- CPU: ARM Cortex-M3.
- Flash: 64 KiB.
- SRAM: 20 KiB.
- Clock: HSI 8 MHz sau reset.
- LED: PC13, active-low.
- UART: USART1, PA9/PA10, 9600 baud, 8-N-1.
- Code style: bare-metal, register-level, không HAL/SPL.

Khi chuyển sang board hoặc MCU khác, cần kiểm tra lại Linker Script, memory map, Vector Table, địa chỉ thanh ghi, chân GPIO, chân UART và tần số clock.

---

## 1. Mục tiêu

Sau khi hoàn thành repository này, người học có thể:

- Giải thích quan hệ giữa dữ liệu, địa chỉ và con trỏ.
- Phân biệt little-endian và big-endian.
- Truy cập dữ liệu thông qua địa chỉ và hiểu vai trò của `volatile`.
- Phân biệt Flash, SRAM, peripheral region và Cortex-M system region.
- Giải thích vai trò của `.isr_vector`, `.text`, `.rodata`, `.data` và `.bss`.
- Hiểu luồng reset từ Vector Table tới `main()`.
- Tự viết Startup Code tối thiểu cho Cortex-M3.
- Tự viết Linker Script cho STM32F103C8T6.
- Điều khiển LED PC13 bằng thanh ghi GPIO.
- Gửi, nhận và echo dữ liệu bằng USART1 polling.
- Tạo các artifact `.elf`, `.bin`, `.hex`, `.map` và `.lst`.
- Phân tích firmware bằng `size`, `nm`, `objdump`, map file và listing file.
- Phân biệt lab chạy trên host với firmware chạy trên target MCU.

---

## 2. Cấu trúc repository

```text
01-embedded-foundation/
├── README.md
├── Makefile
├── .gitignore
├── VALIDATION.md
├── linker/
│   └── memory.ld
├── startup/
│   └── startup.c
├── include/
│   ├── gpio.h
│   └── uart.h
├── src/
│   ├── gpio.c
│   ├── main.c
│   └── uart.c
├── labs/
│   ├── README.md
│   ├── 01-endianness/
│   ├── 02-memory-access/
│   ├── 03-memory-sections/
│   ├── 04-startup-code/
│   ├── 05-linker-script/
│   ├── 06-gpio-register/
│   ├── 07-uart-polling/
│   └── 08-map-analysis/
├── docs/
│   ├── map-analysis.md
│   ├── memory-map.md
│   ├── sections-analysis.md
│   └── startup-flow.md
└── build/
```

---

## 3. Project root làm gì?

Firmware root là chương trình bare-metal tổng kết chạy trên STM32F103C8T6:

```text
Reset
  |
  v
Vector Table
  |
  v
Reset_Handler
  |-- Copy .data từ Flash sang SRAM
  |-- Clear .bss
  v
main()
  |-- Khởi tạo LED PC13
  |-- Khởi tạo USART1
  |-- Gửi banner qua UART
  v
Super-loop
  |-- Toggle LED
  |-- Gửi "tick"
  |-- Poll RXNE
  |-- Echo byte nhận được
```

Firmware root kết hợp kết quả của các lab Startup Code, Linker Script, GPIO register-level và UART polling thành một project hoàn chỉnh.

---

## 4. Cài toolchain

Ubuntu/Debian:

```bash
sudo apt update
sudo apt install \
    gcc-arm-none-eabi \
    binutils-arm-none-eabi \
    make \
    gcc
```

Công cụ flash và debug tùy chọn:

```bash
sudo apt install \
    openocd \
    stlink-tools \
    gdb-multiarch \
    picocom
```

Kiểm tra:

```bash
arm-none-eabi-gcc --version
arm-none-eabi-objcopy --version
arm-none-eabi-size --version
gcc --version
make --version
```

Firmware root và các target lab dùng GNU Arm Embedded Toolchain. Ba host lab đầu dùng compiler native `gcc`.

---

## 5. Build project tổng kết

Từ thư mục root:

```bash
make
```

Artifact:

```text
build/embedded_foundation.elf
build/embedded_foundation.bin
build/embedded_foundation.hex
build/embedded_foundation.map
build/embedded_foundation.lst
```

Các lệnh phân tích:

```bash
make size
make sections
make symbols
make symbols-size
make disasm
make clean
make rebuild
```

`make clean` tại root chỉ xóa artifact của firmware tổng kết, không xóa `build/` nằm trong từng lab.

---

## 6. Chạy và kiểm thử

### Flash firmware

ST-Link tools:

```bash
make flash-stlink
```

OpenOCD:

```bash
make flash-openocd
```

Mass erase:

```bash
make erase
```

Debug bằng OpenOCD:

```bash
openocd \
    -f interface/stlink.cfg \
    -f target/stm32f1x.cfg
```

Ở terminal khác:

```bash
gdb-multiarch build/embedded_foundation.elf
```

Trong GDB:

```gdb
target extended-remote :3333
monitor reset halt
load
monitor reset run
```

---

### UART

Kết nối:

```text
STM32F103       USB-UART
PA9  TX   --->  RX
PA10 RX   <---  TX
GND        ---  GND
```

Mở terminal:

```bash
picocom -b 9600 /dev/ttyUSB0
```

Banner dự kiến:

```text
STM32F103 bare-metal foundation
Clock: HSI 8 MHz
USART1: 9600 8-N-1
Type characters to test echo.
```

Firmware tiếp tục gửi:

```text
tick
```

Mỗi byte nhận được trên PA10 sẽ được gửi lại qua PA9.

---

## 7. Build các lab

Makefile ở thư mục gốc **chỉ quản lý firmware tổng kết**. Nó không gọi hoặc điều khiển Makefile của các lab.

Mỗi lab được build ngay trong thư mục của chính lab đó.

Ví dụ với lab chạy trên host:

```bash
cd labs/01-endianness
make
make run
```

Ví dụ với lab chạy trên STM32:

```bash
cd labs/06-gpio-register
make
make flash-stlink
```

Để quay lại thư mục root:

```bash
cd ../..
```

Quy ước:

- Host labs: `01-endianness`, `02-memory-access`, `03-memory-sections`.
- Target labs: `04-startup-code`, `05-linker-script`, `06-gpio-register`, `07-uart-polling`, `08-map-analysis`.
- Mỗi lab tạo output trong `build/` của chính lab đó.
- `make clean` trong một lab chỉ xóa output của lab hiện tại.
- `make flash-stlink` trong target lab chỉ nạp firmware của chính target lab.
- Không dùng `make lab01` hoặc `make lab06` tại root vì Root Makefile không có các rule này.
- Không chạy `make flash-stlink` tại root khi mục tiêu là nạp một lab cụ thể.

---

## 8. Danh sách bài thực hành

| Bài | Chủ đề | Môi trường | Kết quả chính |
|---:|---|---|---|
| 1 | Endianness | Host | Quan sát thứ tự byte của `0x12345678` |
| 2 | Memory access | Host | Đọc và ghi dữ liệu thông qua địa chỉ |
| 3 | Memory sections | Host | Khảo sát `.rodata`, `.data`, `.bss` và symbol |
| 4 | Startup Code | STM32F103 | Kiểm tra copy `.data` và clear `.bss` |
| 5 | Linker Script | STM32F103 | Bố trí Flash, SRAM và section `.noinit` |
| 6 | GPIO register-level | STM32F103 | Điều khiển LED PC13 bằng thanh ghi |
| 7 | UART polling | STM32F103 | TX, RX và echo USART1 |
| 8 | Map analysis | STM32F103 | Phân tích Flash, SRAM, section và symbol |

---

## 9. Quy trình học đề xuất

```text
Đọc README của lab
      |
      v
Đọc source code và xác định mục tiêu
      |
      v
Build trong đúng thư mục lab
      |
      v
Chạy executable host hoặc flash firmware target
      |
      v
Quan sát output, LED, UART, GDB hoặc map file
      |
      v
Trả lời câu hỏi cuối lab
      |
      v
Cố ý thay đổi một giả thiết hoặc tạo một lỗi
      |
      v
Giải thích nguyên nhân rồi khôi phục code
      |
      v
Chuyển sang lab tiếp theo
```

Không nên chỉ chạy code có sẵn. Cần tự thay đổi dữ liệu, địa chỉ, section, kích thước buffer, cấu hình GPIO hoặc baud rate và quan sát tác động.

---

## 10. Kiến trúc và nguyên tắc quan trọng

### Startup contract

Trước khi gọi `main()`, firmware phải bảo đảm:

```text
MSP hợp lệ
.data đã được copy từ Flash sang SRAM
.bss đã được clear về 0
Vector Table nằm đúng địa chỉ
```

### Memory-Mapped I/O

GPIO và UART được truy cập qua địa chỉ thanh ghi `volatile`. Việc thay đổi MCU yêu cầu kiểm tra lại memory map, clock gate, pin configuration và peripheral register.

### Host và target

Các lab kiến thức thuần chạy trên host để quan sát nhanh. Các lab Startup Code, Linker Script, GPIO, UART và map analysis tạo firmware ARM riêng cho STM32F103.

## 11. Ghi chú kỹ thuật

### LED PC13 active-low

```text
PC13 = 0 -> LED sáng
PC13 = 1 -> LED tắt
```

### Clock

Startup Code không bật HSE hoặc PLL. MCU tiếp tục chạy bằng HSI 8 MHz sau reset. USART1 được tính với PCLK2 bằng 8 MHz.

### Runtime

Firmware root và target labs link bằng `-nostdlib` và `-nostartfiles`. Project không dùng startup mặc định, `printf()` hoặc C library heap trên MCU.

### Host labs

Ba lab đầu là executable Linux và có thể dùng `printf()`. Chúng không tạo firmware ARM và không thể được nạp vào STM32.

### Memory-Mapped I/O

Các driver GPIO và UART truy cập thanh ghi bằng con trỏ `volatile`. Địa chỉ phải được đối chiếu với Reference Manual của STM32F1.

### Map file

Kết quả từ `arm-none-eabi-size` chưa bao gồm đầy đủ stack usage tại runtime. Khi đánh giá RAM, cần xét thêm stack, buffer runtime và khoảng dự phòng.
