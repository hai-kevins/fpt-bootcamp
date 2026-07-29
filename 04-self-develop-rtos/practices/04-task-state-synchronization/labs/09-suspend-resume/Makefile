CC ?= cc
TARGET := demo
BUILD_DIR := build
CPPFLAGS := -Iinclude
CFLAGS := -std=c11 -Og -g3 -Wall -Wextra -Werror -Wshadow -Wpedantic \
          -fsanitize=address,undefined -fno-omit-frame-pointer
LDFLAGS := -fsanitize=address,undefined
SOURCES := $(wildcard src/*.c)
TEST_SOURCES := $(wildcard tests/*.c)
DEMO := $(BUILD_DIR)/$(TARGET)
TEST := $(BUILD_DIR)/tests

.PHONY: all test run clean help

all: test $(DEMO)

$(DEMO): $(SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $@

$(TEST): $(filter-out src/main.c,$(SOURCES)) $(TEST_SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(filter-out src/main.c,$(SOURCES)) $(TEST_SOURCES) $(LDFLAGS) -o $@

test: $(TEST)
	ASAN_OPTIONS=detect_leaks=1 $(TEST)

run: $(DEMO)
	$(DEMO)

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo "make test  Build and run this lab's tests"
	@echo "make run   Build and run this lab's demo"
	@echo "make clean Remove this lab's build output"
