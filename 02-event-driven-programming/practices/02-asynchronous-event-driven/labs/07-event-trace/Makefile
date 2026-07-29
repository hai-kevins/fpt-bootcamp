TARGET := lab07_event_trace
BUILD_DIR ?= build
CC ?= gcc

CFLAGS := -std=c11 -O0 -g3 \
	-Wall -Wextra -Werror -Wshadow -Wundef \
	-Wconversion -Wsign-conversion \
	-fsanitize=address,undefined -Iinclude

SOURCES := src/event_trace.c src/main.c

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


