TARGET := lab08_queue_stress_test
BUILD_DIR ?= build
CC ?= gcc

CFLAGS := -std=c11 -O0 -g3 \
	-Wall -Wextra -Werror -Wshadow -Wundef \
	-Wconversion -Wsign-conversion \
	-fsanitize=address,undefined -Iinclude

SOURCES := main.c

.PHONY: all run clean rebuild

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(SOURCES)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

run: $(BUILD_DIR)/$(TARGET)
	$(BUILD_DIR)/$(TARGET)

rebuild: clean all

clean:
	rm -rf $(BUILD_DIR)


