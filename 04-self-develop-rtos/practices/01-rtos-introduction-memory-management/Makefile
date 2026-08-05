TARGET := rtos_memory_foundation
BUILD_DIR ?= build
SRC_DIR := src
STARTUP_DIR := startup
INCLUDE_DIR := include
LINKER_SCRIPT := linker/memory.ld

TOOLCHAIN ?= gcc
ifeq ($(TOOLCHAIN),clang)
CC := clang
OBJCOPY := llvm-objcopy
OBJDUMP := llvm-objdump
SIZE := size
NM := nm
TARGET_FLAGS := --target=arm-none-eabi
LINKER_DRIVER := -fuse-ld=lld
else
CC := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump
SIZE := arm-none-eabi-size
NM := arm-none-eabi-nm
TARGET_FLAGS :=
LINKER_DRIVER :=
endif

HOST_CC ?= gcc

CPU_FLAGS := -mcpu=cortex-m3 -mthumb
CFLAGS := $(TARGET_FLAGS) $(CPU_FLAGS) \
	-std=c11 -Og -g3 \
	-Wall -Wextra -Werror -Wshadow -Wundef \
	-Wconversion -Wsign-conversion \
	-ffreestanding -fno-builtin -fno-common \
	-fno-unwind-tables -fno-asynchronous-unwind-tables \
	-ffunction-sections -fdata-sections \
	-MMD -MP -I$(INCLUDE_DIR)

LDFLAGS := $(TARGET_FLAGS) $(LINKER_DRIVER) $(CPU_FLAGS) \
	-nostdlib -nostartfiles \
	-T$(LINKER_SCRIPT) \
	-Wl,--gc-sections \
	-Wl,--build-id=none \
	-Wl,-Map=$(BUILD_DIR)/$(TARGET).map

SOURCES := \
	$(STARTUP_DIR)/startup.c \
	$(SRC_DIR)/runtime.c \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/gpio.c \
	$(SRC_DIR)/uart.c \
	$(SRC_DIR)/systick.c \
	$(SRC_DIR)/cmd_line.c \
	$(SRC_DIR)/heap.c \
	$(SRC_DIR)/heap_stats.c \
	$(SRC_DIR)/memory_layout.c \
	$(SRC_DIR)/memory_explorer.c \
	$(SRC_DIR)/stack_monitor.c

OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCES))
DEPENDENCIES := $(OBJECTS:.o=.d)
ELF := $(BUILD_DIR)/$(TARGET).elf
BIN := $(BUILD_DIR)/$(TARGET).bin
HEX := $(BUILD_DIR)/$(TARGET).hex
LST := $(BUILD_DIR)/$(TARGET).lst
MAP := $(BUILD_DIR)/$(TARGET).map

HOST_TEST := $(BUILD_DIR)/host/test_cmd_line
HOST_SOURCES := $(SRC_DIR)/cmd_line.c tests/test_cmd_line.c
HOST_CFLAGS := \
	-std=c11 -O0 -g3 \
	-Wall -Wextra -Werror -Wshadow -Wundef \
	-Wconversion -Wsign-conversion \
	-fsanitize=address,undefined \
	-I$(INCLUDE_DIR)

.PHONY: all test clean rebuild size sections symbols symbols-size disasm \
	flash-stlink flash-openocd erase help

all: $(ELF) $(BIN) $(HEX) $(LST) size

$(ELF): $(OBJECTS) $(LINKER_SCRIPT)
	@mkdir -p $(dir $@)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

$(LST): $(ELF)
	$(OBJDUMP) -d -S $< > $@

$(HOST_TEST): $(HOST_SOURCES)
	@mkdir -p $(dir $@)
	$(HOST_CC) $(HOST_CFLAGS) $(HOST_SOURCES) -o $@

test: $(HOST_TEST)
	$(HOST_TEST)

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
	@echo "Final project only: RTOS Memory Explorer"
	@echo "Build   : make | make TOOLCHAIN=clang"
	@echo "Test    : make test"
	@echo "Inspect : make size | make sections | make symbols | make symbols-size | make disasm"
	@echo "Flash   : make flash-stlink | make flash-openocd | make erase"
	@echo "Clean   : make clean | make rebuild"
	@echo "Labs are built from inside their own directories."

-include $(DEPENDENCIES)
