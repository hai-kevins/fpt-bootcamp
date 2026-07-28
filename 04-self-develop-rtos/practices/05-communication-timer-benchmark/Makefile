TARGET := communication_benchmark_console
BUILD_DIR ?= build
LINKER_SCRIPT := linker/memory.ld

GNU_GCC := $(shell command -v arm-none-eabi-gcc 2>/dev/null)

ifeq ($(strip $(GNU_GCC)),)
CC := clang
OBJCOPY := llvm-objcopy
OBJDUMP := llvm-objdump
SIZE := size
NM := nm
TOOLCHAIN_FLAGS := --target=arm-none-eabi -mcpu=cortex-m3 -mthumb
LINKER_DRIVER_FLAGS := -fuse-ld=lld
else
CC := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump
SIZE := arm-none-eabi-size
NM := arm-none-eabi-nm
TOOLCHAIN_FLAGS := -mcpu=cortex-m3 -mthumb
LINKER_DRIVER_FLAGS :=
endif

CPPFLAGS := -Iinclude
CFLAGS := $(TOOLCHAIN_FLAGS) -std=c11 -Og -g3 \
	-Wall -Wextra -Werror -Wshadow -Wundef \
	-ffreestanding -fno-builtin -fno-common \
	-fno-unwind-tables -fno-asynchronous-unwind-tables \
	-ffunction-sections -fdata-sections -MMD -MP
ASFLAGS := $(TOOLCHAIN_FLAGS) -x assembler-with-cpp -g3
LDFLAGS := $(TOOLCHAIN_FLAGS) $(LINKER_DRIVER_FLAGS) \
	-nostdlib -nostartfiles -T$(LINKER_SCRIPT) \
	-Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

C_SOURCES := $(wildcard startup/*.c) $(wildcard src/*.c)
S_SOURCES := $(wildcard startup/*.S) $(wildcard src/*.S)
C_OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
S_OBJECTS := $(patsubst %.S,$(BUILD_DIR)/%.o,$(S_SOURCES))
OBJECTS := $(C_OBJECTS) $(S_OBJECTS)
DEPENDENCIES := $(C_OBJECTS:.o=.d)

ELF := $(BUILD_DIR)/$(TARGET).elf
BIN := $(BUILD_DIR)/$(TARGET).bin
HEX := $(BUILD_DIR)/$(TARGET).hex
LST := $(BUILD_DIR)/$(TARGET).lst

.PHONY: all clean rebuild size sections symbols symbols-size disasm \
	flash-stlink flash-openocd erase help

all: $(ELF) $(BIN) $(HEX) $(LST) size

$(ELF): $(OBJECTS) $(LINKER_SCRIPT)
	@mkdir -p $(dir $@)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(ASFLAGS) -c $< -o $@

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

$(LST): $(ELF)
	$(OBJDUMP) -d -S $< > $@

size: $(ELF)
	$(SIZE) $<

sections: $(ELF)
	$(OBJDUMP) -h $<

symbols: $(ELF)
	$(NM) -n $<

symbols-size: $(ELF)
	$(NM) -S --size-sort $<

disasm: $(LST)
	@echo "Generated $(LST)"

flash-stlink: $(BIN)
	st-flash write $(BIN) 0x08000000

flash-openocd: $(ELF)
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
		-c "program $(ELF) verify reset exit"

erase:
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
		-c "init; reset halt; stm32f1x mass_erase 0; reset run; exit"

rebuild: clean all

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo "make              Build HairRTOS Communication and Benchmark Console"
	@echo "make size         Show section sizes"
	@echo "make sections     Show ELF sections"
	@echo "make symbols      Show symbols by address"
	@echo "make symbols-size Show symbols by size"
	@echo "make disasm       Generate annotated disassembly"
	@echo "make flash-stlink Flash BIN with st-flash"
	@echo "make flash-openocd Flash ELF with OpenOCD"
	@echo "make clean        Remove root build output"
	@echo "Lab Makefiles are independent and are not called here."

-include $(DEPENDENCIES)
