TARGET := 04-message-pool
BUILD_DIR := build
CC ?= cc
CPPFLAGS := -Iinclude
CFLAGS := -std=c11 -Og -g3 -Wall -Wextra -Werror -Wshadow -Wpedantic \
    -fsanitize=address,undefined -fno-omit-frame-pointer
LDFLAGS := -fsanitize=address,undefined
SOURCES := src/message_pool.c
TEST := $(BUILD_DIR)/tests
DEMO := $(BUILD_DIR)/04-message-pool

.PHONY: all test run clean help

all: test $(DEMO)

test: $(TEST)
	ASAN_OPTIONS=detect_leaks=1 $(TEST)
$(TEST): $(SOURCES) tests/test.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ $(LDFLAGS) -o $@
$(DEMO): $(SOURCES) src/main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ $(LDFLAGS) -o $@

run: $(DEMO)
	./$(DEMO)

clean:
	rm -rf $(BUILD_DIR)
	
help:
	@echo "make | make test | make run | make clean"
