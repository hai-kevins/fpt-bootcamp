TARGET := active_kernel
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
INCLUDES := -Iak/inc -Iapplication/include -Idriver/inc -Iplatform/inc

CFLAGS := $(TARGET_FLAGS) $(CPU_FLAGS) \
	-std=c11 -Og -g3 \
	-Wall -Wextra -Werror -Wshadow -Wundef \
	-Wconversion -Wsign-conversion \
	-ffreestanding -fno-builtin -fno-common \
	-fno-unwind-tables -fno-asynchronous-unwind-tables \
	-ffunction-sections -fdata-sections -MMD -MP $(INCLUDES)

LDFLAGS := $(TARGET_FLAGS) $(LINKER_DRIVER) $(CPU_FLAGS) \
	-nostdlib -nostartfiles -Tlinker/memory.ld \
	-Wl,--gc-sections -Wl,--build-id=none \
	-Wl,-Map=$(BUILD_DIR)/$(TARGET).map

KERNEL_SOURCES := \
	ak/src/ak.c ak/src/message.c ak/src/task.c ak/src/timer.c \
	ak/src/fsm.c ak/src/tsm.c ak/src/event_record.c ak/src/fatal.c \
	ak/src/ak_port_default.c

APP_SOURCES := \
	application/src/app.c application/src/task_list.c \
	application/src/task_app.c application/src/task_button.c \
	application/src/task_led.c application/src/task_shell.c \
	application/src/task_sensor.c application/src/main.c

DRIVER_SOURCES := driver/src/led.c driver/src/button.c driver/src/uart.c
PLATFORM_SOURCES := \
	platform/src/runtime.c platform/src/platform.c \
	platform/src/critical_section.c platform/src/gpio.c \
	platform/src/uart.c platform/src/systick.c platform/src/reset.c

SOURCES := startup/startup.c $(KERNEL_SOURCES) $(APP_SOURCES) \
	$(DRIVER_SOURCES) $(PLATFORM_SOURCES)
OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCES))
DEPENDENCIES := $(OBJECTS:.o=.d)

ELF := $(BUILD_DIR)/$(TARGET).elf
BIN := $(BUILD_DIR)/$(TARGET).bin
HEX := $(BUILD_DIR)/$(TARGET).hex
LST := $(BUILD_DIR)/$(TARGET).lst

HOST_TEST := $(BUILD_DIR)/host/test_runner
HOST_SOURCES := $(KERNEL_SOURCES) \
	application/src/app.c application/src/task_list.c \
	application/src/task_app.c application/src/task_button.c \
	application/src/task_led.c application/src/task_shell.c \
	application/src/task_sensor.c \
	tests/fakes.c tests/test_message.c tests/test_task.c tests/test_timer.c \
	tests/test_fsm.c tests/test_event_record.c tests/test_fatal.c \
	tests/test_application.c tests/test_main.c
HOST_CFLAGS := -std=c11 -O0 -g3 \
	-Wall -Wextra -Werror -Wshadow -Wundef \
	-Wconversion -Wsign-conversion \
	-fsanitize=address,undefined $(INCLUDES) -Itests

.PHONY: all test clean rebuild size sections symbols symbols-size disasm \
	flash-stlink flash-openocd erase help

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
	@echo "Final project only: Active Kernel on STM32F103"
	@echo "Build   : make | make TOOLCHAIN=clang"
	@echo "Test    : make test"
	@echo "Inspect : make size | make sections | make symbols | make symbols-size"
	@echo "Flash   : make flash-stlink | make flash-openocd | make erase"
	@echo "Labs are built from inside their own directories."

-include $(DEPENDENCIES)
