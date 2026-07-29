TARGET := asynchronous_event_driven
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

HOST_CC ?= gcc

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
	src/event_trace.c \
	src/dispatcher.c \
	src/software_timer.c \
	src/app.c \
	src/button.c \
	src/led.c \
	src/uart_service.c \
	src/main.c \
	platform/platform.c \
	platform/gpio.c \
	platform/uart.c \
	platform/systick.c \
	platform/critical_section.c

OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCES))
DEPENDENCIES := $(OBJECTS:.o=.d)

ELF := $(BUILD_DIR)/$(TARGET).elf
BIN := $(BUILD_DIR)/$(TARGET).bin
HEX := $(BUILD_DIR)/$(TARGET).hex
LST := $(BUILD_DIR)/$(TARGET).lst

HOST_BUILD_DIR := $(BUILD_DIR)/host
HOST_TEST := $(HOST_BUILD_DIR)/test_runner
HOST_SOURCES := \
	src/event_queue.c \
	src/event_trace.c \
	src/dispatcher.c \
	src/software_timer.c \
	src/app.c \
	src/button.c \
	src/led.c \
	src/uart_service.c \
	tests/fakes.c \
	tests/test_support.c \
	tests/test_event_queue.c \
	tests/test_timer.c \
	tests/test_button_sm.c \
	tests/test_led_sm.c \
	tests/test_app_sm.c \
	tests/test_main.c

HOST_CFLAGS := \
	-std=c11 -O0 -g3 \
	-Wall -Wextra -Werror -Wshadow -Wundef \
	-Wconversion -Wsign-conversion \
	-fsanitize=address,undefined \
	-Iinclude -Itests

.PHONY: all test run-tests clean rebuild size sections symbols \
	symbols-size disasm flash-stlink flash-openocd erase help

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

$(HOST_TEST): $(HOST_SOURCES)
	@mkdir -p $(dir $@)
	$(HOST_CC) $(HOST_CFLAGS) $(HOST_SOURCES) -o $@

test: $(HOST_TEST)
	$(HOST_TEST)

run-tests: test

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
	@echo "Final project only: Asynchronous Event-Driven Firmware"
	@echo "Build   : make | make TOOLCHAIN=clang"
	@echo "Test    : make test"
	@echo "Inspect : make size | make sections | make symbols | make symbols-size | make disasm"
	@echo "Flash   : make flash-stlink | make flash-openocd | make erase"
	@echo "Clean   : make clean | make rebuild"
	@echo "Labs are built from inside their own directories."

-include $(DEPENDENCIES)
