TARGET := lab04_button_debounce
BUILD_DIR ?= build
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

CPU_FLAGS := -mcpu=cortex-m3 -mthumb

CFLAGS := $(TARGET_FLAGS) $(CPU_FLAGS) \
	-std=c11 -Og -g3 \
	-Wall -Wextra -Werror -Wshadow -Wundef \
	-Wconversion -Wsign-conversion \
	-ffreestanding -fno-builtin -fno-common \
	-fno-unwind-tables -fno-asynchronous-unwind-tables \
	-ffunction-sections -fdata-sections \
	-MMD -MP -Iinclude

LDFLAGS := $(TARGET_FLAGS) $(LINKER_DRIVER) $(CPU_FLAGS) \
	-nostdlib -nostartfiles \
	-Tlinker/memory.ld \
	-Wl,--gc-sections \
	-Wl,--build-id=none \
	-Wl,-Map=$(BUILD_DIR)/$(TARGET).map

SOURCES := \
	startup/startup.c \
	src/runtime.c \
	src/event_queue.c \
	src/software_timer.c \
	src/main.c \
	platform/gpio.c \
	platform/systick.c \
	platform/critical_section.c

OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)

ELF := $(BUILD_DIR)/$(TARGET).elf
BIN := $(BUILD_DIR)/$(TARGET).bin
HEX := $(BUILD_DIR)/$(TARGET).hex
LST := $(BUILD_DIR)/$(TARGET).lst

.PHONY: all clean rebuild size sections symbols report 	flash-stlink flash-openocd

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

sections: $(ELF)
	$(OBJDUMP) -h $<

symbols: $(ELF)
	$(NM) -S --size-sort $<

report: $(ELF)
	@mkdir -p $(BUILD_DIR)
	$(SIZE) $(ELF) > $(BUILD_DIR)/size-report.txt
	$(OBJDUMP) -h $(ELF) > $(BUILD_DIR)/section-report.txt
	$(NM) -S --size-sort $(ELF) > $(BUILD_DIR)/symbol-report.txt

flash-stlink: $(BIN)
	st-flash write $(BIN) 0x08000000

flash-openocd: $(ELF)
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
		-c "program $(ELF) verify reset exit"

rebuild: clean all

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
