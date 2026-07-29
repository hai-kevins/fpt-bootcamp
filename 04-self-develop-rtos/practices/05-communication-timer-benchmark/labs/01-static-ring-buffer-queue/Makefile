TARGET := 01-static-ring-buffer-queue
BUILD_DIR := build
CC ?= cc
CPPFLAGS := -Iinclude
CFLAGS := -std=c11 -Og -g3 -Wall -Wextra -Werror -Wshadow -Wpedantic \
    -fsanitize=address,undefined -fno-omit-frame-pointer
LDFLAGS := -fsanitize=address,undefined
SOURCES := src/ring_queue.c
TEST := $(BUILD_DIR)/tests
DEMO := $(BUILD_DIR)/01-static-ring-buffer-queue

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
