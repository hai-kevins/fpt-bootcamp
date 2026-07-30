TARGET := lab13-fatal-record
BUILD_DIR ?= build
CC ?= gcc
CFLAGS := -std=c11 -O0 -g3 -Wall -Wextra -Werror -Wshadow -Wundef -Wconversion -Wsign-conversion -fsanitize=address,undefined -Iinclude
SOURCES := src/main.c

.PHONY: all test run clean rebuild
all: test
$(BUILD_DIR)/$(TARGET): $(SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SOURCES) -o $@
test: $(BUILD_DIR)/$(TARGET)
	$(BUILD_DIR)/$(TARGET)
run: test
rebuild: clean all
clean:
	rm -rf $(BUILD_DIR)
