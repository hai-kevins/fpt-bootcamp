TARGET := lab02_event_queue
BUILD_DIR ?= build
CC ?= gcc

CFLAGS := -std=c11 -O0 -g3 \
	-Wall -Wextra -Werror -Wshadow -Wundef \
	-Wconversion -Wsign-conversion \
	-fsanitize=address,undefined -Iinclude

SOURCES := src/event_queue.c src/main.c

.PHONY: all run clean rebuild test

all: $(BUILD_DIR)/$(TARGET) test

$(BUILD_DIR)/$(TARGET): $(SOURCES)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

run: $(BUILD_DIR)/$(TARGET)
	$(BUILD_DIR)/$(TARGET)

TEST_TARGET := $(BUILD_DIR)/test_runner
TEST_SOURCES := src/event_queue.c tests/test_main.c

$(TEST_TARGET): $(TEST_SOURCES)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(TEST_SOURCES) -o $@

test: $(TEST_TARGET)
	$(TEST_TARGET)

rebuild: clean all

clean:
	rm -rf $(BUILD_DIR)


