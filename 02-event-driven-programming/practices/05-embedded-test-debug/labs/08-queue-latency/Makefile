CC ?= gcc
BUILD_DIR := build
TARGET := $(BUILD_DIR)/lab

CFLAGS := \
	-std=c11 \
	-O0 \
	-g3 \
	-Wall \
	-Wextra \
	-Werror \
	-Wpedantic \
	-Wshadow

SANITIZER_FLAGS := \
	-fsanitize=address,undefined \
	-fno-omit-frame-pointer

.PHONY: all run test sanitize clean

all: $(TARGET)

$(TARGET): main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

run: $(TARGET)
	./$(TARGET)

test: run

sanitize:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SANITIZER_FLAGS) main.c -o $(BUILD_DIR)/lab_sanitize
	./$(BUILD_DIR)/lab_sanitize

clean:
	rm -rf $(BUILD_DIR)
