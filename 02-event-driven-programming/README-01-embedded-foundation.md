# Chủ đề 1 - Kiến thức nền tảng trong Embedded System Programming
## Computer Architecture, Memory và Bare-Metal Project

> Mục tiêu của chủ đề này là xây dựng nền tảng vững chắc về kiến trúc máy tính, địa chỉ, bộ nhớ và quy trình tạo một dự án bare-metal cho vi điều khiển ARM Cortex-M.
>
> Nội dung trọng tâm gồm CPU và bus, không gian địa chỉ, Flash/SRAM, con trỏ, Memory-Mapped I/O, startup code, linker script, Makefile, quy trình build và kiểm thử firmware tối thiểu.

---

## Mục lục

1. [Mục tiêu học tập](#1-mục-tiêu-học-tập)
2. [Kiến thức cần chuẩn bị](#2-kiến-thức-cần-chuẩn-bị)
3. [Cấu trúc tổng quát của một hệ thống nhúng](#3-cấu-trúc-tổng-quát-của-một-hệ-thống-nhúng)
4. [CPU và kiến trúc ARM Cortex-M](#4-cpu-và-kiến-trúc-arm-cortex-m)
5. [Địa chỉ và không gian địa chỉ](#5-địa-chỉ-và-không-gian-địa-chỉ)
6. [Bộ nhớ trong hệ thống nhúng](#6-bộ-nhớ-trong-hệ-thống-nhúng)
7. [Con trỏ và thao tác bộ nhớ trong C](#7-con-trỏ-và-thao-tác-bộ-nhớ-trong-c)
8. [Thanh ghi ngoại vi và Memory-Mapped I/O](#8-thanh-ghi-ngoại-vi-và-memory-mapped-io)
9. [Quy trình khởi động của vi điều khiển](#9-quy-trình-khởi-động-của-vi-điều-khiển)
10. [Các thành phần của dự án bare-metal](#10-các-thành-phần-của-dự-án-bare-metal)
11. [Ví dụ tối thiểu](#11-ví-dụ-tối-thiểu)
12. [Quy trình build và phân tích firmware](#12-quy-trình-build-và-phân-tích-firmware)
13. [Quy trình phát triển dự án bare-metal](#13-quy-trình-phát-triển-dự-án-bare-metal)
14. [Best Practices](#14-best-practices)
15. [Anti-pattern và lỗi thường gặp](#15-anti-pattern-và-lỗi-thường-gặp)
16. [Kiểm thử](#16-kiểm-thử)
17. [Bài thực hành](#17-bài-thực-hành)
18. [Tiêu chí hoàn thành](#18-tiêu-chí-hoàn-thành)
19. [Cấu trúc repository đề xuất](#19-cấu-trúc-repository-đề-xuất)
20. [Tài liệu tham khảo](#20-tài-liệu-tham-khảo)
21. [Tổng kết](#21-tổng-kết)

---

# 1. Mục tiêu học tập

Sau khi hoàn thành chủ đề này, người học cần có khả năng:

- Giải thích được CPU, thanh ghi, bus, Flash, SRAM và ngoại vi.
- Hiểu sự khác nhau giữa địa chỉ, dữ liệu và giá trị được lưu tại địa chỉ.
- Hiểu cách chương trình C được bố trí trong Flash và RAM.
- Sử dụng con trỏ để đọc và ghi dữ liệu tại một địa chỉ xác định.
- Hiểu cơ chế Memory-Mapped I/O.
- Giải thích được quá trình MCU khởi động từ Reset Handler đến `main()`.
- Tạo được một dự án bare-metal bằng:
  - ARM GCC
  - Makefile
  - Linker Script
  - Startup Code
- Build được các file:
  - `.elf`
  - `.bin`
  - `.hex`
  - `.map`
- Đọc được thông tin kích thước chương trình bằng `arm-none-eabi-size`.
- Thực hiện được một ví dụ GPIO hoặc UART ở mức thanh ghi.

---

# 2. Kiến thức cần chuẩn bị

Trước khi bắt đầu, người học nên nắm:

- C cơ bản:
  - Biến
  - Hàm
  - Mảng
  - Cấu trúc
  - Con trỏ
  - Toán tử bit
- Hệ nhị phân và hệ hexadecimal.
- Các phép toán:
  - AND: `&`
  - OR: `|`
  - XOR: `^`
  - NOT: `~`
  - Dịch trái: `<<`
  - Dịch phải: `>>`
- Cách sử dụng terminal Linux cơ bản.
- Cách sử dụng Git ở mức cơ bản.

Ví dụ:

```c
uint32_t value = 0x00000001U;

value |=  (1U << 5);   // Set bit 5
value &= ~(1U << 5);   // Clear bit 5
value ^=  (1U << 5);   // Toggle bit 5
```

---

# 3. Cấu trúc tổng quát của một hệ thống nhúng

Một vi điều khiển thường gồm các thành phần chính:

```text
+------------------------------------------------------+
|                    Microcontroller                   |
|                                                      |
|  +----------+       +----------+       +----------+  |
|  |   CPU    |<----->|   Bus    |<----->|  Memory  |  |
|  | Cortex-M |       | AHB/APB  |       | Flash/RAM|  |
|  +----------+       +----------+       +----------+  |
|       |                   |                   |      |
|       |                   +-------------------+      |
|       |                                              |
|       +-------------> Peripherals                    |
|                       GPIO, UART, SPI, I2C, Timer    |
+------------------------------------------------------+
```

## CPU

CPU thực hiện các công việc:

1. Đọc lệnh từ bộ nhớ.
2. Giải mã lệnh.
3. Thực thi lệnh.
4. Cập nhật thanh ghi.
5. Đọc hoặc ghi dữ liệu.
6. Xử lý ngắt.

Chu trình này thường được mô tả bằng ba bước:

```text
Fetch -> Decode -> Execute
```

## Bus

Bus là đường truyền kết nối CPU, bộ nhớ và ngoại vi.

Các nhóm bus thường gặp trên Cortex-M:

- AHB: tốc độ cao.
- APB: kết nối ngoại vi.
- System bus: truy cập SRAM và ngoại vi.
- I-Code bus: lấy lệnh từ Flash.
- D-Code bus: truy cập dữ liệu trong vùng code.

## Bộ nhớ

Hai vùng quan trọng nhất:

- Flash:
  - Lưu chương trình.
  - Không mất dữ liệu khi mất điện.
- SRAM:
  - Lưu biến trong lúc chạy.
  - Mất dữ liệu khi mất điện.

## Ngoại vi

Ví dụ:

- GPIO
- UART
- SPI
- I2C
- ADC
- Timer
- CAN
- USB

CPU điều khiển ngoại vi bằng cách đọc và ghi vào các thanh ghi ngoại vi.

---

# 4. CPU và kiến trúc ARM Cortex-M

ARM Cortex-M là dòng CPU được thiết kế cho vi điều khiển.

Một số thanh ghi quan trọng:

| Thanh ghi | Chức năng |
|---|---|
| `R0-R12` | Thanh ghi đa dụng |
| `SP` | Stack Pointer |
| `LR` | Link Register |
| `PC` | Program Counter |
| `xPSR` | Thanh ghi trạng thái |

## Program Counter

`PC` chứa địa chỉ của lệnh đang hoặc sắp được thực thi.

```text
PC -> địa chỉ lệnh trong Flash
```

## Stack Pointer

`SP` trỏ tới vùng stack trong RAM.

Stack thường được dùng cho:

- Biến cục bộ.
- Địa chỉ quay về của hàm.
- Lưu thanh ghi khi gọi hàm.
- Lưu context khi xảy ra ngắt.

## Link Register

`LR` thường chứa địa chỉ quay về sau khi hàm kết thúc.

Ví dụ:

```c
void foo(void)
{
    // Khi gọi foo(), địa chỉ quay về được lưu trong LR.
}
```

---

# 5. Địa chỉ và không gian địa chỉ

## Địa chỉ là gì?

Địa chỉ là một số dùng để xác định vị trí trong không gian bộ nhớ.

Ví dụ:

```text
Địa chỉ: 0x20000000
Giá trị: 0x12345678
```

Có thể hình dung:

```text
Address       Value
0x20000000 -> 0x78
0x20000001 -> 0x56
0x20000002 -> 0x34
0x20000003 -> 0x12
```

Trong hệ little-endian, byte thấp được lưu tại địa chỉ thấp.

## Không gian địa chỉ

Cortex-M 32-bit thường có không gian địa chỉ lý thuyết:

```text
0x00000000 -> 0xFFFFFFFF
```

Không phải toàn bộ vùng này đều được triển khai vật lý.

Một bản đồ bộ nhớ điển hình:

```text
0x00000000  +-----------------------------+
            | Code / Flash                |
0x1FFFFFFF  +-----------------------------+

0x20000000  +-----------------------------+
            | SRAM                        |
0x3FFFFFFF  +-----------------------------+

0x40000000  +-----------------------------+
            | Peripheral                  |
0x5FFFFFFF  +-----------------------------+

0xE0000000  +-----------------------------+
            | Cortex-M System Control     |
0xFFFFFFFF  +-----------------------------+
```

Địa chỉ thực tế của từng vùng phụ thuộc vào MCU.

---

# 6. Bộ nhớ trong hệ thống nhúng

Một chương trình C thường được chia thành các section.

## `.text`

Chứa mã máy của chương trình.

Ví dụ:

```c
int add(int a, int b)
{
    return a + b;
}
```

Hàm `add()` thường được đặt trong `.text`.

## `.rodata`

Chứa dữ liệu chỉ đọc.

```c
static const char message[] = "Hello";
```

## `.data`

Chứa biến toàn cục hoặc biến `static` đã được khởi tạo với giá trị khác 0.

```c
uint32_t system_counter = 10U;
```

Giá trị ban đầu được lưu trong Flash, sau đó được sao chép sang RAM khi khởi động.

## `.bss`

Chứa biến toàn cục hoặc biến `static` chưa khởi tạo hoặc khởi tạo bằng 0.

```c
uint32_t error_counter;
static uint8_t rx_buffer[128];
```

Khi khởi động, vùng `.bss` phải được gán bằng 0.

## Heap

Heap thường được sử dụng bởi:

```c
malloc();
calloc();
realloc();
free();
```

Trong firmware nhỏ, việc sử dụng cấp phát động cần được cân nhắc vì có thể gây:

- Fragmentation.
- Khó xác định thời gian thực thi.
- Khó dự đoán mức sử dụng RAM.
- Lỗi thiếu bộ nhớ tại runtime.

## Stack

Stack được sử dụng cho:

- Biến cục bộ.
- Tham số hàm.
- Địa chỉ quay về.
- Context của interrupt.

Ví dụ:

```c
void process(void)
{
    uint8_t local_buffer[64];
}
```

`local_buffer` thường nằm trên stack.

## Sơ đồ bố trí Flash và RAM

```text
FLASH
+--------------------------+
| Vector Table             |
+--------------------------+
| .text                    |
+--------------------------+
| .rodata                  |
+--------------------------+
| Initial value of .data   |
+--------------------------+

SRAM
+--------------------------+  Low address
| .data                    |
+--------------------------+
| .bss                     |
+--------------------------+
| Heap                     |
|          grows up        |
+--------------------------+
|                          |
+--------------------------+
|          grows down      |
| Stack                    |
+--------------------------+  High address
```

---

# 7. Con trỏ và thao tác bộ nhớ trong C

## Đọc dữ liệu tại địa chỉ

```c
#include <stdint.h>

uint32_t read_memory(uint32_t address)
{
    volatile uint32_t *ptr = (volatile uint32_t *)address;
    return *ptr;
}
```

## Ghi dữ liệu tại địa chỉ

```c
#include <stdint.h>

void write_memory(uint32_t address, uint32_t value)
{
    volatile uint32_t *ptr = (volatile uint32_t *)address;
    *ptr = value;
}
```

## Vì sao cần `volatile`?

`volatile` thông báo cho compiler rằng giá trị có thể thay đổi ngoài luồng thực thi thông thường.

Các trường hợp thường dùng:

- Thanh ghi ngoại vi.
- Biến được thay đổi trong ISR.
- Biến dùng chung giữa nhiều context.
- Vùng nhớ được cập nhật bởi DMA.

Ví dụ:

```c
volatile uint32_t *gpio_output =
    (volatile uint32_t *)0x40000000U;
```

Không có `volatile`, compiler có thể tối ưu bỏ một số lần đọc hoặc ghi.

## Lưu ý an toàn

Không được đọc hoặc ghi vào địa chỉ tùy ý.

Một địa chỉ không hợp lệ có thể gây:

- HardFault.
- BusFault.
- Memory Management Fault.
- Hành vi không xác định.

---

# 8. Thanh ghi ngoại vi và Memory-Mapped I/O

Trong vi điều khiển, thanh ghi ngoại vi được ánh xạ vào không gian địa chỉ.

Ví dụ giả định:

```text
GPIO_BASE       = 0x40020000
GPIO_MODE_REG   = GPIO_BASE + 0x00
GPIO_OUTPUT_REG = GPIO_BASE + 0x14
```

## Cách 1: Truy cập trực tiếp

```c
#define GPIO_BASE        (0x40020000U)
#define GPIO_MODE_REG    (*(volatile uint32_t *)(GPIO_BASE + 0x00U))
#define GPIO_OUTPUT_REG  (*(volatile uint32_t *)(GPIO_BASE + 0x14U))
```

Ghi thanh ghi:

```c
GPIO_OUTPUT_REG |= (1U << 5);
```

## Cách 2: Sử dụng struct

```c
#include <stdint.h>

typedef struct
{
    volatile uint32_t MODE;
    volatile uint32_t TYPE;
    volatile uint32_t SPEED;
    volatile uint32_t PULL;
    volatile uint32_t INPUT;
    volatile uint32_t OUTPUT;
} gpio_registers_t;

#define GPIOA_BASE (0x40020000U)
#define GPIOA      ((gpio_registers_t *)GPIOA_BASE)
```

Sử dụng:

```c
GPIOA->OUTPUT |= (1U << 5);
```

Cách dùng struct dễ đọc và dễ bảo trì hơn, nhưng phải bảo đảm offset của các thành viên khớp với datasheet.

## Read-Modify-Write

```c
GPIOA->OUTPUT |= (1U << 5);
```

Dòng trên tương đương:

```c
uint32_t value = GPIOA->OUTPUT;
value = value | (1U << 5);
GPIOA->OUTPUT = value;
```

Cần cẩn thận khi thao tác với:

- Thanh ghi có bit tự xóa.
- Thanh ghi Write-1-to-Clear.
- Thanh ghi được thay đổi bởi phần cứng.
- Thanh ghi được sử dụng đồng thời trong interrupt.

---

# 9. Quy trình khởi động của vi điều khiển

Sau khi reset, Cortex-M thường thực hiện:

```text
Reset
  |
  v
Đọc Initial Stack Pointer từ Vector Table (phần tử đầu tiên)
  |
  v
Đọc địa chỉ Reset_Handler từ Vector Table (phần tử thứ hai)
  |
  v
Thực thi Reset_Handler
  |
  +--> Copy .data từ Flash sang RAM
  |
  +--> Khởi tạo vùng nhớ .bss (biến toàn cục = 0)
  |
  +--> Khởi tạo clock và runtime
  |
  v
Gọi main()
```

## Vector Table

Hai phần tử đầu tiên thường là:

```text
Vector[0] = Initial Stack Pointer
Vector[1] = Reset Handler
```

Các phần tử tiếp theo là địa chỉ của các exception và interrupt handler.

Ví dụ:

```c
typedef void (*isr_handler_t)(void);

extern uint32_t _estack;

void Reset_Handler(void);
void Default_Handler(void);

__attribute__((section(".isr_vector")))
const isr_handler_t vector_table[] =
{
    (isr_handler_t)&_estack,
    Reset_Handler,
    Default_Handler,
    Default_Handler,
};
```

## Reset Handler

Reset Handler chịu trách nhiệm chuẩn bị môi trường C.

Pseudo-code:

```c
void Reset_Handler(void)
{
    copy_data_section();
    clear_bss_section();
    system_init();
    main();

    while (1)
    {
    }
}
```

---

# 10. Các thành phần của dự án bare-metal

Một dự án bare-metal tối thiểu thường có:

## Toolchain

Ví dụ:

```text
arm-none-eabi-gcc
arm-none-eabi-objcopy
arm-none-eabi-size
arm-none-eabi-gdb
```

## Source Code

Các file C và Assembly:

```text
main.c
startup.c
system.c
```

## Startup Code

Startup Code:

- Định nghĩa vector table.
- Cài đặt Reset Handler.
- Sao chép `.data`.
- Xóa `.bss`.
- Gọi `main()`.

## Linker Script

Linker Script mô tả:

- Flash bắt đầu ở đâu.
- RAM bắt đầu ở đâu.
- Dung lượng từng vùng.
- Section nào được đặt vào Flash.
- Section nào được đặt vào RAM.
- Stack được đặt ở đâu.

## Makefile

Makefile tự động hóa:

- Compile.
- Assemble.
- Link.
- Tạo `.bin`.
- Tạo `.hex`.
- Hiển thị kích thước chương trình.
- Clean build.

## Debug Configuration

Có thể sử dụng:

- OpenOCD.
- ST-Link.
- GDB.
- J-Link.

---

# 11. Ví dụ tối thiểu

> Các địa chỉ dưới đây chỉ mang tính minh họa. Khi chạy trên board thật, phải thay bằng địa chỉ trong Reference Manual của MCU.

## 12.1 `src/main.c`

```c
#include <stdint.h>

#define GPIO_BASE       (0x40020000U)
#define GPIO_MODE       (*(volatile uint32_t *)(GPIO_BASE + 0x00U))
#define GPIO_OUTPUT     (*(volatile uint32_t *)(GPIO_BASE + 0x14U))

#define LED_PIN         (5U)

static void delay_cycles(volatile uint32_t cycles)
{
    while (cycles > 0U)
    {
        __asm volatile ("nop");
        cycles--;
    }
}

int main(void)
{
    GPIO_MODE |= (1U << (LED_PIN * 2U));

    while (1)
    {
        GPIO_OUTPUT ^= (1U << LED_PIN);
        delay_cycles(100000U);
    }
}
```

## 12.2 `startup/startup.c`

```c
#include <stdint.h>

extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _estack;

int main(void);

void Reset_Handler(void);
void Default_Handler(void);

typedef void (*isr_handler_t)(void);

__attribute__((section(".isr_vector")))
const isr_handler_t vector_table[] =
{
    (isr_handler_t)&_estack,
    Reset_Handler,
    Default_Handler,
    Default_Handler,
};

void Reset_Handler(void)
{
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;

    while (dst < &_edata)
    {
        *dst = *src;
        dst++;
        src++;
    }

    dst = &_sbss;

    while (dst < &_ebss)
    {
        *dst = 0U;
        dst++;
    }

    (void)main();

    while (1)
    {
    }
}

void Default_Handler(void)
{
    while (1)
    {
    }
}
```

## 12.3 `linker/memory.ld`

```ld
ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 128K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;
    } > FLASH

    _sidata = LOADADDR(.data);

    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT > FLASH

    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > RAM
}
```

## 12.4 `Makefile`

```makefile
TARGET := embedded_foundation

BUILD_DIR := build
SRC_DIR := src
STARTUP_DIR := startup
LINKER_SCRIPT := linker/memory.ld

CC := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size

CPU_FLAGS := -mcpu=cortex-m3 -mthumb
CFLAGS := $(CPU_FLAGS) \
          -std=c11 \
          -Wall \
          -Wextra \
          -Werror \
          -ffreestanding \
          -fdata-sections \
          -ffunction-sections \
          -O0 \
          -g3

LDFLAGS := $(CPU_FLAGS) \
           -T$(LINKER_SCRIPT) \
           -nostartfiles \
           -Wl,--gc-sections \
           -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

SOURCES := $(SRC_DIR)/main.c \
           $(STARTUP_DIR)/startup.c

OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCES))

ELF := $(BUILD_DIR)/$(TARGET).elf
BIN := $(BUILD_DIR)/$(TARGET).bin
HEX := $(BUILD_DIR)/$(TARGET).hex

.PHONY: all clean size

all: $(ELF) $(BIN) $(HEX) size

$(ELF): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

size: $(ELF)
	$(SIZE) $<

clean:
	rm -rf $(BUILD_DIR)
```

---

# 12. Quy trình build và phân tích firmware

## Cài đặt toolchain

Ubuntu/Debian:

```bash
sudo apt update
sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi make
```

Kiểm tra:

```bash
arm-none-eabi-gcc --version
arm-none-eabi-size --version
make --version
```

## Build

```bash
make
```

Các file đầu ra:

```text
build/embedded_foundation.elf
build/embedded_foundation.bin
build/embedded_foundation.hex
build/embedded_foundation.map
```

## Xem kích thước chương trình

```bash
arm-none-eabi-size build/embedded_foundation.elf
```

Ví dụ:

```text
   text    data     bss     dec     hex
   1024      16     256    1296     510
```

Ý nghĩa:

- `text`: code và dữ liệu chỉ đọc.
- `data`: dữ liệu khởi tạo nằm trong RAM.
- `bss`: dữ liệu zero-initialized nằm trong RAM.
- `dec`: tổng kích thước dạng decimal.
- `hex`: tổng kích thước dạng hexadecimal.

## Xem symbol

```bash
arm-none-eabi-nm -n build/embedded_foundation.elf
```

## Disassembly

```bash
arm-none-eabi-objdump -d build/embedded_foundation.elf
```

## Xem section

```bash
arm-none-eabi-objdump -h build/embedded_foundation.elf
```

---

# 13. Quy trình phát triển dự án bare-metal

Quy trình phát triển nên đi từ kiến thức phần cứng đến firmware có thể build, flash và kiểm tra:

```text
Đọc Datasheet / Reference Manual
        |
        v
Xác định memory map và peripheral
        |
        v
Tạo startup code và vector table
        |
        v
Viết linker script
        |
        v
Tạo Makefile và cấu hình toolchain
        |
        v
Viết driver GPIO/UART tối thiểu
        |
        v
Build ELF/BIN/HEX/MAP
        |
        v
Phân tích section, symbol và kích thước
        |
        v
Flash, chạy thử và ghi lại kết quả
```

## Bước 1 — Xác định target

Ghi rõ:

- MCU và board.
- Kiến trúc CPU.
- Địa chỉ Flash và SRAM.
- Dung lượng Flash và SRAM.
- Clock mặc định.
- Chân GPIO/UART sử dụng.
- Công cụ flash và debug.

## Bước 2 — Chuẩn bị nền tảng khởi động

Tạo:

- Vector table.
- `Reset_Handler`.
- Logic copy `.data`.
- Logic clear `.bss`.
- Linker script.
- Entry point.

Chỉ chuyển sang driver khi firmware đã vào được `main()` ổn định.

## Bước 3 — Xây dựng driver tối thiểu

Bắt đầu bằng:

1. GPIO output.
2. UART polling.
3. SysTick hoặc timer cơ bản nếu cần.

Mỗi driver phải có interface rõ ràng và không để application truy cập trực tiếp quá nhiều thanh ghi.

## Bước 4 — Build và phân tích

Sau mỗi thay đổi:

```bash
make clean
make
arm-none-eabi-size build/embedded_foundation.elf
arm-none-eabi-objdump -h build/embedded_foundation.elf
arm-none-eabi-nm -n build/embedded_foundation.elf
```

Kiểm tra:

- Section có nằm đúng vùng nhớ không.
- Symbol startup có tồn tại không.
- Vector table có được giữ lại không.
- Flash/RAM có vượt giới hạn không.

## Bước 5 — Chạy trên target

Xác nhận theo thứ tự:

1. Firmware flash thành công.
2. MCU vào `Reset_Handler`.
3. MCU vào `main()`.
4. GPIO hoạt động.
5. UART gửi được dữ liệu.
6. Không xảy ra HardFault.
7. Kết quả được ghi lại trong tài liệu project.

---

# 14. Best Practices

## Về tổ chức source code

- Tách `startup/`, `linker/`, `src/`, `include/`, `docs/` và `build/`.
- Không đặt toàn bộ startup, driver và application trong một file.
- Header chỉ công khai interface cần thiết.
- Dùng `static` cho hàm và biến chỉ sử dụng nội bộ module.
- Dùng tên file và tên symbol thể hiện đúng trách nhiệm.

## Về truy cập thanh ghi

- Luôn đối chiếu địa chỉ và bit field với Reference Manual.
- Dùng kiểu số nguyên có kích thước rõ ràng như `uint32_t`.
- Dùng `volatile` cho thanh ghi và vùng nhớ có thể thay đổi ngoài luồng thực thi thông thường.
- Dùng mask và macro có tên thay vì magic number rải rác.
- Bật clock ngoại vi trước khi truy cập thanh ghi ngoại vi.

## Về startup và linker

- Giữ vector table bằng `KEEP(*(.isr_vector))`.
- Căn chỉnh section theo yêu cầu của kiến trúc.
- Phân biệt địa chỉ load trong Flash và địa chỉ chạy trong RAM.
- Kiểm tra `_sidata`, `_sdata`, `_edata`, `_sbss`, `_ebss` và `_estack`.
- Không sao chép startup code hoặc linker script từ MCU khác mà không kiểm tra memory map.

## Về build

- Bật warning nghiêm ngặt như `-Wall`, `-Wextra` và `-Werror` trong giai đoạn học.
- Tạo file `.map` trong mỗi lần link.
- Dùng `size`, `nm` và `objdump` để kiểm tra output.
- Clean build khi thay đổi linker script, startup code hoặc compiler flags.
- Không commit thư mục `build/` nếu repository không yêu cầu lưu binary.

## Về tài liệu

- Ghi rõ MCU, board, clock, toolchain và cách flash.
- Vẽ memory map và startup flow.
- Ghi lại lỗi đã gặp và nguyên nhân.
- Mỗi bài thực hành nên có expected result và tiêu chí hoàn thành.

---

# 15. Anti-pattern và lỗi thường gặp

## `undefined reference`

Nguyên nhân:

- Thiếu source file.
- Khai báo hàm nhưng chưa định nghĩa.
- Sai tên symbol trong linker script.
- Thứ tự link không phù hợp.

## Vector table bị loại bỏ

Nguyên nhân:

- Linker garbage collection loại section không được tham chiếu.

Giải pháp:

```ld
KEEP(*(.isr_vector))
```

## Chương trình không vào `main()`

Kiểm tra:

- Địa chỉ `_estack`.
- Địa chỉ `Reset_Handler`.
- Vector table.
- `.data` và `.bss`.
- Entry point.
- Địa chỉ Flash.
- Startup file có thực sự được link hay không.

## HardFault khi truy cập thanh ghi

Nguyên nhân:

- Địa chỉ sai.
- Clock ngoại vi chưa được bật.
- Truy cập sai kích thước.
- Truy cập vùng nhớ không tồn tại.
- Cấu hình bus hoặc privilege không đúng.

## `.data` sai giá trị

Nguyên nhân:

- Không sao chép `.data` từ Flash sang RAM.
- `_sidata`, `_sdata`, `_edata` sai.
- Linker script không đúng.

## Biến toàn cục không bằng 0

Nguyên nhân:

- Không xóa `.bss`.
- `_sbss`, `_ebss` sai.

## Makefile không tạo được object file

Kiểm tra:

- Pattern rule.
- Đường dẫn source.
- Thư mục output.
- Tab trong recipe.
- Biến `OBJECTS`.

---

# 16. Kiểm thử

Kiểm thử ở Chủ đề 1 tập trung vào việc xác nhận startup, memory layout, build output và thao tác phần cứng tối thiểu.

## Kiểm tra trên host

Các nội dung có thể kiểm tra trên máy tính:

- Endianness.
- Phép toán bit.
- Ép kiểu con trỏ.
- Hàm thao tác buffer không phụ thuộc phần cứng.
- Quy tắc đặt dữ liệu vào section bằng cách phân tích object file.

Có thể build một chương trình C nhỏ bằng compiler host để quan sát byte order trước khi chạy trên MCU.

## Kiểm tra file build

Sau khi build, chạy:

```bash
arm-none-eabi-size build/embedded_foundation.elf
arm-none-eabi-objdump -h build/embedded_foundation.elf
arm-none-eabi-nm -n build/embedded_foundation.elf
arm-none-eabi-objdump -d build/embedded_foundation.elf
```

Cần xác nhận:

- `.isr_vector` nằm đầu vùng Flash.
- `.text` và `.rodata` nằm trong Flash.
- `.data` có VMA trong RAM và giá trị khởi tạo trong Flash.
- `.bss` nằm trong RAM.
- `Reset_Handler` và `main` có trong symbol table.
- Kích thước Flash/RAM không vượt target.

## Kiểm tra trên target

Thực hiện theo thứ tự:

1. Flash chương trình tối thiểu.
2. Xác nhận LED toggle.
3. Xác nhận UART gửi đúng chuỗi.
4. Đặt breakpoint tại `Reset_Handler` và `main()`.
5. Kiểm tra giá trị biến `.data`.
6. Kiểm tra biến `.bss` được khởi tạo bằng 0.
7. Cố ý thay đổi một phần cấu hình để quan sát lỗi và khôi phục.

## Fault injection đề xuất

- Bỏ `KEEP(*(.isr_vector))`.
- Không copy `.data`.
- Không clear `.bss`.
- Đặt sai địa chỉ Flash hoặc RAM.
- Dùng sai offset thanh ghi.
- Không bật clock GPIO.
- Xóa một source file khỏi Makefile.

Mỗi lỗi cần ghi lại:

- Triệu chứng.
- Nguyên nhân.
- Cách phát hiện.
- Cách sửa.

---

# 17. Bài thực hành

Các bài thực hành được sắp xếp từ cơ bản đến tích hợp:

## Bài 1 — Biểu diễn số và endianness

Viết chương trình:

```c
uint32_t value = 0x12345678U;
uint8_t *ptr = (uint8_t *)&value;
```

Quan sát:

```c
ptr[0]
ptr[1]
ptr[2]
ptr[3]
```

Mục tiêu:

- Hiểu byte order.
- Hiểu quan hệ giữa con trỏ và địa chỉ.
- Nhận biết little-endian.

---

## Bài 2 — Đọc và ghi bộ nhớ

Viết hai hàm:

```c
uint32_t memory_read32(uint32_t address);
void memory_write32(uint32_t address, uint32_t value);
```

Mục tiêu:

- Ép kiểu địa chỉ sang con trỏ.
- Hiểu `volatile`.
- Hiểu rủi ro khi truy cập địa chỉ không hợp lệ.

---

## Bài 3 — Khảo sát các section

Tạo các biến:

```c
const uint32_t version = 1U;
uint32_t initialized_data = 100U;
uint32_t uninitialized_data;
static uint8_t static_buffer[128];
```

Sau đó dùng:

```bash
arm-none-eabi-nm -n build/embedded_foundation.elf
arm-none-eabi-objdump -h build/embedded_foundation.elf
```

Mục tiêu:

- Xác định biến nằm trong `.rodata`, `.data` hay `.bss`.
- Hiểu vai trò của linker.

---

## Bài 4 — Tự viết Startup Code

Yêu cầu:

- Tạo vector table.
- Tạo `Reset_Handler`.
- Copy `.data`.
- Clear `.bss`.
- Gọi `main()`.

Mục tiêu:

- Hiểu quá trình từ reset tới `main()`.
- Không phụ thuộc vào startup code có sẵn của IDE.

---

## Bài 5 — Tự viết Linker Script

Yêu cầu:

- Khai báo Flash.
- Khai báo RAM.
- Đặt `.isr_vector` vào Flash.
- Đặt `.text` vào Flash.
- Đặt `.data` vào RAM nhưng load từ Flash.
- Đặt `.bss` vào RAM.

Mục tiêu:

- Hiểu VMA và LMA.
- Hiểu cách chương trình được bố trí trong bộ nhớ.

---

## Bài 6 — Điều khiển GPIO bằng thanh ghi

Yêu cầu:

- Bật clock cho GPIO.
- Cấu hình chân LED output.
- Toggle LED.
- Không sử dụng HAL.

Mục tiêu:

- Đọc Reference Manual.
- Xác định địa chỉ base.
- Xác định offset thanh ghi.
- Thực hiện bit manipulation.

---

## Bài 7 — UART polling

Yêu cầu:

- Khởi tạo UART.
- Gửi chuỗi `"Hello Bare Metal\r\n"`.
- Không sử dụng thư viện chuẩn nặng.

Mục tiêu:

- Hiểu peripheral register.
- Hiểu polling.
- Biết kiểm tra status flag.

---

## Bài 8 — Phân tích file `.map`

Yêu cầu:

- Xác định kích thước `.text`.
- Xác định kích thước `.data`.
- Xác định kích thước `.bss`.
- Tìm symbol chiếm nhiều bộ nhớ nhất.
- Kiểm tra stack và heap.

Mục tiêu:

- Biết phân tích mức sử dụng Flash/RAM.
- Chuẩn bị cho việc tối ưu firmware.

---

# 18. Tiêu chí hoàn thành

Người học được xem là hoàn thành chủ đề khi có thể:

- Giải thích `PC`, `SP`, `LR`.
- Vẽ được sơ đồ Flash, SRAM, peripheral.
- Phân biệt `.text`, `.rodata`, `.data`, `.bss`, stack và heap.
- Giải thích được `volatile`.
- Đọc và ghi một thanh ghi bằng con trỏ.
- Giải thích được Memory-Mapped I/O.
- Tự viết vector table tối thiểu.
- Tự viết `Reset_Handler`.
- Tự viết linker script.
- Tự viết Makefile.
- Build được `.elf`, `.bin`, `.hex`, `.map`.
- Dùng `objdump`, `nm`, `size` để phân tích firmware.
- Điều khiển GPIO không dùng HAL.
- Commit source code và tài liệu lên GitHub.

---

# 19. Cấu trúc repository đề xuất

## Kết quả đầu ra đề xuất

Cấu trúc repository đề xuất:

```text
01-embedded-foundation/
├── README.md
├── Makefile
├── .gitignore
├── linker/
│   └── memory.ld
├── startup/
│   └── startup.c
├── src/
│   ├── main.c
│   ├── gpio.c
│   └── uart.c
├── include/
│   ├── gpio.h
│   └── uart.h
├── labs/
│   ├── 01-endianness/ # practice 1
│   ├── 02-memory-access/ # practice 2
│   ├── 03-memory-sections/ # practice 3
│   ├── 04-startup-code/ # practice 4
│   ├── 05-linker-script/ # practice 5
│   ├── 06-gpio-register/ # practice 6
│   ├── 07-uart-polling/ # practice 7
│   └── 08-map-analysis/ # practice 8
├── docs/
│   ├── memory-map.md
│   ├── startup-flow.md
│   ├── sections-analysis.md
│   └── map-analysis.md
└── build/
```

# Nội dung README của project
- Mục tiêu.
- Board và MCU.
- Kiến trúc CPU và thông số bộ nhớ.
- Build và flash.
- Memory map của MCU.
- Bố trí các section
- Luồng khởi động từ Reset đến main().
- Cấu trúc Vector Table.
- Trách nhiệm của Reset_Handler.
- Cấu hình Linker Script.
- Cấu hình Makefile.
- Compiler và linker flags quan trọng.
- Địa chỉ thanh ghi và bit cấu hình quan trọng.

---

# 20. Tài liệu tham khảo

Các tài liệu nên tham khảo:

## Programming Paradigms — Stanford CS107

- https://www.youtube.com/playlist?list=PL9D558D49CA734A02

## AK Embedded Base Kit — Getting Started

- https://epcb.vn/blogs/ak-embedded-software/ak-embedded-base-kit-stm32l151-getting-started

## Application Startup Code

- https://epcb.vn/blogs/ak-embedded-software/ak-embedded-base-kit-stm32l151-application-startup-code

## Building Bare-Metal ARM Systems with GNU

- https://www.state-machine.com/doc/Building_bare-metal_ARM_with_GNU.pdf

## Tài liệu MCU cần đọc thêm

- Datasheet.
- Reference Manual.
- ARM Cortex-M Generic User Guide.
- ARM Architecture Reference Manual.
- Schematic của board.
- Errata Sheet.

---

# 21. Tổng kết

Xây dựng nền móng để hiểu một firmware bare-metal từ lúc MCU reset đến khi application chạy.

Luồng kiến thức chính:

```text
CPU và bus
      |
      v
Không gian địa chỉ
      |
      v
Flash, SRAM và section
      |
      v
Con trỏ và Memory-Mapped I/O
      |
      v
Vector table và Reset Handler
      |
      v
Linker script
      |
      v
Makefile và toolchain
      |
      v
GPIO/UART mức thanh ghi
      |
      v
Phân tích ELF, MAP và memory usage
```

Các nguyên tắc cần nhớ:

1. Địa chỉ và dữ liệu là hai khái niệm khác nhau.
2. Thanh ghi ngoại vi được truy cập qua Memory-Mapped I/O.
3. `volatile` cần thiết với dữ liệu có thể thay đổi ngoài luồng C thông thường.
4. Startup code phải chuẩn bị `.data` và `.bss` trước khi gọi `main()`.
5. Linker script quyết định vị trí của code và dữ liệu.
6. Vector table phải nằm đúng địa chỉ và không bị linker loại bỏ.
7. Build thành công chưa đủ; cần kiểm tra ELF, MAP, section và symbol.
8. Địa chỉ thanh ghi phải lấy từ tài liệu chính thức của MCU.
9. Warning của compiler cần được xử lý thay vì bỏ qua.
10. Mọi bài bare-metal nên có cách kiểm tra trên target và tài liệu kết quả.

Sau chủ đề này, người học cần có một project bare-metal tối thiểu có thể:

```text
Khởi động từ Reset_Handler
+
Khởi tạo .data và .bss
+
Điều khiển GPIO
+
Gửi UART polling
+
Build ELF/BIN/HEX/MAP
+
Phân tích Flash/RAM
```

Đây là nền tảng trực tiếp cho các chủ đề tiếp theo về interrupt, timer, Event-Driven Programming, scheduler và RTOS.

