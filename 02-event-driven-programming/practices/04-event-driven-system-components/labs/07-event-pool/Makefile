ROOT := ../..
BUILD_DIR := build
CC ?= gcc
TARGET := $(BUILD_DIR)/lab

CPPFLAGS := \
    -I$(ROOT)/framework/include \
    -I$(ROOT)/datalink/include \
    -I$(ROOT)/application/include

CFLAGS := \
    -std=c11 -O0 -g3 \
    -Wall -Wextra -Werror -Wpedantic \
    -Wshadow -Wconversion -Wsign-conversion

SANITIZER_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer
SOURCES := main.c $(ROOT)/framework/src/event_pool.c $(ROOT)/framework/src/event.c

.PHONY: all run test sanitize clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

run: $(TARGET)
	./$(TARGET)

test: run

sanitize:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SANITIZER_FLAGS) $(SOURCES) -o $(BUILD_DIR)/lab_sanitize
	./$(BUILD_DIR)/lab_sanitize

clean:
	rm -rf $(BUILD_DIR)
