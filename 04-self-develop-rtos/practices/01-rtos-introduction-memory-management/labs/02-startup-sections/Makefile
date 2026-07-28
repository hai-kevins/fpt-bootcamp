TARGET := 02_startup_sections
BUILD_DIR ?= build
TOOLCHAIN ?= gcc

ifeq ($(TOOLCHAIN),clang)
CC := clang
OBJCOPY := llvm-objcopy
OBJDUMP := llvm-objdump
SIZE := size
TARGET_FLAGS := --target=arm-none-eabi
LINKER_DRIVER := -fuse-ld=lld
else
CC := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump
SIZE := arm-none-eabi-size
TARGET_FLAGS :=
LINKER_DRIVER :=
endif

CPU_FLAGS := -mcpu=cortex-m3 -mthumb
CFLAGS := $(TARGET_FLAGS) $(CPU_FLAGS) -std=c11 -Og -g3 \
	-Wall -Wextra -Werror -Wshadow -Wundef -Wconversion -Wsign-conversion \
	-ffreestanding -fno-builtin -fno-common -fno-unwind-tables \
	-fno-asynchronous-unwind-tables -ffunction-sections -fdata-sections \
	-MMD -MP -Iinclude
LDFLAGS := $(TARGET_FLAGS) $(LINKER_DRIVER) $(CPU_FLAGS) -nostdlib -nostartfiles \
	-Tlinker/memory.ld -Wl,--gc-sections -Wl,--build-id=none \
	-Wl,-Map=$(BUILD_DIR)/$(TARGET).map
SOURCES := startup/startup.c src/runtime.c src/gpio.c src/uart.c src/systick.c src/main.c
OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)
ELF := $(BUILD_DIR)/$(TARGET).elf
BIN := $(BUILD_DIR)/$(TARGET).bin
HEX := $(BUILD_DIR)/$(TARGET).hex
LST := $(BUILD_DIR)/$(TARGET).lst

.PHONY: all size flash-stlink flash-openocd clean rebuild

all: $(ELF) $(BIN) $(HEX) $(LST) size
$(ELF): $(OBJECTS) linker/memory.ld
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

size: $(ELF)
	$(SIZE) $<

flash-stlink: $(BIN)
	st-flash write $(BIN) 0x08000000

flash-openocd: $(BIN)
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg

rebuild: clean all

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
